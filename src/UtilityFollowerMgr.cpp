/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "UtilityFollowerMgr.h"
#include "Chat/Chat.h"
#include "Objects/Creature.h"
#include "Objects/Player.h"
#include "Objects/TemporarySummon.h"
#include "Maps/Map.h"
#include "Movement/MotionMaster.h"
#include "Log.h"
#include "Timer.h"
#include "WorldSession.h"
#include "UtilityFollowerAI.h"
#include "UtilityFollowerConfig.h"
#include "UtilityFollowerServices.h"

#ifndef PET_FOLLOW_ANGLE
#define PET_FOLLOW_ANGLE (M_PI_F / 2.0f)
#endif

UtilityFollowerMgr* UtilityFollowerMgr::instance()
{
    static UtilityFollowerMgr instance;
    return &instance;
}

bool UtilityFollowerMgr::IsFollower(ObjectGuid const& creatureGuid) const
{
    std::lock_guard<std::mutex> lock(_lock);
    return _followerToOwner.find(creatureGuid) != _followerToOwner.end();
}

ObjectGuid UtilityFollowerMgr::GetFollowerOwner(ObjectGuid const& creatureGuid) const
{
    std::lock_guard<std::mutex> lock(_lock);
    auto itr = _followerToOwner.find(creatureGuid);
    if (itr != _followerToOwner.end())
        return itr->second.first;

    return ObjectGuid();
}

FollowerType UtilityFollowerMgr::GetFollowerType(ObjectGuid const& creatureGuid) const
{
    std::lock_guard<std::mutex> lock(_lock);
    auto itr = _followerToOwner.find(creatureGuid);
    if (itr != _followerToOwner.end())
        return itr->second.second;

    return FollowerType::Banker;
}

bool UtilityFollowerMgr::IsFollowerActive(Player const* player, FollowerType type) const
{
    std::lock_guard<std::mutex> lock(_lock);
    auto itr = _playerFollowers.find(player->GetObjectGuid());
    if (itr == _playerFollowers.end())
        return false;

    for (auto const& record : itr->second)
        if (record.type == type)
            return true;

    return false;
}

Creature* UtilityFollowerMgr::GetFollower(Player const* player, FollowerType type) const
{
    ObjectGuid creatureGuid;
    {
        std::lock_guard<std::mutex> lock(_lock);
        auto itr = _playerFollowers.find(player->GetObjectGuid());
        if (itr == _playerFollowers.end())
            return nullptr;

        for (auto const& record : itr->second)
        {
            if (record.type == type)
            {
                creatureGuid = record.creatureGuid;
                break;
            }
        }
    }

    if (!creatureGuid)
        return nullptr;

    return player->GetMap()->GetCreature(creatureGuid);
}

uint32 UtilityFollowerMgr::GetActiveFollowerCount(Player const* player) const
{
    std::lock_guard<std::mutex> lock(_lock);
    auto itr = _playerFollowers.find(player->GetObjectGuid());
    if (itr == _playerFollowers.end())
        return 0;

    return static_cast<uint32>(itr->second.size());
}

bool UtilityFollowerMgr::IsMapAllowed(Map const* map) const
{
    if (!map)
        return false;

    if (map->IsBattleGround())
        return sUtilityFollowerConfig->AllowInBattlegrounds;

    if (map->IsRaid())
        return sUtilityFollowerConfig->AllowInRaids;

    if (map->IsDungeon())
        return sUtilityFollowerConfig->AllowInDungeons;

    return sUtilityFollowerConfig->AllowInWorld;
}

bool UtilityFollowerMgr::CanSummon(Player const* player, FollowerType type, std::string& reason) const
{
    if (!player || !IsRealPlayer(player))
    {
        reason = "Only real players may summon utility followers.";
        return false;
    }

    if (!sUtilityFollowerConfig->Enable)
    {
        reason = "Utility followers module is disabled.";
        return false;
    }

    switch (type)
    {
        case FollowerType::Banker:
            if (!sUtilityFollowerConfig->BankerEnable)
            {
                reason = "Banker follower is currently disabled.";
                return false;
            }
            break;
        case FollowerType::Auctioneer:
            if (!sUtilityFollowerConfig->AuctioneerEnable)
            {
                reason = "Auctioneer follower is currently disabled.";
                return false;
            }
            break;
        case FollowerType::Trainer:
            if (!sUtilityFollowerConfig->TrainerEnable)
            {
                reason = "Trainer & Teleporter (Kirin Tor Familiar) follower is currently disabled.";
                return false;
            }
            break;
        default:
            return false;
    }

    if (!player->IsAlive())
    {
        reason = "You cannot summon a follower while dead.";
        return false;
    }

    if (!IsMapAllowed(player->GetMap()))
    {
        reason = "Utility followers are not permitted in this area.";
        return false;
    }

    return true;
}

bool UtilityFollowerMgr::SummonFollower(Player* player, FollowerType type)
{
    if (!player || !IsRealPlayer(player))
        return false;

    std::string reason;
    if (!CanSummon(player, type, reason))
    {
        if (player->GetSession())
            ChatHandler(player->GetSession()).SendSysMessage(reason.c_str());
        return false;
    }

    // Check capacity and policy
    uint32 activeCount = GetActiveFollowerCount(player);
    if (activeCount >= sUtilityFollowerConfig->MaxActive)
    {
        if (sUtilityFollowerConfig->ActivePolicy == MaxActivePolicy::ReplaceOldest)
        {
            FollowerType oldestType = FollowerType::Banker;
            uint32 oldestTime = 0xFFFFFFFF;
            {
                std::lock_guard<std::mutex> lock(_lock);
                auto const& list = _playerFollowers[player->GetObjectGuid()];
                for (auto const& rec : list)
                {
                    if (rec.spawnTime < oldestTime)
                    {
                        oldestTime = rec.spawnTime;
                        oldestType = rec.type;
                    }
                }
            }
            DespawnFollower(player, oldestType);
        }
        else
        {
            ChatHandler(player->GetSession()).SendSysMessage("Cannot summon follower: maximum active follower limit reached.");
            return false;
        }
    }

    // Determine appearance settings
    uint32 creatureEntry = 0;
    uint32 displayId = 0;
    float scale = 1.0f;
    uint32 npcFlags = UNIT_NPC_FLAG_GOSSIP;

    switch (type)
    {
        case FollowerType::Banker:
        {
            bool isAlliance = (player->GetTeam() == ALLIANCE || player->GetTeamId() == TEAM_ALLIANCE);
            creatureEntry = isAlliance ? sUtilityFollowerConfig->BankerCreatureEntryAlliance : sUtilityFollowerConfig->BankerCreatureEntryHorde;
            displayId = isAlliance ? sUtilityFollowerConfig->BankerAllianceDisplayId : sUtilityFollowerConfig->BankerHordeDisplayId;
            scale = sUtilityFollowerConfig->BankerScale;
            npcFlags |= UNIT_NPC_FLAG_BANKER;
            break;
        }
        case FollowerType::Auctioneer:
        {
            creatureEntry = sUtilityFollowerConfig->AuctioneerCreatureEntry;
            displayId = sUtilityFollowerConfig->AuctioneerDisplayId;
            scale = sUtilityFollowerConfig->AuctioneerScale;
            npcFlags |= UNIT_NPC_FLAG_AUCTIONEER;
            break;
        }
        case FollowerType::Trainer:
        {
            creatureEntry = sUtilityFollowerConfig->TrainerCreatureEntry;
            displayId = sUtilityFollowerConfig->TrainerDisplayId;
            scale = sUtilityFollowerConfig->TrainerScale;
            // Kirin Tor Familiar starts with UNIT_NPC_FLAG_GOSSIP. When training is requested,
            // it dynamically adapts to the canonical class/profession trainer template.
            break;
        }
        default:
            return false;
    }

    // Summon temporary runtime creature
    Creature* summon = player->SummonCreature(creatureEntry, player->GetPositionX(), player->GetPositionY(),
                                              player->GetPositionZ(), player->GetOrientation(),
                                              TEMPSUMMON_MANUAL_DESPAWN);
    if (!summon)
    {
        sLog.outError("mod-QOL-followers: Failed to summon utility follower template entry %u for player %s",
                      creatureEntry, player->GetName());
        return false;
    }

    // Configure follower attributes and appearance
    summon->SetFactionTemplateId(player->GetFactionTemplateId());
    summon->SetDisplayId(displayId);
    summon->SetNativeDisplayId(displayId);
    summon->SetFloatValue(OBJECT_FIELD_SCALE_X, scale);

    // Make strictly non-combat utility companion with guaranteed health and flags
    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SPAWNING | UNIT_FLAG_PACIFIED);
    summon->SetUInt32Value(UNIT_NPC_FLAGS, npcFlags);
    summon->SetMaxHealth(100);
    summon->SetHealth(100);

    // Attach custom follower AI
    summon->SetAI(new UtilityFollowerAI(summon, player->GetObjectGuid(), type, displayId, scale));

    // Register active follower
    {
        std::lock_guard<std::mutex> lock(_lock);
        FollowerRecord record;
        record.type = type;
        record.creatureGuid = summon->GetObjectGuid();
        record.spawnTime = WorldTimer::getMSTime();

        _playerFollowers[player->GetObjectGuid()].push_back(record);
        _followerToOwner[summon->GetObjectGuid()] = std::make_pair(player->GetObjectGuid(), type);
    }

    // Initialize orientation towards owner and start following
    summon->SetFacingToObject(player);
    summon->GetMotionMaster()->MoveFollow(player, sUtilityFollowerConfig->FollowDistance, PET_FOLLOW_ANGLE);

    return true;
}

void UtilityFollowerMgr::DespawnFollower(Player* player, FollowerType type)
{
    ObjectGuid creatureGuid;
    {
        std::lock_guard<std::mutex> lock(_lock);
        auto playerItr = _playerFollowers.find(player->GetObjectGuid());
        if (playerItr == _playerFollowers.end())
            return;

        auto& list = playerItr->second;
        for (auto itr = list.begin(); itr != list.end(); ++itr)
        {
            if (itr->type == type)
            {
                creatureGuid = itr->creatureGuid;
                list.erase(itr);
                break;
            }
        }

        if (list.empty())
            _playerFollowers.erase(playerItr);

        if (creatureGuid)
            _followerToOwner.erase(creatureGuid);
    }

    if (creatureGuid)
    {
        if (Creature* creature = player->GetMap()->GetCreature(creatureGuid))
            creature->DespawnOrUnsummon();
    }
}

void UtilityFollowerMgr::DespawnAllFollowers(Player* player)
{
    std::vector<ObjectGuid> toDespawn;
    {
        std::lock_guard<std::mutex> lock(_lock);
        auto playerItr = _playerFollowers.find(player->GetObjectGuid());
        if (playerItr == _playerFollowers.end())
            return;

        for (auto const& record : playerItr->second)
        {
            toDespawn.push_back(record.creatureGuid);
            _followerToOwner.erase(record.creatureGuid);
        }
        _playerFollowers.erase(playerItr);
    }

    for (auto const& guid : toDespawn)
    {
        if (Creature* creature = player->GetMap()->GetCreature(guid))
            creature->DespawnOrUnsummon();
    }
}

void UtilityFollowerMgr::HandleSpellSummon(Player* player, FollowerType type)
{
    // Toggle behavior: present -> dismiss (or restore trainer mode), absent -> summon
    if (IsFollowerActive(player, type))
    {
        // If Kirin Tor Familiar is active in morphed trainer mode, restore to default familiar form
        if (type == FollowerType::Trainer)
        {
            if (Creature* follower = GetFollower(player, type))
            {
                if (follower->GetEntry() != sUtilityFollowerConfig->TrainerCreatureEntry)
                {
                    UtilityFollowerServices::RestoreTrainerFamiliar(follower);
                    ChatHandler(player).SendSysMessage("Kirin Tor Familiar restored to default form.");
                    return;
                }
            }
        }

        DespawnFollower(player, type);
    }
    else
        SummonFollower(player, type);
}

void UtilityFollowerMgr::TeachFollowerSpell(Player* player, uint32 spellId)
{
    if (!player || !spellId || !IsRealPlayer(player) || player->HasSpell(spellId))
        return;

    player->LearnSpell(spellId, false);
}

void UtilityFollowerMgr::SyncSpellsOnLogin(Player* player)
{
    if (!sUtilityFollowerConfig->Enable || !sUtilityFollowerConfig->SpellsSyncOnLogin || !IsRealPlayer(player))
        return;

    uint8 level = player->GetLevel();

    // Banker (Faction-Specific Spell)
    if (sUtilityFollowerConfig->BankerEnable &&
        sUtilityFollowerConfig->BankerSpellEnable &&
        sUtilityFollowerConfig->BankerSpellAutoLearn &&
        level >= sUtilityFollowerConfig->BankerSpellLearnLevel)
    {
        bool isAlliance = (player->GetTeam() == ALLIANCE || player->GetTeamId() == TEAM_ALLIANCE);
        uint32 correctSpell = isAlliance ?
            sUtilityFollowerConfig->BankerSpellIdAlliance :
            sUtilityFollowerConfig->BankerSpellIdHorde;
        uint32 oppositeSpell = isAlliance ?
            sUtilityFollowerConfig->BankerSpellIdHorde :
            sUtilityFollowerConfig->BankerSpellIdAlliance;

        // Clean up wrong-faction spell if character faction shifted or was previously taught opposite spell
        if (player->HasSpell(oppositeSpell))
            player->RemoveSpell(oppositeSpell);

        TeachFollowerSpell(player, correctSpell);
    }

    // Auctioneer
    if (sUtilityFollowerConfig->AuctioneerEnable &&
        sUtilityFollowerConfig->AuctioneerSpellEnable &&
        sUtilityFollowerConfig->AuctioneerSpellAutoLearn &&
        level >= sUtilityFollowerConfig->AuctioneerSpellLearnLevel)
    {
        TeachFollowerSpell(player, sUtilityFollowerConfig->AuctioneerSpellId);
    }

    // Trainer
    if (sUtilityFollowerConfig->TrainerEnable &&
        sUtilityFollowerConfig->TrainerSpellEnable &&
        sUtilityFollowerConfig->TrainerSpellAutoLearn &&
        level >= sUtilityFollowerConfig->TrainerSpellLearnLevel)
    {
        TeachFollowerSpell(player, sUtilityFollowerConfig->TrainerSpellId);
    }

    // Auto-spawn restoration if enabled
    if (sUtilityFollowerConfig->AutoSpawn && IsMapAllowed(player->GetMap()))
    {
        if (sUtilityFollowerConfig->BankerEnable && !IsFollowerActive(player, FollowerType::Banker))
            SummonFollower(player, FollowerType::Banker);
        if (sUtilityFollowerConfig->AuctioneerEnable && !IsFollowerActive(player, FollowerType::Auctioneer))
            SummonFollower(player, FollowerType::Auctioneer);
        if (sUtilityFollowerConfig->TrainerEnable && !IsFollowerActive(player, FollowerType::Trainer))
            SummonFollower(player, FollowerType::Trainer);
    }
}

void UtilityFollowerMgr::CheckSpellsOnLevelChange(Player* player, uint8 oldLevel)
{
    if (!sUtilityFollowerConfig->Enable || !IsRealPlayer(player))
        return;

    uint8 newLevel = player->GetLevel();

    if (newLevel > oldLevel)
    {
        // Level-up: teach newly eligible spells once
        if (sUtilityFollowerConfig->BankerEnable &&
            sUtilityFollowerConfig->BankerSpellEnable &&
            sUtilityFollowerConfig->BankerSpellAutoLearn &&
            newLevel >= sUtilityFollowerConfig->BankerSpellLearnLevel &&
            oldLevel < sUtilityFollowerConfig->BankerSpellLearnLevel)
        {
            bool isAlliance = (player->GetTeam() == ALLIANCE || player->GetTeamId() == TEAM_ALLIANCE);
            uint32 correctSpell = isAlliance ?
                sUtilityFollowerConfig->BankerSpellIdAlliance :
                sUtilityFollowerConfig->BankerSpellIdHorde;
            uint32 oppositeSpell = isAlliance ?
                sUtilityFollowerConfig->BankerSpellIdHorde :
                sUtilityFollowerConfig->BankerSpellIdAlliance;

            if (player->HasSpell(oppositeSpell))
                player->RemoveSpell(oppositeSpell);

            TeachFollowerSpell(player, correctSpell);
        }

        if (sUtilityFollowerConfig->AuctioneerEnable &&
            sUtilityFollowerConfig->AuctioneerSpellEnable &&
            sUtilityFollowerConfig->AuctioneerSpellAutoLearn &&
            newLevel >= sUtilityFollowerConfig->AuctioneerSpellLearnLevel &&
            oldLevel < sUtilityFollowerConfig->AuctioneerSpellLearnLevel)
        {
            TeachFollowerSpell(player, sUtilityFollowerConfig->AuctioneerSpellId);
        }

        if (sUtilityFollowerConfig->TrainerEnable &&
            sUtilityFollowerConfig->TrainerSpellEnable &&
            sUtilityFollowerConfig->TrainerSpellAutoLearn &&
            newLevel >= sUtilityFollowerConfig->TrainerSpellLearnLevel &&
            oldLevel < sUtilityFollowerConfig->TrainerSpellLearnLevel)
        {
            TeachFollowerSpell(player, sUtilityFollowerConfig->TrainerSpellId);
        }
    }
    else if (newLevel < oldLevel && sUtilityFollowerConfig->SpellsRemoveIfBelowLevel)
    {
        // De-level: remove spells if configured
        if (newLevel < sUtilityFollowerConfig->BankerSpellLearnLevel && player->HasSpell(sUtilityFollowerConfig->BankerSpellId))
            player->RemoveSpell(sUtilityFollowerConfig->BankerSpellId);

        if (newLevel < sUtilityFollowerConfig->AuctioneerSpellLearnLevel && player->HasSpell(sUtilityFollowerConfig->AuctioneerSpellId))
            player->RemoveSpell(sUtilityFollowerConfig->AuctioneerSpellId);

        if (newLevel < sUtilityFollowerConfig->TrainerSpellLearnLevel && player->HasSpell(sUtilityFollowerConfig->TrainerSpellId))
            player->RemoveSpell(sUtilityFollowerConfig->TrainerSpellId);
    }
}

void UtilityFollowerMgr::HandleMapChange(Player* player)
{
    std::vector<FollowerType> activeTypes;
    {
        std::lock_guard<std::mutex> lock(_lock);
        auto itr = _playerFollowers.find(player->GetObjectGuid());
        if (itr != _playerFollowers.end())
        {
            for (auto const& rec : itr->second)
            {
                activeTypes.push_back(rec.type);
                _followerToOwner.erase(rec.creatureGuid);
            }
            _playerFollowers.erase(itr);
        }
    }

    // If new map is allowed, restore followers at player's location
    if (IsMapAllowed(player->GetMap()))
    {
        for (FollowerType type : activeTypes)
            SummonFollower(player, type);
    }
}

void UtilityFollowerMgr::UnregisterFollower(ObjectGuid const& creatureGuid)
{
    std::lock_guard<std::mutex> lock(_lock);
    auto ownerItr = _followerToOwner.find(creatureGuid);
    if (ownerItr == _followerToOwner.end())
        return;

    ObjectGuid ownerGuid = ownerItr->second.first;
    _followerToOwner.erase(ownerItr);

    auto playerItr = _playerFollowers.find(ownerGuid);
    if (playerItr != _playerFollowers.end())
    {
        auto& list = playerItr->second;
        for (auto itr = list.begin(); itr != list.end(); ++itr)
        {
            if (itr->creatureGuid == creatureGuid)
            {
                list.erase(itr);
                break;
            }
        }
        if (list.empty())
            _playerFollowers.erase(playerItr);
    }
}

bool UtilityFollowerMgr::IsFollowerSpell(uint32 spellId) const
{
    return spellId == sUtilityFollowerConfig->BankerSpellId ||
           spellId == sUtilityFollowerConfig->AuctioneerSpellId ||
           spellId == sUtilityFollowerConfig->TrainerSpellId;
}

bool UtilityFollowerMgr::IsAutoLearningSpell(ObjectGuid const& playerGuid, uint32 spellId) const
{
    std::lock_guard<std::mutex> lock(_lock);
    auto itr = _autoLearningSpells.find(playerGuid);
    if (itr != _autoLearningSpells.end())
        return itr->second.find(spellId) != itr->second.end();
    return false;
}

void UtilityFollowerMgr::MarkAutoLearningSpell(ObjectGuid const& playerGuid, uint32 spellId)
{
    std::lock_guard<std::mutex> lock(_lock);
    _autoLearningSpells[playerGuid].insert(spellId);
}

void UtilityFollowerMgr::ClearAutoLearningSpell(ObjectGuid const& playerGuid, uint32 spellId)
{
    std::lock_guard<std::mutex> lock(_lock);
    auto itr = _autoLearningSpells.find(playerGuid);
    if (itr != _autoLearningSpells.end())
    {
        itr->second.erase(spellId);
        if (itr->second.empty())
            _autoLearningSpells.erase(itr);
    }
}

bool UtilityFollowerMgr::IsRealPlayer(Player const* player)
{
    if (!player)
        return false;

    WorldSession const* session = player->GetSession();
    if (!session)
        return false;

    return !session->IsHeadless();
}

bool UtilityFollowerMgr::IsPlayerBot(Player const* player)
{
    return !IsRealPlayer(player);
}
