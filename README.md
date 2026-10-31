# mod-twow-QOL-followers

![mod-twow-QOL-followers Banner](preview.jpg)

mod-twow-QOL-followers is a standalone Turtle WoW custom vMaNGOS module (targeting client 1.18.1 build 7272) that provides players with summonable quality-of-life companion followers for essential town and field services across Azeroth.

The module introduces three distinct utility companions: a faction-tailored Banker, an Auctioneer, and the Kirin Tor Familiar offering full class training, profession training, talent resets, multi-spec management, and worldwide fast-travel teleportation.

## Companions and Services

| Follower | Creature ID | Native Summon Spell | Appearance | Main Services |
| :--- | :---: | :---: | :---: | :--- |
| Banker (Alliance) | 70100 | 46459 (Forworn Mule) | Pack Mule (Display 14546) | Opens personal bank storage vault anywhere in the world. |
| Banker (Horde) | 70103 | 46483 (Caravan Kodo) | Pack Kodo (Display 7933) | Opens personal bank storage vault anywhere in the world. |
| Auctioneer | 70101 | 46460 (Mechanical Auctioneer) | Auctioneer Beardo (Display 7993) | Opens global Auction House browsing, bidding, and listing. |
| Kirin Tor Familiar | 70102 | 46465 (Kirin Tor Familiar) | Kirin Tor Familiar (Display 14432) | Class training, profession training, talent reset, pet training reset, multi-spec, and teleportation. |

### Kirin Tor Familiar Teleportation Network
The Kirin Tor Familiar provides fast-travel teleportation to verified open-world entrance coordinates:
- Alliance Capitals: Stormwind City, Ironforge, Darnassus.
- Horde Capitals: Orgrimmar, Undercity, Thunder Bluff.
- Neutral Hubs: Booty Bay, Gadgetzan, Ratchet, Everlook.
- Classic Dungeons: Ragefire Chasm, The Deadmines, Wailing Caverns, Shadowfang Keep, Blackfathom Deeps, The Stockade, Gnomeregan, Razorfen Kraul, Scarlet Monastery, Razorfen Downs, Uldaman, Zul'Farrak, Maraudon, Sunken Temple, Blackrock Depths, Lower Blackrock Spire, Dire Maul, Scholomance, Stratholme.
- Classic Raids: Molten Core, Onyxia's Lair, Blackwing Lair, Zul'Gurub, Ruins of Ahn'Qiraj, Temple of Ahn'Qiraj, Naxxramas.

## Movement and Behavior

- Native Engine Follow: Companions follow their owner using vMaNGOS MotionMaster MoveFollow at a comfortable 3.0 yard offset without hysteresis stutter.
- Interaction Distraction: Companions automatically pause movement while their owner is interacting with gossip, bank, auction, or trainer windows.
- Catch-Up Recovery: If the player mounts or teleports, the companion instantly catches up when distance exceeds 30 yards.
- Single-Entity Guarantee: Core vanity mini-pet summons triggered by Effect 97 (SPELL_EFFECT_SUMMON_CRITTER) are intercepted and suppressed synchronously via PetScript, ensuring only the interactive utility follower exists.

## Summoning Methods

### 1. Native Spellbook Spells
Spells are automatically learned upon character creation (configurable minimum level, default: Level 1):
- Banker (Alliance): Spell 46459 (Forworn Mule)
- Banker (Horde): Spell 46483 (Caravan Kodo)
- Auctioneer: Spell 46460 (Mechanical Auctioneer)
- Kirin Tor Familiar: Spell 46465 (Kirin Tor Familiar)

Casting a spell summons the corresponding companion. Casting the spell again dismisses the companion. Spells can be dragged directly onto action bars.

### 2. Player Chat Commands
Players can also summon and dismiss followers via in-game chat commands:
- .utility banker (or .utility bank) - Toggle personal banker companion.
- .utility auctioneer (or .utility ah) - Toggle personal auctioneer companion.
- .utility helper (or .utility familiar) - Toggle Kirin Tor Familiar companion.
- .utility dismiss [all|banker|auctioneer|helper|familiar] - Dismiss active followers.
- .utility reload - Reload module configuration (Game Master security level required).

## Installation

1. Place the mod-twow-QOL-followers directory into your Turtle WoW source modules folder:
   tortoise-wow/src/modules/mod-twow-QOL-followers

2. Re-run CMake to register the module:
   cmake -B build -S .

3. Build the core server binary.

4. Apply the world database migration located at:
   data/sql/world/20261030000000_world_qol_followers.sql

5. Copy the configuration file:
   cp conf/mod_qol_followers.conf.dist conf/mod_qol_followers.conf

## Configuration

The module is fully configurable via mod_qol_followers.conf:
- Master enable switches for each follower type.
- Custom display IDs, creature template IDs, and model scales.
- Configurable following distance, update intervals, and catch-up thresholds.
- Teleportation destination toggles, minimum level restrictions, and gold costs.
- Free talent reset option and profession training modes.

## License

This module is released under the GNU General Public License v2 (or at your option any later version).
