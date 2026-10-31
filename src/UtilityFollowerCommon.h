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

    // Trainer / Medivh Menu Navigation
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

// Dedicated Follower & Proxy IDs created in 20261030000000_world_qol_followers.sql
enum FollowerDedicatedEntries : uint32
{
    ENTRY_BANKER_MULE                   = 70100,
    ENTRY_BANKER_KODO                   = 70103,
    ENTRY_AUCTIONEER_BEARDO             = 70101,
    ENTRY_UTILITY_HELPER                = 70102,
    ENTRY_KIRIN_TOR_FAMILIAR           = 70102,
    ENTRY_MEDIVH_GUARDIAN               = 70102, // Backward-compatible alias

    // Dedicated Class Trainer Proxies (70110 - 70118)
    ENTRY_PROXY_TRAINER_WARRIOR         = 70110,
    ENTRY_PROXY_TRAINER_PALADIN         = 70111,
    ENTRY_PROXY_TRAINER_HUNTER          = 70112,
    ENTRY_PROXY_TRAINER_ROGUE           = 70113,
    ENTRY_PROXY_TRAINER_PRIEST          = 70114,
    ENTRY_PROXY_TRAINER_SHAMAN          = 70115,
    ENTRY_PROXY_TRAINER_MAGE            = 70116,
    ENTRY_PROXY_TRAINER_WARLOCK         = 70117,
    ENTRY_PROXY_TRAINER_DRUID           = 70118,

    // Dedicated Profession Trainer Proxies (70120 - 70132)
    ENTRY_PROXY_TRAINER_ALCHEMY         = 70120,
    ENTRY_PROXY_TRAINER_BLACKSMITHING   = 70121,
    ENTRY_PROXY_TRAINER_ENCHANTING      = 70122,
    ENTRY_PROXY_TRAINER_ENGINEERING     = 70123,
    ENTRY_PROXY_TRAINER_HERBALISM       = 70124,
    ENTRY_PROXY_TRAINER_LEATHERWORKING  = 70125,
    ENTRY_PROXY_TRAINER_MINING          = 70126,
    ENTRY_PROXY_TRAINER_SKINNING        = 70127,
    ENTRY_PROXY_TRAINER_TAILORING       = 70128,
    ENTRY_PROXY_TRAINER_JEWELCRAFTING   = 70129,
    ENTRY_PROXY_TRAINER_COOKING         = 70130,
    ENTRY_PROXY_TRAINER_FIRST_AID       = 70131,
    ENTRY_PROXY_TRAINER_FISHING         = 70132
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
