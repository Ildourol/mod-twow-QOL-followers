/*
 * Copyright (C) 2026+ Turtle WoW Custom vMaNGOS Project
 * Ported for Turtle WoW 1.18.1 (client build 7272)
 */

#include "ScriptObjects.h"
#include "Chat/Chat.h"
#include "Objects/Player.h"
#include "UtilityFollowerCommon.h"
#include "UtilityFollowerConfig.h"
#include "UtilityFollowerMgr.h"
#include "UtilityFollowerServices.h"
#include "WorldSession.h"
#include <cstring>
#include <string>

class UtilityFollowerCommandScript : public AllCommandScript
{
public:
    UtilityFollowerCommandScript() : AllCommandScript("UtilityFollowerCommandScript") { }

    bool CanExecuteCommand(ChatHandler* handler, char const* cmd, char const* argsIn) override
    {
        if (!cmd || !handler)
            return true;

        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : nullptr;
        if (!player)
            return true;

        if (strcmp(cmd, "utility") == 0 || strcmp(cmd, "follower") == 0)
        {
            std::string rest = argsIn ? argsIn : "";
            auto const takeWord = [&rest]() -> std::string
            {
                size_t const b = rest.find_first_not_of(' ');
                if (b == std::string::npos) { rest.clear(); return std::string(); }
                size_t const e = rest.find(' ', b);
                std::string word = rest.substr(b, e == std::string::npos ? std::string::npos : e - b);
                rest = e == std::string::npos ? std::string() : rest.substr(e + 1);
                return word;
            };

            std::string const sub = takeWord();

            if (sub == "banker" || sub == "bank")
            {
                sUtilityFollowerMgr->HandleSpellSummon(player, FollowerType::Banker);
                return false;
            }
            else if (sub == "auctioneer" || sub == "ah" || sub == "auction")
            {
                sUtilityFollowerMgr->HandleSpellSummon(player, FollowerType::Auctioneer);
                return false;
            }
            else if (sub == "trainer" || sub == "helper" || sub == "familiar" || sub == "medivh" || sub == "teleport")
            {
                sUtilityFollowerMgr->HandleSpellSummon(player, FollowerType::Trainer);
                return false;
            }
            else if (sub == "restore" || sub == "reset")
            {
                if (Creature* follower = sUtilityFollowerMgr->GetFollower(player, FollowerType::Trainer))
                {
                    UtilityFollowerServices::RestoreTrainerFamiliar(follower);
                    handler->SendSysMessage("Kirin Tor Familiar restored to default form.");
                    return false;
                }
                else
                {
                    handler->SendSysMessage("You do not have an active Kirin Tor Familiar.");
                    return false;
                }
            }
            else if (sub == "dismiss")
            {
                std::string const target = takeWord();
                if (target == "banker" || target == "bank")
                    sUtilityFollowerMgr->DespawnFollower(player, FollowerType::Banker);
                else if (target == "auctioneer" || target == "ah")
                    sUtilityFollowerMgr->DespawnFollower(player, FollowerType::Auctioneer);
                else if (target == "trainer" || target == "helper" || target == "familiar" || target == "medivh")
                    sUtilityFollowerMgr->DespawnFollower(player, FollowerType::Trainer);
                else
                    sUtilityFollowerMgr->DespawnAllFollowers(player);

                handler->SendSysMessage("Follower(s) dismissed.");
                return false;
            }
            else if (sub == "reload")
            {
                if (!player->IsGameMaster() && handler->GetSession()->GetSecurity() < SEC_ADMINISTRATOR)
                {
                    handler->SendSysMessage("You do not have permission to reload the module configuration.");
                    return false;
                }

                sUtilityFollowerConfig->Load();
                handler->SendSysMessage("Utility Followers configuration reloaded successfully.");
                return false;
            }
            else
            {
                handler->SendSysMessage("=== Utility Followers Commands ===");
                handler->SendSysMessage("  .utility banker       - Summon or dismiss your personal banker");
                handler->SendSysMessage("  .utility auctioneer   - Summon or dismiss your personal auctioneer");
                handler->SendSysMessage("  .utility familiar     - Summon or dismiss Kirin Tor Familiar (Trainer & Teleporter)");
                handler->SendSysMessage("  .utility restore      - Restore Kirin Tor Familiar from trainer mode to default form");
                handler->SendSysMessage("  .utility dismiss [all|banker|auctioneer|helper|familiar] - Dismiss active followers");
                if (player->IsGameMaster() || handler->GetSession()->GetSecurity() >= SEC_ADMINISTRATOR)
                    handler->SendSysMessage("  .utility reload       - Reload module configuration");
                return false;
            }
        }

        return true;
    }
};

void AddUtilityFollowerCommandScripts()
{
    new UtilityFollowerCommandScript();
}
