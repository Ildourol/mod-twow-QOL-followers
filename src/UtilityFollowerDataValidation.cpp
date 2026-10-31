/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "UtilityFollowerDataValidation.h"
#include "ScriptObjects.h"
#include "ObjectMgr.h"
#include "Log.h"
#include "UtilityFollowerConfig.h"

class UtilityFollowerWorldScript : public WorldScript
{
public:
    UtilityFollowerWorldScript() : WorldScript("UtilityFollowerWorldScript") { }

    void OnStartup() override
    {
        sUtilityFollowerConfig->Load();

        if (!sUtilityFollowerConfig->Enable)
        {
            sLog.outString("mod-QOL-followers: Module is disabled via configuration.");
            return;
        }

        sLog.outString("mod-QOL-followers: Validating creature templates...");
        uint32 entries[] = {
            sUtilityFollowerConfig->BankerCreatureEntryAlliance,
            sUtilityFollowerConfig->BankerCreatureEntryHorde,
            sUtilityFollowerConfig->AuctioneerCreatureEntry,
            sUtilityFollowerConfig->TrainerCreatureEntry
        };

        for (uint32 entry : entries)
        {
            if (!sObjectMgr.GetCreatureTemplate(entry))
            {
                sLog.outError("mod-QOL-followers: Creature template entry %u not found in database! Ensure SQL migration 20261030000000_world_qol_followers.sql is applied.", entry);
            }
        }

        sLog.outString("mod-QOL-followers: Startup validation complete.");
    }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload)
        {
            sLog.outString("mod-QOL-followers: Reloading module configuration...");
            sUtilityFollowerConfig->Load();
        }
    }
};

void AddUtilityFollowerDataValidationScripts()
{
    new UtilityFollowerWorldScript();
}
