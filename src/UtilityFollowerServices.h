/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#ifndef UTILITY_FOLLOWER_SERVICES_H
#define UTILITY_FOLLOWER_SERVICES_H

#include "UtilityFollowerCommon.h"

class Player;
class Creature;

namespace UtilityFollowerServices
{
    bool OpenBank(Player* player, Creature* creature);
    bool OpenAuctioneer(Player* player, Creature* creature);
    bool OpenClassTrainer(Player* player, Creature* creature);
    bool OpenProfessionTrainer(Player* player, Creature* creature, uint32 profAction);
    bool ResetPlayerTalents(Player* player, Creature* creature);
    bool ResetPetTalents(Player* player, Creature* creature);
    bool ActivateDualSpec(Player* player, uint8 slot);
    bool SaveDualSpec(Player* player, uint8 slot);
    bool TeleportPlayer(Player* player, uint32 action);
}

#endif // UTILITY_FOLLOWER_SERVICES_H
