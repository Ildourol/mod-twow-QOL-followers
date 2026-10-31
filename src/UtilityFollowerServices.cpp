/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "UtilityFollowerServices.h"
#include "UtilityFollowerConfig.h"
#include "UtilityFollowerDestinations.h"
#include "Objects/Player.h"
#include "Objects/Creature.h"
#include "Objects/Pet.h"
#include "Chat/Chat.h"
#include "WorldSession.h"
#include "ScriptedGossip.h"

#ifndef PlayerTalkClass
#define PlayerTalkClass (player->PlayerTalkClass)
#endif

namespace UtilityFollowerServices
{
    bool OpenBank(Player* player, Creature* creature)
    {
        if (!player || !creature)
            return false;

        player->GetSession()->SendShowBank(creature->GetObjectGuid());
        return true;
    }

    bool OpenAuctioneer(Player* player, Creature* creature)
    {
        if (!player || !creature)
            return false;

        player->GetSession()->SendAuctionHello(creature);
        return true;
    }

    bool OpenClassTrainer(Player* player, Creature* creature)
    {
        if (!player || !creature)
            return false;

        bool isHorde = (player->GetTeam() == HORDE || player->GetTeamId() == TEAM_HORDE);
        uint32 trainerEntry = 0;

        switch (player->GetClass())
        {
            case CLASS_WARRIOR: trainerEntry = isHorde ? TRAINER_HORDE_WARRIOR : TRAINER_ALLIANCE_WARRIOR; break;
            case CLASS_PALADIN: trainerEntry = TRAINER_ALLIANCE_PALADIN; break;
            case CLASS_HUNTER:  trainerEntry = isHorde ? TRAINER_HORDE_HUNTER  : TRAINER_ALLIANCE_HUNTER;  break;
            case CLASS_ROGUE:   trainerEntry = isHorde ? TRAINER_HORDE_ROGUE   : TRAINER_ALLIANCE_ROGUE;   break;
            case CLASS_PRIEST:  trainerEntry = isHorde ? TRAINER_HORDE_PRIEST  : TRAINER_ALLIANCE_PRIEST;  break;
            case CLASS_SHAMAN:  trainerEntry = TRAINER_HORDE_SHAMAN; break;
            case CLASS_MAGE:    trainerEntry = isHorde ? TRAINER_HORDE_MAGE    : TRAINER_ALLIANCE_MAGE;    break;
            case CLASS_WARLOCK: trainerEntry = isHorde ? TRAINER_HORDE_WARLOCK : TRAINER_ALLIANCE_WARLOCK; break;
            case CLASS_DRUID:   trainerEntry = isHorde ? TRAINER_HORDE_DRUID   : TRAINER_ALLIANCE_DRUID;   break;
            default:
                ChatHandler(player).SendSysMessage("No class trainer available for your class.");
                return false;
        }

        if (trainerEntry)
        {
            creature->UpdateEntry(trainerEntry);
            creature->SetFactionTemplateId(35);
            creature->SetDisplayId(sUtilityFollowerConfig->TrainerDisplayId);
            creature->SetNativeDisplayId(sUtilityFollowerConfig->TrainerDisplayId);
            creature->SetFloatValue(OBJECT_FIELD_SCALE_X, sUtilityFollowerConfig->TrainerScale);
            creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER);

            CreatureInfo const* ci = creature->GetCreatureInfo();
            char const* name = ci ? ci->name.c_str() : "Class Trainer";
            ChatHandler(player).PSendSysMessage("Kirin Tor Familiar attuned as %s. Right-click to train, or cast Kirin Tor Familiar to restore.", name);
            return true;
        }

        return false;
    }

    bool OpenProfessionTrainer(Player* player, Creature* creature, uint32 profAction)
    {
        if (!player || !creature)
            return false;

        uint32 profEntry = 0;
        switch (profAction)
        {
            case ACTION_PROF_ALCHEMY:        profEntry = TRAINER_ALCHEMY; break;
            case ACTION_PROF_BLACKSMITHING:  profEntry = TRAINER_BLACKSMITHING; break;
            case ACTION_PROF_ENCHANTING:     profEntry = TRAINER_ENCHANTING; break;
            case ACTION_PROF_ENGINEERING:    profEntry = TRAINER_ENGINEERING; break;
            case ACTION_PROF_HERBALISM:      profEntry = TRAINER_HERBALISM; break;
            case ACTION_PROF_LEATHERWORKING: profEntry = TRAINER_LEATHERWORKING; break;
            case ACTION_PROF_MINING:         profEntry = TRAINER_MINING; break;
            case ACTION_PROF_SKINNING:       profEntry = TRAINER_SKINNING; break;
            case ACTION_PROF_TAILORING:      profEntry = TRAINER_TAILORING; break;
            case ACTION_PROF_JEWELCRAFTING:  profEntry = TRAINER_JEWELCRAFTING; break;
            case ACTION_PROF_COOKING:        profEntry = TRAINER_COOKING; break;
            case ACTION_PROF_FIRST_AID:      profEntry = TRAINER_FIRST_AID; break;
            case ACTION_PROF_FISHING:        profEntry = TRAINER_FISHING; break;
            default:
                return false;
        }

        if (profEntry)
        {
            creature->UpdateEntry(profEntry);
            creature->SetFactionTemplateId(35);
            creature->SetDisplayId(sUtilityFollowerConfig->TrainerDisplayId);
            creature->SetNativeDisplayId(sUtilityFollowerConfig->TrainerDisplayId);
            creature->SetFloatValue(OBJECT_FIELD_SCALE_X, sUtilityFollowerConfig->TrainerScale);
            creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER);

            CreatureInfo const* ci = creature->GetCreatureInfo();
            char const* name = ci ? ci->name.c_str() : "Profession Trainer";
            ChatHandler(player).PSendSysMessage("Kirin Tor Familiar attuned as %s. Right-click to train, or cast Kirin Tor Familiar to restore.", name);
            return true;
        }

        return false;
    }

    bool RestoreTrainerFamiliar(Creature* creature)
    {
        if (!creature)
            return false;

        if (creature->GetEntry() != sUtilityFollowerConfig->TrainerCreatureEntry)
        {
            creature->UpdateEntry(sUtilityFollowerConfig->TrainerCreatureEntry);
            creature->SetFactionTemplateId(35);
            creature->SetDisplayId(sUtilityFollowerConfig->TrainerDisplayId);
            creature->SetNativeDisplayId(sUtilityFollowerConfig->TrainerDisplayId);
            creature->SetFloatValue(OBJECT_FIELD_SCALE_X, sUtilityFollowerConfig->TrainerScale);
            creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            return true;
        }

        return false;
    }

    bool ShowTrainerFollowerMenu(Player* player, Creature* creature)
    {
        if (!player || !creature)
            return false;

        RestoreTrainerFamiliar(creature);

        PlayerTalkClass->ClearMenus();

        ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Class Training", GOSSIP_SENDER_MAIN, ACTION_TRAINER_CLASS);
        ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Profession Training...", GOSSIP_SENDER_MAIN, ACTION_TRAINER_PROF_MAIN);
        ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Talent & Pet Services...", GOSSIP_SENDER_MAIN, ACTION_TRAINER_TALENT_SERVICES);
        ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Specialization Management...", GOSSIP_SENDER_MAIN, ACTION_TRAINER_DUAL_SPEC_MENU);

        if (sUtilityFollowerConfig->TrainerTeleportEnable)
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Destination...", GOSSIP_SENDER_MAIN, ACTION_TRAINER_TELEPORT_MENU);

        ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Dismiss Kirin Tor Familiar", GOSSIP_SENDER_MAIN, ACTION_FOLLOWER_DISMISS);

        SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
        return true;
    }

    bool ResetPlayerTalents(Player* player, Creature* creature)
    {
        if (!player)
            return false;

        (void)creature;
        bool freeReset = sUtilityFollowerConfig->TrainerFreeTalentReset;
        if (player->ResetTalents(freeReset))
        {
            ChatHandler(player).SendSysMessage("Your talents have been reset.");
            return true;
        }

        ChatHandler(player).SendSysMessage("Unable to reset talents. Ensure you have sufficient funds.");
        return false;
    }

    bool ResetPetTalents(Player* player, Creature* creature)
    {
        if (!player)
            return false;

        (void)creature;
        Pet* pet = player->GetPet();
        if (!pet || pet->getPetType() != HUNTER_PET)
        {
            ChatHandler(player).SendSysMessage("You do not have an active hunter pet.");
            return false;
        }

        uint32 cost = pet->GetResetTalentsCost();
        if (!sUtilityFollowerConfig->TrainerFreeTalentReset)
        {
            if (player->GetMoney() < cost)
            {
                player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                return false;
            }
            player->ModifyMoney(-int32(cost));
        }

        for (PetSpellMap::iterator itr = pet->m_petSpells.begin(); itr != pet->m_petSpells.end();)
        {
            uint32 spell_id = itr->first;
            ++itr;
            pet->unlearnSpell(spell_id, false);
        }

        pet->SetTP(pet->GetLevel() * (pet->GetLoyaltyLevel() - 1));

        CharmInfo* charmInfo = pet->GetCharmInfo();
        if (charmInfo)
        {
            for (int i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
            {
                if (UnitActionBarEntry const* ab = charmInfo->GetActionBarEntry(i))
                {
                    if (ab->GetAction() && ab->IsActionBarForSpell())
                        charmInfo->SetActionBar(i, 0, ACT_DISABLED);
                }
            }
        }

        pet->LearnPetPassives();
        pet->CastPetAuras(true);
        ChatHandler(player).SendSysMessage("Your pet's training points have been reset.");
        return true;
    }

    bool ActivateDualSpec(Player* player, uint8 slot)
    {
        if (!player)
            return false;

        // Defensive guard against out-of-bounds slot index
        if (slot < 1 || slot > 4)
        {
            ChatHandler(player).SendSysMessage("Invalid specialization slot (must be 1-4).");
            return false;
        }

        if (player->IsInCombat())
        {
            ChatHandler(player).SendSysMessage("You cannot activate a specialization while in combat.");
            return false;
        }

        return player->ActivateTalentSpec(slot);
    }

    bool SaveDualSpec(Player* player, uint8 slot)
    {
        if (!player)
            return false;

        // Defensive guard against out-of-bounds slot index
        if (slot < 1 || slot > 4)
        {
            ChatHandler(player).SendSysMessage("Invalid specialization slot (must be 1-4).");
            return false;
        }

        return player->SaveTalentSpec(slot);
    }

    bool TeleportPlayer(Player* player, uint32 action)
    {
        if (!player)
            return false;

        FollowerTeleportLocation const* dest = UtilityFollowerDestinations::GetDestination(action);
        if (!dest)
        {
            ChatHandler(player).SendSysMessage("Unknown destination.");
            return false;
        }

        if (sUtilityFollowerConfig->TrainerTeleportCombatCheck && player->IsInCombat())
        {
            ChatHandler(player).SendSysMessage("You cannot teleport while in combat.");
            return false;
        }

        if (player->GetLevel() < sUtilityFollowerConfig->TrainerTeleportMinLevel)
        {
            ChatHandler(player).PSendSysMessage("You must be at least level %u to use this teleport.",
                                                sUtilityFollowerConfig->TrainerTeleportMinLevel);
            return false;
        }

        if (sUtilityFollowerConfig->TrainerTeleportCost > 0)
        {
            if (player->GetMoney() < sUtilityFollowerConfig->TrainerTeleportCost)
            {
                ChatHandler(player).SendSysMessage("You do not have enough money to teleport.");
                return false;
            }
            player->ModifyMoney(-int32(sUtilityFollowerConfig->TrainerTeleportCost));
        }

        return player->TeleportTo(dest->mapId, dest->x, dest->y, dest->z, dest->o);
    }
}
