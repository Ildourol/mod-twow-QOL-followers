/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "UtilityFollowerConfig.h"
#include "Config/Config.h"
#include "Log.h"

UtilityFollowerConfig* UtilityFollowerConfig::instance()
{
    static UtilityFollowerConfig instance;
    return &instance;
}

void UtilityFollowerConfig::Load()
{
    sLog.outString("mod-QOL-followers: Loading Utility Followers configuration...");

    Enable = sConfig.GetBoolDefault("UtilityFollowers.Enable", true);

    BankerEnable = sConfig.GetBoolDefault("UtilityFollowers.Banker.Enable", true);
    AuctioneerEnable = sConfig.GetBoolDefault("UtilityFollowers.Auctioneer.Enable", true);
    TrainerEnable = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Enable", true);

    FollowDistance = sConfig.GetFloatDefault("UtilityFollowers.FollowDistance", 3.0f);
    StartFollowingDistance = sConfig.GetFloatDefault("UtilityFollowers.StartFollowingDistance", 4.0f);
    CatchUpDistance = sConfig.GetFloatDefault("UtilityFollowers.CatchUpDistance", 30.0f);
    FollowUpdateInterval = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.FollowUpdateInterval", 300));
    TeleportWhenStuck = sConfig.GetBoolDefault("UtilityFollowers.TeleportWhenStuck", true);

    MaxActive = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.MaxActive", 1));
    std::string policyStr = sConfig.GetStringDefault("UtilityFollowers.MaxActivePolicy", "ReplaceOldest");

    AllowInWorld = sConfig.GetBoolDefault("UtilityFollowers.AllowInWorld", true);
    AllowInDungeons = sConfig.GetBoolDefault("UtilityFollowers.AllowInDungeons", true);
    AllowInRaids = sConfig.GetBoolDefault("UtilityFollowers.AllowInRaids", true);
    AllowInBattlegrounds = sConfig.GetBoolDefault("UtilityFollowers.AllowInBattlegrounds", false);
    AllowInArenas = sConfig.GetBoolDefault("UtilityFollowers.AllowInArenas", false);

    AutoSpawn = sConfig.GetBoolDefault("UtilityFollowers.AutoSpawn", false);

    // Banker Appearance (Alliance: 70100 Pack Mule [14546], Horde: 70103 Pack Kodo [7933])
    BankerCreatureEntryHorde = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Banker.CreatureEntry.Horde", 70103));
    BankerCreatureEntryAlliance = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Banker.CreatureEntry.Alliance", 70100));
    BankerHordeDisplayId = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Banker.Horde.DisplayId", 7933));
    BankerAllianceDisplayId = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Banker.Alliance.DisplayId", 14546));
    BankerScale = sConfig.GetFloatDefault("UtilityFollowers.Banker.Scale", 1.0f);

    // Auctioneer Appearance
    AuctioneerCreatureEntry = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Auctioneer.CreatureEntry", 70101));
    AuctioneerDisplayId = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Auctioneer.DisplayId", 7993));
    AuctioneerScale = sConfig.GetFloatDefault("UtilityFollowers.Auctioneer.Scale", 1.0f);

    // Trainer & Teleporter (Kirin Tor Familiar) Appearance (Display 14432 = Mana Fiend in 1.18.1 DBC)
    TrainerCreatureEntry = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Trainer.CreatureEntry", 70102));
    TrainerDisplayId = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Trainer.DisplayId", 14432));
    TrainerScale = sConfig.GetFloatDefault("UtilityFollowers.Trainer.Scale", 0.5f);

    // Global Spell Defaults
    DefaultSpellLearnLevel = static_cast<uint8>(sConfig.GetIntDefault("UtilityFollowers.Spells.DefaultLearnLevel", 1));

    // Banker Spells (Alliance: 46459 Forworn Mule, Horde: 46483 Caravan Kodo)
    BankerSpellEnable = sConfig.GetBoolDefault("UtilityFollowers.Banker.Spell.Enable", true);
    BankerSpellAutoLearn = sConfig.GetBoolDefault("UtilityFollowers.Banker.Spell.AutoLearn", true);
    BankerSpellLearnLevel = static_cast<uint8>(sConfig.GetIntDefault("UtilityFollowers.Banker.Spell.LearnLevel", DefaultSpellLearnLevel));
    BankerSpellId = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Banker.Spell.SpellId", 46459));
    BankerSpellIdAlliance = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Banker.Spell.SpellId.Alliance", BankerSpellId));
    BankerSpellIdHorde = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Banker.Spell.SpellId.Horde", 46483));

    // Auctioneer Spell (46460 - Mechanical Auctioneer)
    AuctioneerSpellEnable = sConfig.GetBoolDefault("UtilityFollowers.Auctioneer.Spell.Enable", true);
    AuctioneerSpellAutoLearn = sConfig.GetBoolDefault("UtilityFollowers.Auctioneer.Spell.AutoLearn", true);
    AuctioneerSpellLearnLevel = static_cast<uint8>(sConfig.GetIntDefault("UtilityFollowers.Auctioneer.Spell.LearnLevel", DefaultSpellLearnLevel));
    AuctioneerSpellId = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Auctioneer.Spell.SpellId", 46460));

    // Utility Helper / Trainer Spell (46465 - Kirin Tor Familiar)
    TrainerSpellEnable = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Spell.Enable", true);
    TrainerSpellAutoLearn = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Spell.AutoLearn", true);
    TrainerSpellLearnLevel = static_cast<uint8>(sConfig.GetIntDefault("UtilityFollowers.Trainer.Spell.LearnLevel", DefaultSpellLearnLevel));
    TrainerSpellId = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Trainer.Spell.SpellId", 46465));

    // Sync & De-level
    SpellsSyncOnLogin = sConfig.GetBoolDefault("UtilityFollowers.Spells.SyncOnLogin", true);
    SpellsRemoveIfBelowLevel = sConfig.GetBoolDefault("UtilityFollowers.Spells.RemoveIfBelowLevel", false);
    PreventActionBarAutoAdd = sConfig.GetBoolDefault("UtilityFollowers.Spells.PreventActionBarAutoAdd", false);

    // Trainer Services
    TrainerFreeTraining = sConfig.GetBoolDefault("UtilityFollowers.Trainer.FreeTraining", false);
    TrainerIgnoreLevelRequirements = sConfig.GetBoolDefault("UtilityFollowers.Trainer.IgnoreLevelRequirements", false);
    TrainerFreeTalentReset = sConfig.GetBoolDefault("UtilityFollowers.Trainer.FreeTalentReset", false);
    TrainerProfessionMode = sConfig.GetStringDefault("UtilityFollowers.Trainer.ProfessionMode", "Trainer");

    // Medivh Teleportation Services
    TrainerTeleportEnable = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Teleport.Enable", true);
    TrainerTeleportCombatCheck = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Teleport.CombatCheck", true);
    TrainerTeleportCost = static_cast<uint32>(sConfig.GetIntDefault("UtilityFollowers.Trainer.Teleport.Cost", 0));
    TrainerTeleportMinLevel = static_cast<uint8>(sConfig.GetIntDefault("UtilityFollowers.Trainer.Teleport.MinLevel", 1));
    TrainerTeleportEnableExtraLocations = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Teleport.EnableExtraLocations", true);
    TrainerTeleportEnableDungeons = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Teleport.EnableDungeons", true);
    TrainerTeleportEnableRaids = sConfig.GetBoolDefault("UtilityFollowers.Trainer.Teleport.EnableRaids", true);

    // =========================================================================
    // Validation and Safe Fallbacks
    // =========================================================================
    if (FollowDistance <= 0.0f)
    {
        sLog.outError("mod-QOL-followers: Invalid UtilityFollowers.FollowDistance, resetting to default 3.5");
        FollowDistance = 3.5f;
    }

    if (StartFollowingDistance <= FollowDistance)
    {
        sLog.outError("mod-QOL-followers: UtilityFollowers.StartFollowingDistance must be greater than FollowDistance, resetting to 7.0");
        StartFollowingDistance = 7.0f;
    }

    if (CatchUpDistance <= StartFollowingDistance)
    {
        sLog.outError("mod-QOL-followers: UtilityFollowers.CatchUpDistance must be greater than StartFollowingDistance, resetting to 30.0");
        CatchUpDistance = 30.0f;
    }

    if (FollowUpdateInterval < 50)
    {
        sLog.outError("mod-QOL-followers: UtilityFollowers.FollowUpdateInterval too low, setting to minimum safe value 100 ms");
        FollowUpdateInterval = 100;
    }

    if (MaxActive == 0)
    {
        sLog.outError("mod-QOL-followers: UtilityFollowers.MaxActive cannot be 0, resetting to 1");
        MaxActive = 1;
    }

    if (policyStr == "ReplaceOldest")
        ActivePolicy = MaxActivePolicy::ReplaceOldest;
    else if (policyStr == "Reject")
        ActivePolicy = MaxActivePolicy::Reject;
    else
    {
        sLog.outError("mod-QOL-followers: Unknown UtilityFollowers.MaxActivePolicy '%s', falling back to 'Reject'", policyStr.c_str());
        ActivePolicy = MaxActivePolicy::Reject;
    }

    if (BankerScale <= 0.0f)
    {
        sLog.outError("mod-QOL-followers: Invalid UtilityFollowers.Banker.Scale, falling back to 1.0");
        BankerScale = 1.0f;
    }

    if (AuctioneerScale <= 0.0f)
    {
        sLog.outError("mod-QOL-followers: Invalid UtilityFollowers.Auctioneer.Scale, falling back to 1.0");
        AuctioneerScale = 1.0f;
    }

    if (TrainerScale <= 0.0f)
    {
        sLog.outError("mod-QOL-followers: Invalid UtilityFollowers.Trainer.Scale, falling back to 0.5");
        TrainerScale = 0.5f;
    }

    sLog.outString("mod-QOL-followers: Configuration loaded successfully. [Master Enable: %s]", Enable ? "YES" : "NO");
}
