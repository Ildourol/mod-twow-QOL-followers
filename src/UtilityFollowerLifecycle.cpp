/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "ScriptObjects.h"
#include "Objects/Player.h"
#include "Objects/Creature.h"
#include "Objects/Pet.h"
#include "Spells/Spell.h"
#include "UtilityFollowerCommon.h"
#include "UtilityFollowerConfig.h"
#include "UtilityFollowerMgr.h"

class UtilityFollowerPlayerScript : public PlayerScript
{
public:
    UtilityFollowerPlayerScript() : PlayerScript("UtilityFollowerPlayerScript") { }

    void OnLogin(Player* player) override
    {
        if (!player)
            return;

        sUtilityFollowerMgr->SyncSpellsOnLogin(player);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (!player)
            return;

        sUtilityFollowerMgr->CheckSpellsOnLevelChange(player, oldLevel);
    }

    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/) override
    {
        if (!player || !spell || !spell->m_spellInfo || !sUtilityFollowerMgr->IsRealPlayer(player))
            return;

        uint32 const spellId = spell->m_spellInfo->Id;

        if (sUtilityFollowerConfig->BankerSpellEnable &&
            (spellId == sUtilityFollowerConfig->BankerSpellId ||
             spellId == sUtilityFollowerConfig->BankerSpellIdAlliance ||
             spellId == sUtilityFollowerConfig->BankerSpellIdHorde))
        {
            sUtilityFollowerMgr->HandleSpellSummon(player, FollowerType::Banker);
        }
        else if (sUtilityFollowerConfig->AuctioneerSpellEnable && spellId == sUtilityFollowerConfig->AuctioneerSpellId)
        {
            sUtilityFollowerMgr->HandleSpellSummon(player, FollowerType::Auctioneer);
        }
        else if (sUtilityFollowerConfig->TrainerSpellEnable && spellId == sUtilityFollowerConfig->TrainerSpellId)
        {
            sUtilityFollowerMgr->HandleSpellSummon(player, FollowerType::Trainer);
        }
    }

    void OnBeforeLogout(Player* player) override
    {
        if (!player)
            return;

        sUtilityFollowerMgr->DespawnAllFollowers(player);
    }

    void OnLogout(Player* player) override
    {
        if (!player)
            return;

        sUtilityFollowerMgr->DespawnAllFollowers(player);
    }

    void OnMapChanged(Player* player) override
    {
        if (!player)
            return;

        sUtilityFollowerMgr->HandleMapChange(player);
    }

    void OnBeforeTeleport(Player* player, uint32 /*mapId*/, float /*x*/, float /*y*/, float /*z*/, float /*orientation*/) override
    {
        if (!player)
            return;

        // Despawn followers before far teleportation to prevent orphan entities
        sUtilityFollowerMgr->DespawnAllFollowers(player);
    }

    void OnPlayerJustDied(Player* player) override
    {
        if (!player)
            return;

        // Followers remain passive while player is dead
    }
};

// Intercept core critter pet additions caused by SPELL_EFFECT_SUMMON_CRITTER
class UtilityFollowerPetScript : public PetScript
{
public:
    UtilityFollowerPetScript() : PetScript("UtilityFollowerPetScript") { }

    void OnPetAddToWorld(Pet* pet) override
    {
        if (!pet)
            return;

        uint32 const spellId = pet->GetUInt32Value(UNIT_CREATED_BY_SPELL);
        if (spellId == sUtilityFollowerConfig->BankerSpellId ||
            spellId == sUtilityFollowerConfig->BankerSpellIdAlliance ||
            spellId == sUtilityFollowerConfig->BankerSpellIdHorde ||
            spellId == sUtilityFollowerConfig->AuctioneerSpellId ||
            spellId == sUtilityFollowerConfig->TrainerSpellId)
        {
            // Immediately conceal and clean up the engine's vanity pet critter
            pet->SetUInt32Value(UNIT_FIELD_DISPLAYID, 0);
            if (Player* owner = pet->GetCharmerOrOwnerPlayer())
                owner->RemoveMiniPet();
            else
                pet->Unsummon(PET_SAVE_AS_DELETED);
        }
    }
};

class UtilityFollowerAllCreatureScript : public AllCreatureScript
{
public:
    UtilityFollowerAllCreatureScript() : AllCreatureScript("UtilityFollowerAllCreatureScript") { }

    void OnCreatureRemoveWorld(Creature* creature) override
    {
        if (!creature)
            return;

        if (sUtilityFollowerMgr->IsFollower(creature->GetObjectGuid()))
            sUtilityFollowerMgr->UnregisterFollower(creature->GetObjectGuid());
    }
};

void AddUtilityFollowerLifecycleScripts()
{
    new UtilityFollowerPlayerScript();
    new UtilityFollowerPetScript();
    new UtilityFollowerAllCreatureScript();
}
