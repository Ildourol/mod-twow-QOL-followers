/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#ifndef UTILITY_FOLLOWER_CONFIG_H
#define UTILITY_FOLLOWER_CONFIG_H

#include "UtilityFollowerCommon.h"
#include <string>

class UtilityFollowerConfig
{
public:
    static UtilityFollowerConfig* instance();

    void Load();

    // Master switch
    bool Enable{true};

    // Follower enables
    bool BankerEnable{true};
    bool AuctioneerEnable{true};
    bool TrainerEnable{true};

    // Movement (smooth follower tracking: 3.0 yards follow distance)
    float FollowDistance{3.0f};
    float StartFollowingDistance{4.0f};
    float CatchUpDistance{30.0f};
    uint32 FollowUpdateInterval{300};
    bool TeleportWhenStuck{true};

    // Active limit & policy
    uint32 MaxActive{1};
    MaxActivePolicy ActivePolicy{MaxActivePolicy::ReplaceOldest};

    // Environment restrictions
    bool AllowInWorld{true};
    bool AllowInDungeons{true};
    bool AllowInRaids{true};
    bool AllowInBattlegrounds{false};
    bool AllowInArenas{false};

    // Auto spawn
    bool AutoSpawn{false};

    // Banker Appearance
    uint32 BankerCreatureEntryHorde{70103};
    uint32 BankerCreatureEntryAlliance{70100};
    uint32 BankerHordeDisplayId{7933};
    uint32 BankerAllianceDisplayId{14546};
    float BankerScale{1.0f};

    // Auctioneer Appearance
    uint32 AuctioneerCreatureEntry{70101};
    uint32 AuctioneerDisplayId{7993};
    float AuctioneerScale{1.0f};

    // Trainer & Teleporter (Kirin Tor Familiar) Appearance
    uint32 TrainerCreatureEntry{70102};
    uint32 TrainerDisplayId{14432}; // Kirin Tor Familiar (Mana Fiend model) in 1.18.1 DBC
    float TrainerScale{0.5f};

    // Banker Spells (Alliance: 46459 Forworn Mule, Horde: 46483 Caravan Kodo)
    bool BankerSpellEnable{true};
    bool BankerSpellAutoLearn{true};
    uint8 BankerSpellLearnLevel{1};
    uint32 BankerSpellId{46459};
    uint32 BankerSpellIdAlliance{46459};
    uint32 BankerSpellIdHorde{46483};

    // Auctioneer Spell (46460 - Mechanical Auctioneer)
    bool AuctioneerSpellEnable{true};
    bool AuctioneerSpellAutoLearn{true};
    uint8 AuctioneerSpellLearnLevel{1};
    uint32 AuctioneerSpellId{46460};

    // Helper / Trainer Spell (46465 - Kirin Tor Familiar)
    bool TrainerSpellEnable{true};
    bool TrainerSpellAutoLearn{true};
    uint8 TrainerSpellLearnLevel{1};
    uint32 TrainerSpellId{46465};

    // Global default spell learn level
    uint8 DefaultSpellLearnLevel{1};

    // Spell sync & de-level handling
    bool SpellsSyncOnLogin{true};
    bool SpellsRemoveIfBelowLevel{false};
    bool PreventActionBarAutoAdd{false};

    // Trainer policy
    bool TrainerFreeTraining{false};
    bool TrainerIgnoreLevelRequirements{false};
    bool TrainerFreeTalentReset{false};
    std::string TrainerProfessionMode{"Trainer"};

    // Kirin Tor Familiar Teleportation policy
    bool TrainerTeleportEnable{true};
    bool TrainerTeleportCombatCheck{true};
    uint32 TrainerTeleportCost{0};
    uint8 TrainerTeleportMinLevel{1};
    bool TrainerTeleportEnableExtraLocations{true};
    bool TrainerTeleportEnableDungeons{true};
    bool TrainerTeleportEnableRaids{true};

private:
    UtilityFollowerConfig() = default;
};

#define sUtilityFollowerConfig UtilityFollowerConfig::instance()

#endif // UTILITY_FOLLOWER_CONFIG_H
