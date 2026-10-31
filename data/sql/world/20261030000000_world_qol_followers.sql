--
-- mod-QOL-followers: Dedicated Follower Templates & Trainer Proxies
-- Target: Turtle WoW custom vMaNGOS (1.18.1 / 1.12.1)
--

-- Cleanup any previous rows in reserved module range 70100-70135
DELETE FROM `creature_template` WHERE `entry` IN (
    70100, 70101, 70102, 70103,
    70110, 70111, 70112, 70113, 70114, 70115, 70116, 70117, 70118,
    70120, 70121, 70122, 70123, 70124, 70125, 70126, 70127, 70128, 70129, 70130, 70131, 70132
);

DELETE FROM `npc_trainer` WHERE `entry` IN (
    70110, 70111, 70112, 70113, 70114, 70115, 70116, 70117, 70118,
    70120, 70121, 70122, 70123, 70124, 70125, 70126, 70127, 70128, 70129, 70130, 70131, 70132
);

-- =========================================================================
-- 1. BASE FOLLOWER COMPANION CREATURE TEMPLATES
-- =========================================================================
-- 70100: Alliance Banker (Pack Mule, friendly faction 35)
-- 70103: Horde Banker (Pack Kodo, friendly faction 35)
-- 70101: Personal Auctioneer (Auctioneer Beardo, friendly faction 35)
-- 70102: Kirin Tor Familiar (Trainer & Teleporter, friendly faction 35, Display 14432, Scale 0.5)
-- npc_flags:
--   70100, 70103: UNIT_NPC_FLAG_GOSSIP (1) | UNIT_NPC_FLAG_BANKER (4096) = 4097
--   70101: UNIT_NPC_FLAG_GOSSIP (1) | UNIT_NPC_FLAG_AUCTIONEER (2097152) = 2097153
--   70102: UNIT_NPC_FLAG_GOSSIP (1) ONLY - Helper uses proxy summons for training!
-- unit_flags: 131074 (UNIT_FLAG_SPAWNING [2] | UNIT_FLAG_PACIFIED [131072])
-- health_min / health_max: 100 (Ensures IsAlive() evaluates to true for interaction)
INSERT INTO `creature_template` (
    `entry`, `display_id1`, `name`, `subname`, `health_min`, `health_max`,
    `faction`, `npc_flags`, `unit_flags`, `scale`, `inhabit_type`, `script_name`
) VALUES
(70100, 14546, 'Pack Mule', 'Personal Banker', 100, 100, 35, 4097, 131074, 1.0, 3, 'utility_follower_banker'),
(70103, 7933, 'Pack Kodo', 'Personal Banker', 100, 100, 35, 4097, 131074, 1.0, 3, 'utility_follower_banker'),
(70101, 7993, 'Auctioneer Beardo', 'Personal Auctioneer', 100, 100, 35, 2097153, 131074, 1.0, 3, 'utility_follower_auctioneer'),
(70102, 14432, 'Kirin Tor Familiar', 'Trainer & Teleporter', 100, 100, 35, 1, 131074, 0.5, 3, 'utility_follower_trainer');

-- =========================================================================
-- 2. CLASS TRAINER PROXY TEMPLATES (70110 - 70118)
-- =========================================================================
-- npc_flags = UNIT_NPC_FLAG_GOSSIP (1) | UNIT_NPC_FLAG_TRAINER (16) = 17
-- trainer_type = 1 (TRAINER_TYPE_CLASS)
-- health_min / health_max: 100 (Critical: CanInteractWithNPC requires IsAlive())
-- scale: 0.01 (Virtually invisible proxy)
INSERT INTO `creature_template` (
    `entry`, `display_id1`, `name`, `subname`, `health_min`, `health_max`,
    `faction`, `npc_flags`, `unit_flags`, `scale`, `trainer_type`, `trainer_class`,
    `inhabit_type`, `script_name`
) VALUES
(70110, 1504, 'Warrior Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 1, 3, ''),
(70111, 3289, 'Paladin Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 2, 3, ''),
(70112, 3312, 'Hunter Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 3, 3, ''),
(70113, 1507, 'Rogue Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 4, 3, ''),
(70114, 11048, 'Priest Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 5, 3, ''),
(70115, 2082, 'Shaman Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 7, 3, ''),
(70116, 1294, 'Mage Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 8, 3, ''),
(70117, 3291, 'Warlock Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 9, 3, ''),
(70118, 2261, 'Druid Trainer Proxy', 'Class Trainer', 100, 100, 35, 17, 131074, 0.01, 1, 11, 3, '');

-- =========================================================================
-- 3. PROFESSION TRAINER PROXY TEMPLATES (70120 - 70132)
-- =========================================================================
-- trainer_type = 2 (TRAINER_TYPE_TRADESKILLS)
-- trainer_spell = 0 (Critical: Non-zero trainer_spell blocks new apprentices from learning rank 1)
-- health_min / health_max: 100 (Critical: CanInteractWithNPC requires IsAlive())
-- scale: 0.01 (Virtually invisible proxy)
INSERT INTO `creature_template` (
    `entry`, `display_id1`, `name`, `subname`, `health_min`, `health_max`,
    `faction`, `npc_flags`, `unit_flags`, `scale`, `trainer_type`, `trainer_spell`,
    `trainer_id`, `inhabit_type`, `script_name`
) VALUES
(70120, 2341, 'Alchemy Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70121, 2743, 'Blacksmithing Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70122, 7429, 'Enchanting Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70123, 4041, 'Engineering Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70124, 2366, 'Herbalism Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70125, 2108, 'Leatherworking Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70126, 2575, 'Mining Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70127, 8613, 'Skinning Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70128, 3908, 'Tailoring Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70129, 7039, 'Jewelcrafting Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70130, 2550, 'Cooking Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70131, 3273, 'First Aid Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, ''),
(70132, 7620, 'Fishing Trainer Proxy', 'Profession Trainer', 100, 100, 35, 17, 131074, 0.01, 2, 0, 0, 3, '');

-- =========================================================================
-- 4. COPY CANONICAL TRAINER CATALOGS INTO PROXY TEMPLATES (npc_trainer)
-- =========================================================================
-- Class Spells:
INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70110, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 914;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70111, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 5492;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70112, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 5515;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70113, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 918;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70114, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 11401;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70115, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 3032;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70116, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 328;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70117, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 5495;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70118, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 4217;

-- Profession Spells:
INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70120, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 1386;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70121, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 2836;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70122, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 11072;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70123, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 11017;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70124, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 5484;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70125, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 11097;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70126, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 5513;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70127, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 7088;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70128, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 11052;

-- Custom Turtle WoW Jewelcrafting Trainer catalog (copied from Sylvan Gemcrafter entry 22220):
INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70129, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 22220;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70130, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 1382;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70131, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 2329;

INSERT IGNORE INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 70132, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 5493;
