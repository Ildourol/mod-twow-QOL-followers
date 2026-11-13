/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "UtilityFollowerDestinations.h"
#include <unordered_map>

namespace UtilityFollowerDestinations
{
    static const std::vector<FollowerTeleportLocation> g_AllianceCapitals = {
        { ACTION_TELEPORT_STORMWIND,  "Stormwind City",                 0, -8913.23f,   554.63f,   94.79f, 0.60f },
        { ACTION_TELEPORT_IRONFORGE,  "Ironforge",                      0, -4918.88f,  -940.40f,  501.55f, 5.40f },
        { ACTION_TELEPORT_DARNASSUS,  "Darnassus",                      1,  9951.52f,  2280.32f, 1341.39f, 1.60f },
        { ACTION_TELEPORT_ALAHTHALAS, "Alah'Thalas (High Elf Capital)", 0,  4108.56f, -2716.73f,   23.16f, 5.56f }
    };

    static const std::vector<FollowerTeleportLocation> g_HordeCapitals = {
        { ACTION_TELEPORT_ORGRIMMAR,     "Orgrimmar",                   1,  1633.33f, -4439.11f,   15.69f, 0.10f },
        { ACTION_TELEPORT_UNDERCITY,     "Undercity",                   0,  1586.48f,   239.56f,  -52.15f, 0.00f },
        { ACTION_TELEPORT_THUNDER_BLUFF, "Thunder Bluff",               1, -1277.37f,   117.15f,  131.05f, 5.20f },
        { ACTION_TELEPORT_SPARKWATER,    "Sparkwater Port (Goblin)",    1, -1190.00f, -3740.00f,   12.50f, 0.00f }
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

    static const std::vector<FollowerTeleportLocation> g_TurtleDungeons = {
        { ACTION_TELEPORT_FROSTMANE_HOLLOW,   "Frostmane Hollow (13-20)",   0, -5023.99f, -1443.13f, 499.82f, 2.09f },
        { ACTION_TELEPORT_DRAGONMAW_RETREAT,  "Dragonmaw Retreat (25-30)",  0, -3813.14f, -3587.64f, -48.15f, 1.37f },
        { ACTION_TELEPORT_CRESCENT_GROVE,     "The Crescent Grove (32-38)", 1,  1722.00f, -1272.60f, 163.26f, 5.80f },
        { ACTION_TELEPORT_WINDHORN_CANYON,    "Windhorn Canyon (38-44)",    1, -5126.21f, -3532.49f,   4.54f, 4.54f },
        { ACTION_TELEPORT_STORMWROUGHT_RUINS, "Stormwrought Ruins (48-54)", 0, -8821.21f,  2900.45f, 135.49f, 5.26f },
        { ACTION_TELEPORT_HATEFORGE_QUARRY,   "Hateforge Quarry (50-60)",   0, -8169.20f, -3106.70f, 200.40f, 1.10f },
        { ACTION_TELEPORT_KARAZHAN_CRYPT,     "Karazhan Crypt (58-60)",     0,-11068.90f, -1828.60f,  60.26f, 3.10f },
        { ACTION_TELEPORT_STORMWIND_VAULT,    "Stormwind Vault (60)",       0, -8679.12f,   639.34f,  95.82f, 2.29f },
        { ACTION_TELEPORT_GILNEAS_CITY,       "Gilneas City (60)",          0, -1089.86f,  2231.40f, 182.86f, 5.54f },
        { ACTION_TELEPORT_BLACK_MORASS,       "The Black Morass (60)",      1, -8756.80f, -4191.30f,-209.40f, 5.50f },
        { ACTION_TELEPORT_WINTER_VEIL_VALE,   "Winter Veil Vale (Holiday)", 813,-2653.06f, 1117.36f,  54.91f, 1.47f },
        { ACTION_TELEPORT_GNOMEREGAN_REC,     "Gnomeregan Reclamation",     0, -5033.86f,   503.47f, 411.93f, 0.61f }
    };

    static const std::vector<FollowerTeleportLocation> g_TurtleRaids = {
        { ACTION_TELEPORT_LOWER_KARAZHAN,    "Lower Karazhan Halls (10-Man)",  0,-11118.50f, -2010.84f,   47.08f, 3.70f },
        { ACTION_TELEPORT_EMERALD_SANCTUM,   "Emerald Sanctum (40-Man)",       1,  4819.06f, -1746.33f, 1156.99f, 2.50f },
        { ACTION_TELEPORT_TOWER_OF_KARAZHAN, "Tower of Karazhan (40-Man)",     0,-11039.60f, -1997.65f,   94.08f, 2.23f }
    };

    static const std::vector<FollowerTeleportLocation> g_TurtleZones = {
        { ACTION_TELEPORT_TELABIM,       "Tel'Abim (Tel Co. Basecamp)",    1, -8892.64f, -6455.32f,    7.80f, 1.53f },
        { ACTION_TELEPORT_LAPIDIS,       "Lapidis Isle (Caelan's Rest)",   0,-11864.90f,  3188.40f,   20.00f, 0.19f },
        { ACTION_TELEPORT_GILLIJIM,      "Gillijim's Isle (Maul'Ogg)",     0,-14087.20f,  2269.20f,   67.00f, 1.29f },
        { ACTION_TELEPORT_HYJAL,         "Mount Hyjal (Nordanaar)",        1,  5492.38f, -3733.48f, 1594.16f, 2.88f },
        { ACTION_TELEPORT_BALOR,         "Balor Island (Stormbreaker Pt)", 0, -9180.10f,  2573.64f,    9.47f, 0.40f },
        { ACTION_TELEPORT_GILNEAS_WORLD, "Gilneas (Ravenshire)",           0, -2055.72f,  1103.78f,   32.17f, 3.53f }
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

        for (auto const& loc : g_TurtleDungeons)
            if (loc.action == action)
                return &loc;

        for (auto const& loc : g_TurtleRaids)
            if (loc.action == action)
                return &loc;

        for (auto const& loc : g_TurtleZones)
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
    std::vector<FollowerTeleportLocation> const& GetTurtleDungeons() { return g_TurtleDungeons; }
    std::vector<FollowerTeleportLocation> const& GetTurtleRaids() { return g_TurtleRaids; }
    std::vector<FollowerTeleportLocation> const& GetTurtleZones() { return g_TurtleZones; }
}
