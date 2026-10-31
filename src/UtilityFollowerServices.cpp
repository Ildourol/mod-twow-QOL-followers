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

        uint8 pClass = player->GetClass();
        uint32 proxyEntry = 0;
        switch (pClass)
        {
            case CLASS_WARRIOR: proxyEntry = ENTRY_PROXY_TRAINER_WARRIOR; break;
            case CLASS_PALADIN: proxyEntry = ENTRY_PROXY_TRAINER_PALADIN; break;
            case CLASS_HUNTER:  proxyEntry = ENTRY_PROXY_TRAINER_HUNTER; break;
            case CLASS_ROGUE:   proxyEntry = ENTRY_PROXY_TRAINER_ROGUE; break;
            case CLASS_PRIEST:  proxyEntry = ENTRY_PROXY_TRAINER_PRIEST; break;
            case CLASS_SHAMAN:  proxyEntry = ENTRY_PROXY_TRAINER_SHAMAN; break;
            case CLASS_MAGE:    proxyEntry = ENTRY_PROXY_TRAINER_MAGE; break;
            case CLASS_WARLOCK: proxyEntry = ENTRY_PROXY_TRAINER_WARLOCK; break;
            case CLASS_DRUID:   proxyEntry = ENTRY_PROXY_TRAINER_DRUID; break;
            default:
                ChatHandler(player).SendSysMessage("No class trainer available for your class.");
                return false;
        }

        Creature* proxy = player->SummonCreature(proxyEntry, player->GetPositionX(), player->GetPositionY(),
                                                 player->GetPositionZ(), player->GetOrientation(),
                                                 TEMPSUMMON_TIMED_DESPAWN, 120000);
        if (!proxy)
            return false;

        proxy->SetFactionTemplateId(player->GetFactionTemplateId());
        proxy->SetMaxHealth(100);
        proxy->SetHealth(100);
        player->GetSession()->SendTrainerList(proxy->GetObjectGuid());
        return true;
    }

    bool OpenProfessionTrainer(Player* player, Creature* creature, uint32 profAction)
    {
        if (!player || !creature)
            return false;

        uint32 proxyEntry = 0;
        switch (profAction)
        {
            case ACTION_PROF_ALCHEMY:        proxyEntry = ENTRY_PROXY_TRAINER_ALCHEMY; break;
            case ACTION_PROF_BLACKSMITHING:  proxyEntry = ENTRY_PROXY_TRAINER_BLACKSMITHING; break;
            case ACTION_PROF_ENCHANTING:     proxyEntry = ENTRY_PROXY_TRAINER_ENCHANTING; break;
            case ACTION_PROF_ENGINEERING:    proxyEntry = ENTRY_PROXY_TRAINER_ENGINEERING; break;
            case ACTION_PROF_HERBALISM:      proxyEntry = ENTRY_PROXY_TRAINER_HERBALISM; break;
            case ACTION_PROF_LEATHERWORKING: proxyEntry = ENTRY_PROXY_TRAINER_LEATHERWORKING; break;
            case ACTION_PROF_MINING:         proxyEntry = ENTRY_PROXY_TRAINER_MINING; break;
            case ACTION_PROF_SKINNING:       proxyEntry = ENTRY_PROXY_TRAINER_SKINNING; break;
            case ACTION_PROF_TAILORING:      proxyEntry = ENTRY_PROXY_TRAINER_TAILORING; break;
            case ACTION_PROF_JEWELCRAFTING:  proxyEntry = ENTRY_PROXY_TRAINER_JEWELCRAFTING; break;
            case ACTION_PROF_COOKING:        proxyEntry = ENTRY_PROXY_TRAINER_COOKING; break;
            case ACTION_PROF_FIRST_AID:      proxyEntry = ENTRY_PROXY_TRAINER_FIRST_AID; break;
            case ACTION_PROF_FISHING:        proxyEntry = ENTRY_PROXY_TRAINER_FISHING; break;
            default:
                return false;
        }

        Creature* proxy = player->SummonCreature(proxyEntry, player->GetPositionX(), player->GetPositionY(),
                                                 player->GetPositionZ(), player->GetOrientation(),
                                                 TEMPSUMMON_TIMED_DESPAWN, 120000);
        if (!proxy)
            return false;

        proxy->SetFactionTemplateId(player->GetFactionTemplateId());
        proxy->SetMaxHealth(100);
        proxy->SetHealth(100);
        player->GetSession()->SendTrainerList(proxy->GetObjectGuid());
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
