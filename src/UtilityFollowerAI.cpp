/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "UtilityFollowerAI.h"
#include "Objects/Creature.h"
#include "Objects/Player.h"
#include "Movement/MotionMaster.h"
#include "ObjectAccessor.h"
#include "Maps/Map.h"
#include "UtilityFollowerConfig.h"
#include "UtilityFollowerMgr.h"
#include <cmath>

#ifndef PET_FOLLOW_ANGLE
#define PET_FOLLOW_ANGLE (M_PI_F / 2.0f)
#endif

UtilityFollowerAI::UtilityFollowerAI(Creature* creature, ObjectGuid ownerGuid, FollowerType type, uint32 displayId, float scale)
    : CreatureAI(creature),
      _ownerGuid(ownerGuid),
      _type(type),
      _updateTimer(sUtilityFollowerConfig->FollowUpdateInterval),
      _displayId(displayId),
      _scale(scale)
{
    m_bMeleeAttack = false;
    m_bCombatMovement = false;
    m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
    m_creature->SetSpeedRate(MOVE_WALK, 1.0f);
}

void UtilityFollowerAI::Reset()
{
    m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
    m_creature->SetSpeedRate(MOVE_WALK, 1.0f);

    if (Player* owner = ObjectAccessor::FindPlayer(_ownerGuid))
    {
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveFollow(owner, sUtilityFollowerConfig->FollowDistance, PET_FOLLOW_ANGLE);
    }
}

void UtilityFollowerAI::UpdateAI(uint32 const diff)
{
    if (_updateTimer <= diff)
    {
        _updateTimer = sUtilityFollowerConfig->FollowUpdateInterval;

        Player* owner = ObjectAccessor::FindPlayer(_ownerGuid);
        if (!owner || !owner->IsInWorld())
        {
            sUtilityFollowerMgr->UnregisterFollower(m_creature->GetObjectGuid());
            m_creature->DespawnOrUnsummon();
            return;
        }

        // Map transitions are handled by PlayerScript::OnMapChanged
        if (owner->GetMap() != m_creature->GetMap())
            return;

        // If player is dead, pause following until resurrection
        if (!owner->IsAlive())
        {
            if (m_creature->HasUnitState(UNIT_STAT_FOLLOW))
            {
                m_creature->GetMotionMaster()->Clear();
                m_creature->StopMoving();
            }
            return;
        }

        // If follower is interacting with player (gossip/bank/trainer/auction), keep position
        if (m_creature->HasUnitState(UNIT_STAT_DISTRACTED))
            return;

        float dist = m_creature->GetDistance(owner);

        // 1. Stuck / catch-up recovery: if too far away, safely teleport to owner
        if (dist > sUtilityFollowerConfig->CatchUpDistance)
        {
            if (sUtilityFollowerConfig->TeleportWhenStuck)
            {
                m_creature->NearTeleportTo(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation());
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveFollow(owner, sUtilityFollowerConfig->FollowDistance, PET_FOLLOW_ANGLE);
                m_creature->SetFacingToObject(owner);
                m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
                return;
            }
        }

        // 2. Continuous following state maintenance
        if (!m_creature->HasUnitState(UNIT_STAT_FOLLOW) && dist > sUtilityFollowerConfig->FollowDistance)
        {
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveFollow(owner, sUtilityFollowerConfig->FollowDistance, PET_FOLLOW_ANGLE);
            m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
        }
    }
    else
        _updateTimer -= diff;
}

void UtilityFollowerAI::MovementInform(uint32 /*motionType*/, uint32 /*pointId*/)
{
}

void UtilityFollowerAI::JustDied(Unit* /*killer*/)
{
    sUtilityFollowerMgr->UnregisterFollower(m_creature->GetObjectGuid());
}

void UtilityFollowerAI::RestoreAppearance()
{
    m_creature->SetDisplayId(_displayId);
    m_creature->SetNativeDisplayId(_displayId);
    m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, _scale);

    if (Player* owner = ObjectAccessor::FindPlayer(_ownerGuid))
        m_creature->SetFactionTemplateId(owner->GetFactionTemplateId());

    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SPAWNING | UNIT_FLAG_PACIFIED);

    uint32 npcFlags = UNIT_NPC_FLAG_GOSSIP;
    if (_type == FollowerType::Banker)
        npcFlags |= UNIT_NPC_FLAG_BANKER;
    else if (_type == FollowerType::Auctioneer)
        npcFlags |= UNIT_NPC_FLAG_AUCTIONEER;

    // Notice: Trainer follower intentionally uses ONLY UNIT_NPC_FLAG_GOSSIP so that
    // right-clicking opens the service gossip menu rather than bypassing it via CMSG_TRAINER_LIST.
    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, npcFlags);

    m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
    m_creature->SetSpeedRate(MOVE_WALK, 1.0f);
}
