/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "ScriptObjects.h"
#include "ScriptedGossip.h"
#include "Objects/Player.h"
#include "Objects/Creature.h"
#include "Chat/Chat.h"
#include "UtilityFollowerCommon.h"
#include "UtilityFollowerConfig.h"
#include "UtilityFollowerDestinations.h"
#include "UtilityFollowerMgr.h"
#include "UtilityFollowerServices.h"

#ifndef PlayerTalkClass
#define PlayerTalkClass (player->PlayerTalkClass)
#endif

// ============================================================================
// Banker Follower Script
// ============================================================================
class utility_follower_banker : public CreatureScript
{
public:
    utility_follower_banker() : CreatureScript("utility_follower_banker") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!player || !creature)
            return false;

        ObjectGuid ownerGuid = sUtilityFollowerMgr->GetFollowerOwner(creature->GetObjectGuid());
        if (ownerGuid && ownerGuid != player->GetObjectGuid())
        {
            ChatHandler(player).SendSysMessage("This follower does not belong to you.");
            CLOSE_GOSSIP_MENU();
            return true;
        }

        PlayerTalkClass->ClearMenus();
        ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "I would like to check my deposit box.", GOSSIP_SENDER_MAIN, ACTION_BANKER_OPEN);
        ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Dismiss Banker", GOSSIP_SENDER_MAIN, ACTION_FOLLOWER_DISMISS);

        SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        if (!player || !creature)
            return false;

        ObjectGuid ownerGuid = sUtilityFollowerMgr->GetFollowerOwner(creature->GetObjectGuid());
        if (ownerGuid && ownerGuid != player->GetObjectGuid())
        {
            ChatHandler(player).SendSysMessage("This follower does not belong to you.");
            CLOSE_GOSSIP_MENU();
            return true;
        }

        if (action == ACTION_BANKER_OPEN)
        {
            CLOSE_GOSSIP_MENU();
            UtilityFollowerServices::OpenBank(player, creature);
            return true;
        }

        if (action == ACTION_FOLLOWER_DISMISS)
        {
            CLOSE_GOSSIP_MENU();
            sUtilityFollowerMgr->DespawnFollower(player, FollowerType::Banker);
            return true;
        }

        return false;
    }
};

// ============================================================================
// Auctioneer Follower Script
// ============================================================================
class utility_follower_auctioneer : public CreatureScript
{
public:
    utility_follower_auctioneer() : CreatureScript("utility_follower_auctioneer") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!player || !creature)
            return false;

        ObjectGuid ownerGuid = sUtilityFollowerMgr->GetFollowerOwner(creature->GetObjectGuid());
        if (ownerGuid && ownerGuid != player->GetObjectGuid())
        {
            ChatHandler(player).SendSysMessage("This follower does not belong to you.");
            CLOSE_GOSSIP_MENU();
            return true;
        }

        PlayerTalkClass->ClearMenus();
        ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "I would like to browse the auction house.", GOSSIP_SENDER_MAIN, ACTION_AUCTIONEER_OPEN);
        ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Dismiss Auctioneer", GOSSIP_SENDER_MAIN, ACTION_FOLLOWER_DISMISS);

        SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        if (!player || !creature)
            return false;

        ObjectGuid ownerGuid = sUtilityFollowerMgr->GetFollowerOwner(creature->GetObjectGuid());
        if (ownerGuid && ownerGuid != player->GetObjectGuid())
        {
            ChatHandler(player).SendSysMessage("This follower does not belong to you.");
            CLOSE_GOSSIP_MENU();
            return true;
        }

        if (action == ACTION_AUCTIONEER_OPEN)
        {
            CLOSE_GOSSIP_MENU();
            UtilityFollowerServices::OpenAuctioneer(player, creature);
            return true;
        }

        if (action == ACTION_FOLLOWER_DISMISS)
        {
            CLOSE_GOSSIP_MENU();
            sUtilityFollowerMgr->DespawnFollower(player, FollowerType::Auctioneer);
            return true;
        }

        return false;
    }
};

// ============================================================================
// Trainer / Kirin Tor Familiar Follower Script
// ============================================================================
class utility_follower_trainer : public CreatureScript
{
public:
    utility_follower_trainer() : CreatureScript("utility_follower_trainer") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!player || !creature)
            return false;

        ObjectGuid ownerGuid = sUtilityFollowerMgr->GetFollowerOwner(creature->GetObjectGuid());
        if (ownerGuid && ownerGuid != player->GetObjectGuid())
        {
            ChatHandler(player).SendSysMessage("This follower does not belong to you.");
            CLOSE_GOSSIP_MENU();
            return true;
        }

        UtilityFollowerServices::RestoreTrainerFamiliar(creature);
        UtilityFollowerServices::ShowTrainerFollowerMenu(player, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        if (!player || !creature)
            return false;

        ObjectGuid ownerGuid = sUtilityFollowerMgr->GetFollowerOwner(creature->GetObjectGuid());
        if (ownerGuid && ownerGuid != player->GetObjectGuid())
        {
            ChatHandler(player).SendSysMessage("This follower does not belong to you.");
            CLOSE_GOSSIP_MENU();
            return true;
        }

        // Return to main menu
        if (action == ACTION_FOLLOWER_BACK_MAIN)
        {
            UtilityFollowerServices::RestoreTrainerFamiliar(creature);
            return OnGossipHello(player, creature);
        }

        // Dismiss follower
        if (action == ACTION_FOLLOWER_DISMISS)
        {
            UtilityFollowerServices::RestoreTrainerFamiliar(creature);
            CLOSE_GOSSIP_MENU();
            sUtilityFollowerMgr->DespawnFollower(player, FollowerType::Trainer);
            return true;
        }

        // Class Trainer
        if (action == ACTION_TRAINER_CLASS)
        {
            CLOSE_GOSSIP_MENU();
            UtilityFollowerServices::OpenClassTrainer(player, creature);
            return true;
        }

        // Profession Main Menu
        if (action == ACTION_TRAINER_PROF_MAIN)
        {
            PlayerTalkClass->ClearMenus();
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Primary Professions...", GOSSIP_SENDER_MAIN, ACTION_TRAINER_PROF_PRIMARY_MENU);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Secondary Professions...", GOSSIP_SENDER_MAIN, ACTION_TRAINER_PROF_SECONDARY_MENU);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back to Main Menu]", GOSSIP_SENDER_MAIN, ACTION_FOLLOWER_BACK_MAIN);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Primary Professions Submenu
        if (action == ACTION_TRAINER_PROF_PRIMARY_MENU)
        {
            PlayerTalkClass->ClearMenus();
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Alchemy", GOSSIP_SENDER_MAIN, ACTION_PROF_ALCHEMY);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Blacksmithing", GOSSIP_SENDER_MAIN, ACTION_PROF_BLACKSMITHING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Enchanting", GOSSIP_SENDER_MAIN, ACTION_PROF_ENCHANTING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Engineering", GOSSIP_SENDER_MAIN, ACTION_PROF_ENGINEERING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Herbalism", GOSSIP_SENDER_MAIN, ACTION_PROF_HERBALISM);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Leatherworking", GOSSIP_SENDER_MAIN, ACTION_PROF_LEATHERWORKING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Mining", GOSSIP_SENDER_MAIN, ACTION_PROF_MINING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Skinning", GOSSIP_SENDER_MAIN, ACTION_PROF_SKINNING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Tailoring", GOSSIP_SENDER_MAIN, ACTION_PROF_TAILORING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Jewelcrafting", GOSSIP_SENDER_MAIN, ACTION_PROF_JEWELCRAFTING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back]", GOSSIP_SENDER_MAIN, ACTION_TRAINER_PROF_MAIN);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Secondary Professions Submenu
        if (action == ACTION_TRAINER_PROF_SECONDARY_MENU)
        {
            PlayerTalkClass->ClearMenus();
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Cooking", GOSSIP_SENDER_MAIN, ACTION_PROF_COOKING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "First Aid", GOSSIP_SENDER_MAIN, ACTION_PROF_FIRST_AID);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Fishing", GOSSIP_SENDER_MAIN, ACTION_PROF_FISHING);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back]", GOSSIP_SENDER_MAIN, ACTION_TRAINER_PROF_MAIN);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Handle Profession Selection
        if ((action >= ACTION_PROF_ALCHEMY && action <= ACTION_PROF_TAILORING) ||
            (action >= ACTION_PROF_COOKING && action <= ACTION_PROF_FISHING))
        {
            CLOSE_GOSSIP_MENU();
            UtilityFollowerServices::OpenProfessionTrainer(player, creature, action);
            return true;
        }

        // Talent & Pet Services Menu
        if (action == ACTION_TRAINER_TALENT_SERVICES)
        {
            PlayerTalkClass->ClearMenus();
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Reset my character talents", GOSSIP_SENDER_MAIN, ACTION_TALENT_RESET_CONFIRM);
            if (player->GetClass() == CLASS_HUNTER)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Reset my pet's training points", GOSSIP_SENDER_MAIN, ACTION_TALENT_RESET_PET);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back to Main Menu]", GOSSIP_SENDER_MAIN, ACTION_FOLLOWER_BACK_MAIN);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        if (action == ACTION_TALENT_RESET_CONFIRM)
        {
            PlayerTalkClass->ClearMenus();
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Yes, please reset my talents.", GOSSIP_SENDER_MAIN, ACTION_TALENT_RESET_EXECUTE);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Never mind, take me back.", GOSSIP_SENDER_MAIN, ACTION_TRAINER_TALENT_SERVICES);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        if (action == ACTION_TALENT_RESET_EXECUTE)
        {
            CLOSE_GOSSIP_MENU();
            UtilityFollowerServices::ResetPlayerTalents(player, creature);
            return true;
        }

        if (action == ACTION_TALENT_RESET_PET)
        {
            CLOSE_GOSSIP_MENU();
            UtilityFollowerServices::ResetPetTalents(player, creature);
            return true;
        }

        // Specialization Management Menu
        if (action == ACTION_TRAINER_DUAL_SPEC_MENU)
        {
            PlayerTalkClass->ClearMenus();
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Activate Specialization 1", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_ACTIVATE_1);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Activate Specialization 2", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_ACTIVATE_2);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Activate Specialization 3", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_ACTIVATE_3);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Activate Specialization 4", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_ACTIVATE_4);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Save current build as Spec 1", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_SAVE_1);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Save current build as Spec 2", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_SAVE_2);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Save current build as Spec 3", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_SAVE_3);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Save current build as Spec 4", GOSSIP_SENDER_MAIN, ACTION_DUAL_SPEC_SAVE_4);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back to Main Menu]", GOSSIP_SENDER_MAIN, ACTION_FOLLOWER_BACK_MAIN);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        if (action >= ACTION_DUAL_SPEC_ACTIVATE_1 && action <= ACTION_DUAL_SPEC_ACTIVATE_4)
        {
            CLOSE_GOSSIP_MENU();
            uint8 slot = static_cast<uint8>(action - ACTION_DUAL_SPEC_ACTIVATE_1 + 1);
            UtilityFollowerServices::ActivateDualSpec(player, slot);
            return true;
        }

        if (action >= ACTION_DUAL_SPEC_SAVE_1 && action <= ACTION_DUAL_SPEC_SAVE_4)
        {
            CLOSE_GOSSIP_MENU();
            uint8 slot = static_cast<uint8>(action - ACTION_DUAL_SPEC_SAVE_1 + 1);
            UtilityFollowerServices::SaveDualSpec(player, slot);
            return true;
        }

        // Teleportation Main Menu
        if (action == ACTION_TRAINER_TELEPORT_MENU)
        {
            PlayerTalkClass->ClearMenus();

            // Capitals based on faction
            if (player->GetTeam() == ALLIANCE)
            {
                auto const& caps = UtilityFollowerDestinations::GetAllianceCapitals();
                for (auto const& dest : caps)
                    ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, dest.name, GOSSIP_SENDER_MAIN, dest.action);
            }
            else
            {
                auto const& caps = UtilityFollowerDestinations::GetHordeCapitals();
                for (auto const& dest : caps)
                    ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, dest.name, GOSSIP_SENDER_MAIN, dest.action);
            }

            if (sUtilityFollowerConfig->TrainerTeleportEnableExtraLocations)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Neutral & Quest Hubs...", GOSSIP_SENDER_MAIN, ACTION_TELEPORT_CAT_HUBS);

            if (sUtilityFollowerConfig->TrainerTeleportEnableDungeons)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Classic Dungeons...", GOSSIP_SENDER_MAIN, ACTION_TELEPORT_CAT_DUNGEONS);

            if (sUtilityFollowerConfig->TrainerTeleportEnableRaids)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Classic Raids...", GOSSIP_SENDER_MAIN, ACTION_TELEPORT_CAT_RAIDS);

            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back to Main Menu]", GOSSIP_SENDER_MAIN, ACTION_FOLLOWER_BACK_MAIN);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Neutral & Quest Hubs
        if (action == ACTION_TELEPORT_CAT_HUBS)
        {
            PlayerTalkClass->ClearMenus();
            auto const& hubs = UtilityFollowerDestinations::GetNeutralHubs();
            for (auto const& dest : hubs)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, dest.name, GOSSIP_SENDER_MAIN, dest.action);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back]", GOSSIP_SENDER_MAIN, ACTION_TRAINER_TELEPORT_MENU);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Classic Dungeons Page 1
        if (action == ACTION_TELEPORT_CAT_DUNGEONS || action == ACTION_TELEPORT_DUNGEONS_CLASSIC_P1)
        {
            PlayerTalkClass->ClearMenus();
            auto const& dungeons = UtilityFollowerDestinations::GetClassicDungeonsPage1();
            for (auto const& dest : dungeons)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, dest.name, GOSSIP_SENDER_MAIN, dest.action);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Next Page >>", GOSSIP_SENDER_MAIN, ACTION_TELEPORT_DUNGEONS_CLASSIC_P2);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back]", GOSSIP_SENDER_MAIN, ACTION_TRAINER_TELEPORT_MENU);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Classic Dungeons Page 2
        if (action == ACTION_TELEPORT_DUNGEONS_CLASSIC_P2)
        {
            PlayerTalkClass->ClearMenus();
            auto const& dungeons = UtilityFollowerDestinations::GetClassicDungeonsPage2();
            for (auto const& dest : dungeons)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, dest.name, GOSSIP_SENDER_MAIN, dest.action);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<< Previous Page", GOSSIP_SENDER_MAIN, ACTION_TELEPORT_DUNGEONS_CLASSIC_P1);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back]", GOSSIP_SENDER_MAIN, ACTION_TRAINER_TELEPORT_MENU);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Classic Raids
        if (action == ACTION_TELEPORT_CAT_RAIDS)
        {
            PlayerTalkClass->ClearMenus();
            auto const& raids = UtilityFollowerDestinations::GetClassicRaids();
            for (auto const& dest : raids)
                ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, dest.name, GOSSIP_SENDER_MAIN, dest.action);
            ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[Back]", GOSSIP_SENDER_MAIN, ACTION_TRAINER_TELEPORT_MENU);
            SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
            return true;
        }

        // Direct Teleport Execution
        if (UtilityFollowerDestinations::GetDestination(action))
        {
            CLOSE_GOSSIP_MENU();
            UtilityFollowerServices::TeleportPlayer(player, action);
            return true;
        }

        return false;
    }
};

void AddUtilityFollowerGossipScripts()
{
    new utility_follower_banker();
    new utility_follower_auctioneer();
    new utility_follower_trainer();
}
