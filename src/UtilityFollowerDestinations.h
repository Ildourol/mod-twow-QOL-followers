/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#ifndef UTILITY_FOLLOWER_DESTINATIONS_H
#define UTILITY_FOLLOWER_DESTINATIONS_H

#include "UtilityFollowerCommon.h"
#include <vector>

namespace UtilityFollowerDestinations
{
    FollowerTeleportLocation const* GetDestination(uint32 action);

    std::vector<FollowerTeleportLocation> const& GetAllianceCapitals();
    std::vector<FollowerTeleportLocation> const& GetHordeCapitals();
    std::vector<FollowerTeleportLocation> const& GetNeutralHubs();
    std::vector<FollowerTeleportLocation> const& GetClassicDungeonsPage1();
    std::vector<FollowerTeleportLocation> const& GetClassicDungeonsPage2();
    std::vector<FollowerTeleportLocation> const& GetClassicRaids();
}

#endif // UTILITY_FOLLOWER_DESTINATIONS_H
