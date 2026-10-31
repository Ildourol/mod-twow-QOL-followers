/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "UtilityFollowerDestinations.h"
#include <unordered_map>

namespace UtilityFollowerDestinations
{
    static const std::vector<FollowerTeleportLocation> g_AllianceCapitals = {
        { ACTION_TELEPORT_STORMWIND,  "Stormwind City", 0, -8913.23f,  554.63f,  94.79f, 0.6f },
        { ACTION_TELEPORT_IRONFORGE,  "Ironforge",      0, -4918.88f, -940.40f, 501.55f, 5.4f },
        { ACTION_TELEPORT_DARNASSUS,  "Darnassus",      1,  9951.52f, 2280.32f, 1341.39f, 1.6f }
    };

    static const std::vector<FollowerTeleportLocation> g_HordeCapitals = {
        { ACTION_TELEPORT_ORGRIMMAR,     "Orgrimmar",     1,  1633.33f, -4439.11f,  15.69f, 0.1f },
        { ACTION_TELEPORT_UNDERCITY,     "Undercity",     0,  1586.48f,   239.56f, -52.15f, 0.0f },
        { ACTION_TELEPORT_THUNDER_BLUFF, "Thunder Bluff", 1, -1277.37f,   117.15f, 131.05f, 5.2f }
    };

    static const std::vector<FollowerTeleportLocation> g_NeutralHubs = {
        { ACTION_TELEPORT_BOOTY_BAY, "Booty Bay", 0, -14407.4f,   477.58f,  15.35f, 0.6f },
        { ACTION_TELEPORT_GADGETZAN, "Gadgetzan", 1,  -7169.83f, -3843.43f,   8.45f, 5.8f },
        { ACTION_TELEPORT_RATCHET,   "Ratchet",   1,   -957.37f, -3754.76f,   5.35f, 0.7f },
        { ACTION_TELEPORT_EVERLOOK,  "Everlook",  1,   6720.57f, -4641.87f, 721.25f, 3.8f }
    };

    static const std::vector<FollowerTeleportLocation> g_ClassicDungeonsP1 = {
        { ACTION_TELEPORT_RFC,      "Ragefire Chasm (RFC)",      1,   1809.8f, -4384.6f, -17.9f, 5.6f },
        { ACTION_TELEPORT_VC,       "The Deadmines (VC)",        0, -11208.7f,  1667.6f,  24.7f, 1.4f },
        { ACTION_TELEPORT_WC,       "Wailing Caverns (WC)",      1,   -744.8f, -2215.1f,  16.0f, 2.7f },
        { ACTION_TELEPORT_SFK,      "Shadowfang Keep (SFK)",     0,   -234.3f,  1561.6f,  76.9f, 1.2f },
        { ACTION_TELEPORT_BFD,      "Blackfathom Deeps (BFD)",   1,   4248.8f,   936.8f,  -6.1f, 2.0f },
        { ACTION_TELEPORT_STOCKADE, "The Stockade",              0,  -8798.8f,   831.6f,  97.6f, 0.6f },
        { ACTION_TELEPORT_GNOMER,   "Gnomeregan",                0,  -5163.5f,   925.3f, 257.2f, 1.5f },
        { ACTION_TELEPORT_RFK,      "Razorfen Kraul (RFK)",      1,  -4467.5f, -1669.8f,  82.2f, 0.9f },
        { ACTION_TELEPORT_SM,       "Scarlet Monastery (SM)",    0,   2872.2f,  -818.8f, 160.3f, 0.1f },
        { ACTION_TELEPORT_RFD,      "Razorfen Downs (RFD)",      1,  -4657.4f, -2525.1f,  81.1f, 4.4f }
    };

    static const std::vector<FollowerTeleportLocation> g_ClassicDungeonsP2 = {
        { ACTION_TELEPORT_ULDA,   "Uldaman",                    0,  -6069.9f, -2955.7f,  209.8f, 0.1f },
        { ACTION_TELEPORT_ZF,     "Zul'Farrak (ZF)",            1,  -6801.4f, -2892.4f,    8.9f, 0.2f },
        { ACTION_TELEPORT_MARA,   "Maraudon",                   1,  -1420.2f,  2909.5f,  137.4f, 2.3f },
        { ACTION_TELEPORT_ST,     "Sunken Temple",              0, -10178.0f, -3994.9f, -111.2f, 6.0f },
        { ACTION_TELEPORT_BRD,    "Blackrock Depths (BRD)",     0,  -7178.4f,  -921.2f,  165.8f, 5.0f },
        { ACTION_TELEPORT_LBRS,   "Lower Blackrock Spire (LBRS)",0, -7527.1f, -1226.7f,  285.7f, 5.3f },
        { ACTION_TELEPORT_DM,     "Dire Maul",                  1,  -3982.5f,  1128.2f,  161.0f, 0.1f },
        { ACTION_TELEPORT_SCHOLO, "Scholomance",                0,   1269.6f, -2556.1f,   94.1f, 0.5f },
        { ACTION_TELEPORT_STRAT,  "Stratholme",                 0,   3381.7f, -3364.5f,  142.9f, 1.5f }
    };

    static const std::vector<FollowerTeleportLocation> g_ClassicRaids = {
        { ACTION_TELEPORT_MC,        "Molten Core (MC)",          0,  -7509.7f, -1112.4f, 412.3f, 3.8f },
        { ACTION_TELEPORT_ONYXIA,    "Onyxia's Lair",             1,  -4708.4f, -3727.6f,  54.5f, 3.7f },
        { ACTION_TELEPORT_BWL,       "Blackwing Lair (BWL)",      0,  -7526.4f, -1227.8f, 285.7f, 5.3f },
        { ACTION_TELEPORT_ZG,        "Zul'Gurub (ZG)",            0, -11916.7f, -1216.5f,  92.3f, 4.7f },
        { ACTION_TELEPORT_AQ20,      "Ruins of Ahn'Qiraj (AQ20)", 1,  -8411.3f,  1500.2f,  30.6f, 2.6f },
        { ACTION_TELEPORT_AQ40,      "Temple of Ahn'Qiraj (AQ40)",1,  -8238.6f,  1993.4f, 129.0f, 5.5f },
        { ACTION_TELEPORT_NAXXRAMAS, "Naxxramas",                 0,   3144.3f, -4035.7f, 116.8f, 4.5f }
    };

    FollowerTeleportLocation const* GetDestination(uint32 action)
    {
        for (auto const& loc : g_AllianceCapitals)
            if (loc.action == action)
                return &loc;

        for (auto const& loc : g_HordeCapitals)
            if (loc.action == action)
                return &loc;

        for (auto const& loc : g_NeutralHubs)
            if (loc.action == action)
                return &loc;

        for (auto const& loc : g_ClassicDungeonsP1)
            if (loc.action == action)
                return &loc;

        for (auto const& loc : g_ClassicDungeonsP2)
            if (loc.action == action)
                return &loc;

        for (auto const& loc : g_ClassicRaids)
            if (loc.action == action)
                return &loc;

        return nullptr;
    }

    std::vector<FollowerTeleportLocation> const& GetAllianceCapitals() { return g_AllianceCapitals; }
    std::vector<FollowerTeleportLocation> const& GetHordeCapitals() { return g_HordeCapitals; }
    std::vector<FollowerTeleportLocation> const& GetNeutralHubs() { return g_NeutralHubs; }
    std::vector<FollowerTeleportLocation> const& GetClassicDungeonsPage1() { return g_ClassicDungeonsP1; }
    std::vector<FollowerTeleportLocation> const& GetClassicDungeonsPage2() { return g_ClassicDungeonsP2; }
    std::vector<FollowerTeleportLocation> const& GetClassicRaids() { return g_ClassicRaids; }
}
