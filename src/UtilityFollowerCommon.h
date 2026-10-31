/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#ifndef UTILITY_FOLLOWER_COMMON_H
#define UTILITY_FOLLOWER_COMMON_H

#include "Common.h"
#include "SharedDefines.h"
#include <cmath>

enum class FollowerType : uint8
{
    Banker     = 0,
    Auctioneer = 1,
    Trainer    = 2,
    Max        = 3
};

enum class MaxActivePolicy : uint8
{
    Reject        = 0,
    ReplaceOldest = 1
};

// Gossip Senders
enum FollowerGossipSender : uint32
{
    SENDER_FOLLOWER_MAIN              = 100,
    SENDER_FOLLOWER_BANKER            = 101,
    SENDER_FOLLOWER_AUCTIONEER        = 102,
    SENDER_FOLLOWER_TRAINER           = 103,
    SENDER_FOLLOWER_PROF_PRIMARY      = 104,
    SENDER_FOLLOWER_PROF_SECOND       = 105,
    SENDER_FOLLOWER_TALENTS           = 106,
    SENDER_FOLLOWER_DUAL_SPEC         = 107,
    SENDER_FOLLOWER_TELEPORT          = 108,
    SENDER_FOLLOWER_TELEPORT_EXTRA    = 109,
    SENDER_FOLLOWER_TELEPORT_DUNGEONS = 110,
    SENDER_FOLLOWER_TELEPORT_RAIDS    = 111,
    SENDER_FOLLOWER_TELEPORT_DEST     = 112
};

// Gossip Actions
enum FollowerGossipAction : uint32
{
    // General
    ACTION_FOLLOWER_DISMISS             = 1,
    ACTION_FOLLOWER_BACK_MAIN           = 2,

    // Banker
    ACTION_BANKER_OPEN                  = 10,

    // Auctioneer
    ACTION_AUCTIONEER_OPEN              = 20,

    // Trainer / Kirin Tor Familiar Menu Navigation
    ACTION_TRAINER_CLASS                = 30,
    ACTION_TRAINER_PROF_MAIN            = 31,
    ACTION_TRAINER_PROF_PRIMARY_MENU    = 32,
    ACTION_TRAINER_PROF_SECONDARY_MENU  = 33,
    ACTION_TRAINER_TALENT_SERVICES      = 34,
    ACTION_TRAINER_DUAL_SPEC_MENU       = 35,
    ACTION_TRAINER_TELEPORT_MENU        = 36,
    ACTION_TRAINER_TELEPORT_EXTRA_MENU  = 37,

    // Talent Actions
    ACTION_TALENT_RESET_CONFIRM         = 40,
    ACTION_TALENT_RESET_EXECUTE         = 41,
    ACTION_TALENT_RESET_PET             = 42,

    // Dual Spec / Saved Specs Actions (Slots 1 to 4)
    ACTION_DUAL_SPEC_ACTIVATE_1         = 51,
    ACTION_DUAL_SPEC_ACTIVATE_2         = 52,
    ACTION_DUAL_SPEC_ACTIVATE_3         = 53,
    ACTION_DUAL_SPEC_ACTIVATE_4         = 54,
    ACTION_DUAL_SPEC_SAVE_1             = 61,
    ACTION_DUAL_SPEC_SAVE_2             = 62,
    ACTION_DUAL_SPEC_SAVE_3             = 63,
    ACTION_DUAL_SPEC_SAVE_4             = 64,

    // Primary Professions (Base offset 100)
    ACTION_PROF_ALCHEMY                 = 101,
    ACTION_PROF_BLACKSMITHING           = 102,
    ACTION_PROF_ENCHANTING              = 103,
    ACTION_PROF_ENGINEERING             = 104,
    ACTION_PROF_HERBALISM               = 105,
    ACTION_PROF_JEWELCRAFTING           = 107,
    ACTION_PROF_LEATHERWORKING          = 108,
    ACTION_PROF_MINING                  = 109,
    ACTION_PROF_SKINNING                = 110,
    ACTION_PROF_TAILORING               = 111,

    // Secondary Professions (Base offset 200)
    ACTION_PROF_COOKING                 = 201,
    ACTION_PROF_FIRST_AID               = 202,
    ACTION_PROF_FISHING                 = 203,

    // Alliance Capitals
    ACTION_TELEPORT_STORMWIND           = 311,
    ACTION_TELEPORT_IRONFORGE           = 312,
    ACTION_TELEPORT_DARNASSUS           = 313,

    // Horde Capitals
    ACTION_TELEPORT_ORGRIMMAR           = 321,
    ACTION_TELEPORT_UNDERCITY           = 322,
    ACTION_TELEPORT_THUNDER_BLUFF       = 323,

    // Category Menus under "Other Destinations"
    ACTION_TELEPORT_CAT_DUNGEONS        = 340,
    ACTION_TELEPORT_CAT_RAIDS           = 341,
    ACTION_TELEPORT_CAT_HUBS            = 342,

    // Dungeon Menus & Pages
    ACTION_TELEPORT_DUNGEONS_CLASSIC_P1 = 350,
    ACTION_TELEPORT_DUNGEONS_CLASSIC_P2 = 351,

    // Raid Menus
    ACTION_TELEPORT_RAIDS_CLASSIC       = 360,

    // World Hubs (Thematic Destinations)
    ACTION_TELEPORT_BOOTY_BAY           = 371,
    ACTION_TELEPORT_GADGETZAN           = 372,
    ACTION_TELEPORT_RATCHET             = 373,
    ACTION_TELEPORT_EVERLOOK            = 374,

    // Vanilla Dungeons (400 - 449)
    ACTION_TELEPORT_RFC                 = 401,
    ACTION_TELEPORT_VC                  = 402,
    ACTION_TELEPORT_WC                  = 403,
    ACTION_TELEPORT_SFK                 = 404,
    ACTION_TELEPORT_BFD                 = 405,
    ACTION_TELEPORT_STOCKADE            = 406,
    ACTION_TELEPORT_GNOMER              = 407,
    ACTION_TELEPORT_RFK                 = 408,
    ACTION_TELEPORT_SM                  = 409,
    ACTION_TELEPORT_RFD                 = 410,
    ACTION_TELEPORT_ULDA                = 411,
    ACTION_TELEPORT_ZF                  = 412,
    ACTION_TELEPORT_MARA                = 413,
    ACTION_TELEPORT_ST                  = 414,
    ACTION_TELEPORT_BRD                 = 415,
    ACTION_TELEPORT_LBRS                = 416,
    ACTION_TELEPORT_UBRS                = 417,
    ACTION_TELEPORT_DM                  = 418,
    ACTION_TELEPORT_SCHOLO              = 419,
    ACTION_TELEPORT_STRAT               = 420,

    // Vanilla Raids (450 - 469)
    ACTION_TELEPORT_MC                  = 451,
    ACTION_TELEPORT_BWL                 = 452,
    ACTION_TELEPORT_AQ20                = 453,
    ACTION_TELEPORT_AQ40                = 454,
    ACTION_TELEPORT_ONYXIA              = 455,
    ACTION_TELEPORT_ZG                  = 456,
    ACTION_TELEPORT_NAXXRAMAS           = 457
};

// Dedicated Follower IDs
enum FollowerDedicatedEntries : uint32
{
    ENTRY_BANKER_MULE                   = 70100, // Alliance Banker (Pack Mule)
    ENTRY_BANKER_KODO                   = 70103, // Horde Banker (Pack Kodo)
    ENTRY_AUCTIONEER_BEARDO             = 70101, // Auctioneer (Auctioneer Beardo)
    ENTRY_UTILITY_HELPER                = 70104, // Kirin Tor Familiar (Trainer & Teleporter)
    ENTRY_KIRIN_TOR_FAMILIAR            = 70104, // Primary identifier
    ENTRY_KIRIN_TOR_FAMILIAR_LEGACY     = 70102  // Legacy entry alias
};

// Verified Canonical Class Trainer Template IDs from tw_world:
enum AllianceClassTrainerTemplate : uint32
{
    TRAINER_ALLIANCE_WARRIOR            = 914,   // Ander Germaine (Stormwind - Warrior Trainer, 104 spells)
    TRAINER_ALLIANCE_PALADIN            = 5492,  // Katherine the Pure (Stormwind - Paladin Trainer, 159 spells)
    TRAINER_ALLIANCE_HUNTER             = 5515,  // Einris Brightspear (Stormwind - Hunter Trainer, 126 spells)
    TRAINER_ALLIANCE_ROGUE              = 918,   // Osborne the Night Man (Stormwind - Rogue Trainer, 114 spells)
    TRAINER_ALLIANCE_PRIEST             = 11401, // Priestess Alathea (Stormwind - Priest Trainer, 156 spells)
    TRAINER_ALLIANCE_SHAMAN             = 3032,  // Beram Skychaser (Thunder Bluff - Shaman Trainer, 176 spells - fallback/crossfaction)
    TRAINER_ALLIANCE_MAGE               = 5497,  // Jennea Cannon (Stormwind - Mage Trainer, 151 spells)
    TRAINER_ALLIANCE_WARLOCK            = 5495,  // Ursula Deline (Stormwind - Warlock Trainer, 144 spells)
    TRAINER_ALLIANCE_DRUID              = 4217   // Mathrengyl Bearwalker (Darnassus - Druid Trainer, 190 spells)
};

enum HordeClassTrainerTemplate : uint32
{
    TRAINER_HORDE_WARRIOR               = 4593,  // Christoph Walker (Undercity - Warrior Trainer, 104 spells)
    TRAINER_HORDE_PALADIN               = 5492,  // Katherine the Pure (Stormwind - Paladin Trainer fallback)
    TRAINER_HORDE_HUNTER                = 3406,  // Xor'juul (Orgrimmar - Hunter Trainer, 126 spells)
    TRAINER_HORDE_ROGUE                 = 3328,  // Ormok (Orgrimmar - Rogue Trainer, 114 spells)
    TRAINER_HORDE_PRIEST                = 6014,  // X'yera (Orgrimmar - Priest Trainer, 159 spells)
    TRAINER_HORDE_SHAMAN                = 3032,  // Beram Skychaser (Thunder Bluff - Shaman Trainer, 176 spells)
    TRAINER_HORDE_MAGE                  = 5882,  // Pephredo (Orgrimmar - Mage Trainer, 151 spells)
    TRAINER_HORDE_WARLOCK               = 3324,  // Grol'dar (Orgrimmar - Warlock Trainer, 144 spells)
    TRAINER_HORDE_DRUID                 = 3033   // Turak Runetotem (Thunder Bluff - Druid Trainer, 190 spells)
};

// Verified Canonical Profession Trainer Template IDs from tw_world:
enum ProfessionTrainerTemplate : uint32
{
    // Primary Professions
    TRAINER_ALCHEMY                     = 5032,  // World Alchemy Trainer (37 spells, all ranks)
    TRAINER_BLACKSMITHING               = 2836,  // Brikk Keencraft (Master Blacksmith, 71 spells)
    TRAINER_ENCHANTING                  = 5038,  // World Enchanting Trainer (66 spells, all ranks)
    TRAINER_ENGINEERING                 = 5037,  // World Engineering Trainer (80 spells, all ranks)
    TRAINER_HERBALISM                   = 812,   // Alma Jainrose (Herbalism Trainer, 4 spells)
    TRAINER_LEATHERWORKING              = 5040,  // World Leatherworking Trainer (72 spells, all ranks)
    TRAINER_MINING                      = 5513,  // Gelman Stonehand (Mining Trainer, 13 spells)
    TRAINER_SKINNING                    = 7088,  // Thuwd (Skinning Trainer, 4 spells)
    TRAINER_TAILORING                   = 5041,  // World Tailoring Trainer (78 spells, all ranks)
    TRAINER_JEWELCRAFTING               = 61913, // Fanzy Sparkspring (Master Jewelcrafter, 81 spells via trainer_id 6)

    // Secondary Professions
    TRAINER_COOKING                     = 1382,  // Mudduk (Superior Cook, 9 spells)
    TRAINER_FIRST_AID                   = 2329,  // Michelle Belle (Physician / First Aid Trainer, 7 spells)
    TRAINER_FISHING                     = 5493   // Arnold Leland (Fishing Trainer, 2 spells)
};

// Teleport Destination Representation
struct FollowerTeleportLocation
{
    uint32 action;
    char const* name;
    uint32 mapId;
    float x, y, z, o;
};

// Point identifier for custom unanchored follower approach
constexpr uint32 POINT_FOLLOWER_APPROACH = 1001;

#endif // UTILITY_FOLLOWER_COMMON_H
