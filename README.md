# mod-twow-QOL-followers

![mod-twow-QOL-followers Banner](preview.jpg)

[![Turtle WoW](https://img.shields.io/badge/Turtle_WoW-1.18.1%20%2F%201.12.1-orange.svg)](https://turtle-wow.org/)
[![Engine](https://img.shields.io/badge/Core-tortoise--wow%20(vMaNGOS)-blue.svg)]()
[![AI Companion](https://img.shields.io/badge/AI-TortoiseBots%20Compatible-green.svg)]()
[![The Unediting Doctrine](https://img.shields.io/badge/Core%20Modifications-Zero%20(Non--Intrusive)-brightgreen.svg)]()
[![License](https://img.shields.io/badge/License-GPL%20v2-red.svg)](LICENSE)

`mod-twow-QOL-followers` is a standalone Turtle WoW custom vMaNGOS module (targeting client 1.18.1 build 7272) that provides players with summonable quality-of-life companion followers for essential town and field services across Azeroth.

The module introduces three distinct utility companions: a faction-tailored Banker (Alliance Pack Mule / Horde Pack Kodo), an Auctioneer (Auctioneer Beardo), and the Kirin Tor Familiar offering full class training, profession training, talent resets, multi-spec management, and worldwide fast-travel teleportation.

---

## Companions and Services

| Follower | Creature ID | Native Summon Spell | Appearance | Main Services |
| :--- | :---: | :---: | :---: | :--- |
| **Banker (Alliance)** | `70100` | 46459 (*Forworn Mule*) | Pack Mule (Display `14546`) | Opens personal bank storage vault anywhere in the world. |
| **Banker (Horde)** | `70103` | 46483 (*Caravan Kodo*) | Pack Kodo (Display `7933`) | Opens personal bank storage vault anywhere in the world. |
| **Auctioneer** | `70101` | 46460 (*Mechanical Auctioneer*) | Auctioneer Beardo (Display `7993`) | Opens global Auction House browsing, bidding, and listing. |
| **Kirin Tor Familiar** | `70104` | 46465 (*Kirin Tor Familiar*) | Kirin Tor Familiar (Display `14432`) | Class training, profession training, talent reset, pet training reset, multi-spec, and teleportation. |

### Kirin Tor Familiar Teleportation Network
The Kirin Tor Familiar provides fast-travel teleportation to verified open-world entrance coordinates:
- **Alliance Capitals**: Stormwind City, Ironforge, Darnassus.
- **Horde Capitals**: Orgrimmar, Undercity, Thunder Bluff.
- **Neutral Hubs**: Booty Bay, Gadgetzan, Ratchet, Everlook.
- **Classic Dungeons**: Ragefire Chasm, The Deadmines, Wailing Caverns, Shadowfang Keep, Blackfathom Deeps, The Stockade, Gnomeregan, Razorfen Kraul, Scarlet Monastery, Razorfen Downs, Uldaman, Zul'Farrak, Maraudon, Sunken Temple, Blackrock Depths, Lower Blackrock Spire, Dire Maul, Scholomance, Stratholme.
- **Classic Raids**: Molten Core, Onyxia's Lair, Blackwing Lair, Zul'Gurub, Ruins of Ahn'Qiraj, Temple of Ahn'Qiraj, Naxxramas.

---

## Movement and Behavior

- **Native Engine Follow**: Companions follow their owner using vMaNGOS `MotionMaster` `MoveFollow` at a comfortable 3.0 yard offset without hysteresis stutter.
- **Interaction Distraction**: Companions automatically pause movement while their owner is interacting with gossip, bank, auction, or trainer windows (`UNIT_STAT_DISTRACTED`).
- **Catch-Up Recovery**: If the player mounts or teleports, the companion instantly catches up when distance exceeds 30 yards.
- **Single-Entity Guarantee**: Core vanity mini-pet summons triggered by Effect 97 (`SPELL_EFFECT_SUMMON_CRITTER`) are intercepted and suppressed synchronously via `PetScript`, ensuring only the interactive utility follower exists.
- **The Unediting Doctrine**: 100% standalone and non-intrusive. Zero core edits or patches to `tortoise-wow` or `TortoiseBots`.

---

## Summoning Methods

### 1. Native Spellbook Spells
Spells are automatically learned upon character creation (configurable minimum level, default: Level 1):
- **Banker (Alliance)**: Spell `46459` (*Forworn Mule*)
- **Banker (Horde)**: Spell `46483` (*Caravan Kodo*)
- **Auctioneer**: Spell `46460` (*Mechanical Auctioneer*)
- **Kirin Tor Familiar**: Spell `46465` (*Kirin Tor Familiar*)

Casting a spell summons the corresponding companion. Casting the spell again dismisses the companion. Spells can be dragged directly onto action bars.

### 2. Player Chat Commands
Players can also summon and dismiss followers via in-game chat commands:
- `.utility banker` (or `.utility bank`) - Toggle personal banker companion.
- `.utility auctioneer` (or `.utility ah`) - Toggle personal auctioneer companion.
- `.utility helper` (or `.utility familiar`) - Toggle Kirin Tor Familiar companion.
- `.utility dismiss [all|banker|auctioneer|helper|familiar]` - Dismiss active followers.
- `.utility reload` - Reload module configuration (Game Master security level required).

---

## Requirements

- **Server Engine**: Turtle WoW custom vMaNGOS core (`tortoise-wow` targeting client 1.18.1 build 7272).
- **Database**: MariaDB 10.5+ or MySQL 5.7+ (`world` database).
- **Build System**: CMake 3.16+ and a C++17 compatible compiler (MSVC 2022 on Windows, GCC 10+ / Clang 11+ on Linux).

---

## Installation

These steps assume you have a working [TortoiseWoW](https://github.com/Penqle/tortoise-wow) source checkout, initialized core databases, and the server data files for your realm. Use a core revision compatible with this module. Run all build and SQL commands from the core source root, the directory containing the core's `CMakeLists.txt`.


### 1. Add the module and build on Linux

Use Git, CMake, a C++17 compiler, and the core's build dependencies. On Debian or Ubuntu, a typical starting point is:

```bash
sudo apt update
sudo apt install git cmake build-essential libace-dev default-libmysqlclient-dev libssl-dev zlib1g-dev
```

Use your distribution's `mysql` or `mariadb` command-line client if importing SQL manually. Keep any additional dependency and CMake settings required by your core and other enabled modules.

```bash
cd /path/to/tortoise-wow
git clone https://github.com/Ildourol/mod-twow-QOL-followers.git modules/mod-twow-QOL-followers
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$PWD/server-stage" -DMODULE_MOD_TWOW_QOL_FOLLOWERS=static
cmake --build build --parallel
cmake --install build
```

### 2. Add the module and build on Windows

Install Git for Windows, CMake 3.21 or later, and Visual Studio 2022 or its Build Tools with **Desktop development with C++** and a Windows SDK. Open PowerShell with Git and CMake on `PATH`. Use an x64 build and the dependency setup of your working core.

```powershell
Set-Location 'C:\src\tortoise-wow'
git clone https://github.com/Ildourol/mod-twow-QOL-followers.git modules/mod-twow-QOL-followers
cmake -S . -B build-vs -G "Visual Studio 17 2022" -A x64 "-DCMAKE_INSTALL_PREFIX=$PWD/server-stage" -DMODULE_MOD_TWOW_QOL_FOLLOWERS=static
cmake --build build-vs --config Release --parallel
cmake --install build-vs --config Release
```

Check each command for errors before continuing. Both builds include `mangosd`. If the configure output reports an unused module switch, verify the source placement before building. The two build sections are platform alternatives. If you already have a module checkout, use it in place of the clone. A full directory copy, a Linux symlink, or a Windows directory junction works as long as `modules/mod-twow-QOL-followers/src/` is directly below the module directory. The module directory name determines the CMake switch and loader symbol. If reusing a build directory, keep its generator and architecture and preserve the switches for your other modules. Reconfigure after adding the module and verify CMake lists it as static.

The staged world server is normally `server-stage/bin/mangosd` on Linux and `server-stage/mangosd.exe` on Windows. Stop the running world server before deploying the rebuilt executable and any required runtime libraries or module assets. Preserve the live configuration and data files. The paths below assume the standard staged layout. For an existing deployment, use the directory containing the active `mangosd.conf`, including a custom path passed via `-c`.

### 3. Set up the database

Back up the affected databases. The examples use `tw_char` and `tw_world` as database names. Substitute the names from the active `CharacterDatabase.Info` and `WorldDatabase.Info` settings as applicable, plus your database user and any connection options.

Choose 1 import method. For the core auto-updater, verify that `Database.AutoUpdate.Enabled` and `Database.AutoUpdate.SortByName` are enabled, that `Database.AutoUpdate.AllowedModules` includes `mod-twow-QOL-followers` (or uses `all`), and that the runtime host can access the module checkout at the source path embedded by the build. The updater uses the configured `CharUpdateName` and `WorldUpdateName` under `data/sql/` (normally `character` and `world`). Check the first startup log for applied updates. If importing manually, exclude this module from the updater allowlist to avoid replaying the same files. Preserve other modules that use automatic updates.

**Manual import on Linux (Bash)**

```bash
mysql -u root -p tw_world < "modules/mod-twow-QOL-followers/data/sql/world/20261030000000_world_qol_followers.sql"
```

**Manual import on Windows (PowerShell)**

Run `Get-Command mysql.exe` first. If it is unavailable, install the MySQL or MariaDB command-line client and add its `bin` directory to the current PowerShell session. Replace the example path below with your installation path. Use `mariadb.exe` in the import commands if that is the client you installed. Run imports in the order shown and stop if any command reports an error. `cmd.exe` handles input redirection so the SQL file reaches the client directly.

```powershell
# Only needed when mysql.exe is installed but missing from PATH
# $env:Path = 'C:\Program Files\MySQL\MySQL Server 8.0\bin;' + $env:Path
cmd.exe /d /c 'mysql.exe -u root -p tw_world < modules\mod-twow-QOL-followers\data\sql\world\20261030000000_world_qol_followers.sql'
if ($LASTEXITCODE -ne 0) { throw "Module SQL import failed" }
```

### 4. Deploy and enable the configuration

Create `mod_qol_followers.conf` inside the `modules/` subdirectory beside the active `mangosd.conf`. For a first install using the example staging directory, the following commands copy the shipped template. If an active module config already exists, retain it and merge new settings from the template.

**Linux**

```bash
mkdir -p server-stage/etc/modules
cp -n "modules/mod-twow-QOL-followers/conf/mod_qol_followers.conf.dist" "server-stage/etc/modules/mod_qol_followers.conf"
```

**Windows (PowerShell)**

```powershell
New-Item -ItemType Directory -Force 'server-stage/modules' | Out-Null
if (-not (Test-Path 'server-stage/modules/mod_qol_followers.conf')) {
    Copy-Item 'modules/mod-twow-QOL-followers/conf/mod_qol_followers.conf.dist' 'server-stage/modules/mod_qol_followers.conf'
}
```

Keep the template's section header and confirm `UtilityFollowers.Enable = 1` in the active file. Start the rebuilt server with your normal launcher and active `mangosd.conf`. Check the startup log for module configuration, database, and dependency errors.

### Verify the installation

Use `.utility banker`, `.utility auctioneer`, or `.utility helper` on a test character. If a follower fails to spawn, check the world SQL import and the startup log for missing creature templates.

## Configuration Reference

The module is highly customizable via `mod_qol_followers.conf`:

| Setting | Default | Description |
| :--- | :---: | :--- |
| `UtilityFollowers.Enable` | `1` | Master toggle for the entire QoL followers system. |
| `UtilityFollowers.Banker.Enable` | `1` | Enables or disables the personal Banker companion. |
| `UtilityFollowers.Auctioneer.Enable` | `1` | Enables or disables the personal Auctioneer companion. |
| `UtilityFollowers.Trainer.Enable` | `1` | Enables or disables the Kirin Tor Familiar (Trainer & Teleporter). |
| `UtilityFollowers.FollowDistance` | `3.0` | Steady-state distance (yards) the companion maintains behind the player. |
| `UtilityFollowers.StartFollowingDistance` | `4.0` | Deadzone distance (yards) before the follower starts moving. |
| `UtilityFollowers.CatchUpDistance` | `30.0` | Maximum distance before triggering instant catch-up teleport. |
| `UtilityFollowers.FollowUpdateInterval` | `300` | Movement tracking evaluation interval in milliseconds. |
| `UtilityFollowers.TeleportWhenStuck` | `1` | Enables automatic teleportation recovery when pathing is blocked. |
| `UtilityFollowers.MaxActive` | `1` | Maximum simultaneous utility companions per player. |
| `UtilityFollowers.MaxActivePolicy` | `"ReplaceOldest"` | Despawn behavior when exceeding cap (`"ReplaceOldest"` or `"Reject"`). |
| `UtilityFollowers.AllowInWorld` | `1` | Allows companion summoning in the outdoor open world. |
| `UtilityFollowers.AllowInDungeons` | `1` | Allows companion summoning in 5-man dungeons. |
| `UtilityFollowers.AllowInRaids` | `1` | Allows companion summoning in raid instances. |
| `UtilityFollowers.AllowInBattlegrounds` | `0` | Disallows companion summoning inside Battlegrounds. |
| `UtilityFollowers.AllowInArenas` | `0` | Disallows companion summoning inside Arenas. |
| `UtilityFollowers.Banker.Spell.AutoLearn` | `1` | Automatically teaches Banker summon spell to characters on creation / login. |
| `UtilityFollowers.Auctioneer.Spell.AutoLearn` | `1` | Automatically teaches Auctioneer summon spell to characters on creation / login. |
| `UtilityFollowers.Trainer.Spell.AutoLearn` | `1` | Automatically teaches Kirin Tor Familiar summon spell to characters on creation / login. |
| `UtilityFollowers.Spells.DefaultLearnLevel` | `1` | Minimum character level required to auto-learn companion summon spells. |
| `UtilityFollowers.Spells.SyncOnLogin` | `1` | Synchronizes and grants any missing companion spells upon player login. |
| `UtilityFollowers.Trainer.FreeTraining` | `0` | Whether class and profession training spells cost 0 copper (`0` = Normal, `1` = Free). |
| `UtilityFollowers.Trainer.FreeTalentReset` | `0` | Whether resetting talents through Kirin Tor Familiar is free of cost (`0` = Normal, `1` = Free). |
| `UtilityFollowers.Trainer.Teleport.Enable` | `1` | Master switch for Kirin Tor Familiar fast-travel teleportation services. |
| `UtilityFollowers.Trainer.Teleport.Cost` | `0` | Copper cost charged per teleportation (`0` = Free). |
| `UtilityFollowers.Trainer.Teleport.MinLevel` | `1` | Minimum character level required to use teleportation. |
| `UtilityFollowers.Trainer.Teleport.EnableExtraLocations` | `1` | Enables neutral and quest hubs (Booty Bay, Gadgetzan, Ratchet, Everlook). |
| `UtilityFollowers.Trainer.Teleport.EnableDungeons` | `1` | Enables Classic Vanilla 5-player dungeon entrances. |
| `UtilityFollowers.Trainer.Teleport.EnableRaids` | `1` | Enables Classic Vanilla raid entrances. |

---

## License

This module is released under the GNU General Public License v2 (or at your option any later version).
