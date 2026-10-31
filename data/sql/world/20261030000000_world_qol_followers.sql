--
-- mod-QOL-followers: Dedicated Utility Follower Companion Templates
-- Target: Turtle WoW custom vMaNGOS (1.18.1 / 1.12.1)
--

-- Cleanup any previous rows in reserved module range 70100-70135
DELETE FROM `creature_template` WHERE `entry` IN (
    70100, 70101, 70102, 70103, 70104,
    70110, 70111, 70112, 70113, 70114, 70115, 70116, 70117, 70118,
    70120, 70121, 70122, 70123, 70124, 70125, 70126, 70127, 70128, 70129, 70130, 70131, 70132
);

DELETE FROM `npc_trainer` WHERE `entry` IN (
    70100, 70101, 70102, 70103, 70104,
    70110, 70111, 70112, 70113, 70114, 70115, 70116, 70117, 70118,
    70120, 70121, 70122, 70123, 70124, 70125, 70126, 70127, 70128, 70129, 70130, 70131, 70132
);

DELETE FROM `creature` WHERE `id` IN (
    70100, 70101, 70102, 70103, 70104,
    70110, 70111, 70112, 70113, 70114, 70115, 70116, 70117, 70118,
    70120, 70121, 70122, 70123, 70124, 70125, 70126, 70127, 70128, 70129, 70130, 70131, 70132
);

-- =========================================================================
-- BASE FOLLOWER COMPANION CREATURE TEMPLATES
-- =========================================================================
-- 70100: Alliance Banker (Pack Mule, friendly faction 35, display 14546)
-- 70103: Horde Banker (Pack Kodo, friendly faction 35, display 7933)
-- 70101: Personal Auctioneer (Auctioneer Beardo, friendly faction 35, display 7993)
-- 70104: Kirin Tor Familiar (Trainer & Teleporter, friendly faction 35, display 14432, scale 0.5)
-- 70102: Kirin Tor Familiar (Legacy entry alias for backward-compatibility)
-- npc_flags:
--   70100, 70103: UNIT_NPC_FLAG_GOSSIP (1) | UNIT_NPC_FLAG_BANKER (4096) = 4097
--   70101:        UNIT_NPC_FLAG_GOSSIP (1) | UNIT_NPC_FLAG_AUCTIONEER (2097152) = 2097153
--   70104, 70102: UNIT_NPC_FLAG_GOSSIP (1)
-- unit_flags: 131074 (UNIT_FLAG_SPAWNING [2] | UNIT_FLAG_PACIFIED [131072])
-- health_min / health_max: 100 (Ensures IsAlive() evaluates to true for interaction)
INSERT INTO `creature_template` (
    `entry`, `display_id1`, `name`, `subname`, `health_min`, `health_max`,
    `faction`, `npc_flags`, `unit_flags`, `scale`, `inhabit_type`, `script_name`
) VALUES
(70100, 14546, 'Pack Mule', 'Personal Banker', 100, 100, 35, 4097, 131074, 1.0, 3, 'utility_follower_banker'),
(70103, 7933,  'Pack Kodo', 'Personal Banker', 100, 100, 35, 4097, 131074, 1.0, 3, 'utility_follower_banker'),
(70101, 7993,  'Auctioneer Beardo', 'Personal Auctioneer', 100, 100, 35, 2097153, 131074, 1.0, 3, 'utility_follower_auctioneer'),
(70104, 14432, 'Kirin Tor Familiar', 'Trainer & Teleporter', 100, 100, 35, 1, 131074, 0.5, 3, 'utility_follower_trainer'),
(70102, 14432, 'Kirin Tor Familiar', 'Trainer & Teleporter', 100, 100, 35, 1, 131074, 0.5, 3, 'utility_follower_trainer');
