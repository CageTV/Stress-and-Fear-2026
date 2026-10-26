"""Lays out the optimised Stress and Fear script as its own add-on (GitHub only, never uploaded to Nexus).

  package/Stress and Fear 2026 - Optimised Scripts/   scripts/Phobia_Tracker.pex, scripts/source/Phobia_Tracker.psc, README

Needs scripts/build/Phobia_Tracker.pex (python scripts/build_scripts.py).
"""
import os
import shutil

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NAME = "Stress and Fear 2026 - Optimised Scripts"
dest = os.path.join(HERE, "package", NAME)
if os.path.exists(dest):
    shutil.rmtree(dest)
os.makedirs(os.path.join(dest, "scripts", "source"))
shutil.copyfile(os.path.join(HERE, "scripts", "build", "Phobia_Tracker.pex"), os.path.join(dest, "scripts", "Phobia_Tracker.pex"))
shutil.copyfile(os.path.join(HERE, "scripts", "src", "Phobia_Tracker.psc"), os.path.join(dest, "scripts", "source", "Phobia_Tracker.psc"))
open(os.path.join(dest, "README.txt"), "w", encoding="utf-8", newline="\n").write("""Stress and Fear 2026 - Optimised Scripts
Rebuilt from Phobia_Tracker.psc of Stress and Fear - A Dynamic Sanity System (Nexus 116522) by JaySerpa. Needs the original mod. GitHub only: it is not on
Nexus, because it contains a modified copy of the original author's script. It stays up while the author has no objection and comes down on request.

WHAT IT CHANGES
---------------
At the end of every fight the original mod decides whether you develop a fear of the kind of enemy that hurt you, or overcome one. Its script does that with
24 copies of the same block of code, one per kind of enemy (909 lines). This version has the same logic written once (489 lines):

- one function tests the enemy in the original order (the first test that matches wins, exactly as before) and returns its kind;
- two functions develop or overcome the fear, reading the spell, perk and messages of that kind from tables;
- the enemy's voice type and race are read once instead of up to three times each.

Nothing about how the mod plays changes: the same chances (a 3 in 11 roll at the end of a fight for anything to happen, and 2 in 11 for overcoming a fear), the same
health threshold, waits, sounds, messages and the 50 points of stress freed when a fear is overcome. The script keeps all 183 properties of the original, so the
original plugin fills them as before. The enemy tests, the tables and the properties were compared with the original by a script when this was built.

INSTALL
-------
Install it after (below) the original Stress and Fear in MO2's left pane so the script wins. Uninstall it to go back to the original.

CREDITS AND LICENSE
-------------------
Stress and Fear and the original script are by JaySerpa. This rebuilt copy is shared with credit and without any claim over the original work.
""")
print("laid out", dest)
