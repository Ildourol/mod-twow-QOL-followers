/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#ifndef UTILITY_FOLLOWER_AI_H
#define UTILITY_FOLLOWER_AI_H

#include "AI/CreatureAI.h"
#include "UtilityFollowerCommon.h"
#include "ObjectGuid.h"

class Creature;
class Unit;

class UtilityFollowerAI : public CreatureAI
{
public:
    explicit UtilityFollowerAI(Creature* creature, ObjectGuid ownerGuid, FollowerType type, uint32 displayId, float scale);

    void Reset();
    void UpdateAI(uint32 const diff) override;

    void MovementInform(uint32 motionType, uint32 pointId) override;

    void AttackStart(Unit*) override {}
    void MoveInLineOfSight(Unit*) override {}
    void AttackedBy(Unit*) override {}
    void EnterEvadeMode() override {}
    void DamageTaken(Unit* /*done_by*/, uint32& damage) override { damage = 0; }
    void JustDied(Unit* /*killer*/) override;

    [[nodiscard]] FollowerType GetType() const { return _type; }
    [[nodiscard]] ObjectGuid GetOwnerGuid() const { return _ownerGuid; }

    void RestoreAppearance();

private:
    ObjectGuid _ownerGuid;
    FollowerType _type;
    uint32 _updateTimer;
    uint32 _displayId;
    float _scale;
};

#endif // UTILITY_FOLLOWER_AI_H
