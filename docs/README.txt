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
  Profile    the settings file, applying it on every load, and reset to default

Every setting is one of the original mod's own global variables, the same ones its MCM writes, so the pages and the mod always agree. Switching the
stress system off sets your stress back to 0, as the original menu does. Your stress and fears are progress, not settings: they can be changed here but are
never saved in the settings file. Turn the original MCM off (or just do not use it) so the two do not fight over the same values.

Your choices are kept in Data/SKSE/Plugins/StressAndFear/settings.ini and applied on every game load, so they follow you to a new character.

OPTIONAL: GOURMET STRESS RELIEF
-------------------------------
A KID ini (no plugin) that gives the warm stews, soups, dumplings and fondue of Gourmet - A Cooking Overhaul the keyword Stress and Fear uses for the
vanilla ones (stress down 10), and Gourmet's meads the one vanilla mead has (down 25). Without it, eating those dishes does nothing for your stress. The
installer offers it as "Gourmet stress relief"; by hand, drop the ini from the gourmet folder next to the original mod's.

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
mod's code, scripts, meshes, textures, sounds or plugin.
