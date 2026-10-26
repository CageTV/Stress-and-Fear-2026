Stress and Fear 2026
Based on Stress and Fear - A Dynamic Sanity System (Nexus 116522) by JaySerpa. Needs the original mod. Part of the 2026 series with Campfire 2026,
Frostfall 2026, Last Seed 2026, Simple Hunting Overhaul 2026 and Immersive Death Cycle 2026.

WHAT IT DOES
------------
Stress and Fear's settings live in a SkyUI MCM. This adds one "Stress and Fear" section to SKSE Menu Framework with the same settings, a live view of your
stress and your fears, and the tools a modded game needs to fix a save:

  Overview   your stress now (with its band: calm, minor, moderate, high, severe), and how many fears you have and have overcome
  Settings   Enable Stress System, Stress Rate (a slider and the five original presets), Enable Fear System, Show Message Box, and your current stress
  Fears      all 24 kinds of enemy with your state for each (afraid, overcome, none) and buttons to give, cure, overcome or reset a fear
  Light the Way, Protections, Location Types, Custom Locations
             the settings, protections, location types and marked places of Light the Way (see below), when that add-on is installed
  HUD        the stress bar: a thin horizontal bar centred above the compass (see below)
  Profile    the settings file, applying it on every load, and reset to default

Every setting is one of the original mod's own global variables, the same ones its MCM writes, so the pages and the mod always agree. Switching the
stress system off sets your stress back to 0, as the original menu does. Your stress and fears are progress, not settings: they can be changed here but are
never saved in the settings file. Turn the original MCM off (or just do not use it) so the two do not fight over the same values.

Your choices are kept in Data/SKSE/Plugins/StressAndFear/settings.ini and applied on every game load, so they follow you to a new character.

THE STRESS BAR
--------------
A thin horizontal bar sits centred just above the vanilla compass and fills with your stress. Its colour follows the band you are in (calm green, yellow-green,
amber, orange, red), thin marks show where the bands start (25, 45, 65, 85), and the word Stress and your number sit above its ends. It pulses slowly in the
severe band. By default it fades out while you are calm and nothing has changed, and comes back when your stress changes or rises above the calm band.

It has the same adjustable controls as the HUD bars of Frostfall 2026: show or hide, display mode (always or contextual), how long it stays after a change,
horizontal and vertical position, scale, length, thickness and opacity, with a reset button, and it stays on screen while the HUD page is open so you can place it.
The settings are kept in Data/SKSE/Plugins/StressAndFear/hud.ini. The bar is only drawn while the stress system is switched on in Settings.

LIGHT THE WAY
-------------
Light the Way (Nexus 171532, by Blackspine18859) is an add-on that adds stress while you explore dangerous places. When it is installed, this plugin runs it
natively: a timer adds stress every interval in stressful places unless a protection stops it (campfire, sneaking, torch, lantern, invisibility, candlelight,
night eye, vampires and undead, werewolves, followers), with the original's order, half chances in combat, follower maths, night rule and level cap.
Its MCM is replaced by four pages: Light the Way (switches, amount, interval, where stress builds, powers), Protections, Location Types (the 21 kinds of
place) and Custom Locations (places you marked or forced to count as interiors, plus buttons that do what its three powers do).

Light the Way's plugin stays installed as the data file: every setting is one of its own global variables and every list of places is one of its own form
lists, so your save and your settings carry over untouched, and you no longer need JContainers for it. To stop its own script from adding stress at the same
time, this plugin holds Light the Way's master switch at 0 while it runs the add-on; its own "Enable Light the Way" switch is on the first page. "Run Light the
Way from this plugin" turns that over: switch it off and the original script resumes at your next location change and its MCM works again.

OPTIONAL: GOURMET STRESS RELIEF
-------------------------------
A KID ini (no plugin) that gives the warm stews, soups, dumplings and fondue of Gourmet - A Cooking Overhaul the keyword Stress and Fear uses for the
vanilla ones (stress down 10), and Gourmet's meads the one vanilla mead has (down 25). Without it, eating those dishes does nothing for your stress. The
installer offers it as "Gourmet stress relief"; by hand, drop the ini from the gourmet folder next to the original mod's.

OPTIONAL: LAST SEED SPOILED FOOD
--------------------------------
A second KID ini (no plugin): eating the spoiled food of Last Seed (moldy bread, rank fish, spoiled meat and the rest) raises your stress by 10. The
installer offers it as "Last Seed spoiled food"; by hand, drop the ini from the lastseed folder next to the original mod's.

OPTIONAL: OPTIMISED FEAR SCRIPT
-------------------------------
The original's Phobia_Tracker script decides at the end of every fight whether you develop or overcome a fear, with 24 copies of the same block, one per kind of
enemy (909 lines). The installer's "Optimised Phobia_Tracker script" has the same logic written once (489 lines): one function tests the enemy in the original order,
two functions develop or overcome the fear from tables, and the voice type and race are read once. The chances, health threshold, waits, sounds, messages and all
183 properties are unchanged, so the original plugin fills them as before. It is the original author's script, rebuilt and shared with credit (patches are welcome
on the original's page). Install it after the original Stress and Fear; uninstall it to go back to the original.

REQUIREMENTS
------------
- Stress and Fear (Nexus 116522) and the requirements it lists (powerofthree's Papyrus Extender, Keyword Item Distributor)
- SKSE Menu Framework 3
- SKSE (Address Library for the build you use)

BUILDS
------
The installer asks which StressAndFear.dll: "new" for Skyrim 1.6.1170 and newer, "older" for Skyrim VR and 1.6.1130 and older.

CREDITS AND LICENSE
-------------------
Stress and Fear by JaySerpa. The settings pages and StressAndFear.dll are by CageTV, MIT licensed (see LICENSE.txt); they contain none of the original
mod's code, meshes, textures, sounds or plugin; the optional script is JaySerpa's own, rebuilt with credit.
