/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

void AddUtilityFollowerGossipScripts();
void AddUtilityFollowerLifecycleScripts();
void AddUtilityFollowerCommandScripts();
void AddUtilityFollowerDataValidationScripts();

void Addmod_QOL_followers_wotlkScripts()
{
    AddUtilityFollowerDataValidationScripts();
    AddUtilityFollowerGossipScripts();
    AddUtilityFollowerLifecycleScripts();
    AddUtilityFollowerCommandScripts();
}

void Addmod_qol_followersScripts()
{
    Addmod_QOL_followers_wotlkScripts();
}

void Addmod_twow_QOL_followersScripts()
{
    Addmod_QOL_followers_wotlkScripts();
}
