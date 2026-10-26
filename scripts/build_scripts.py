"""Compile the optimised Stress and Fear script(s) with the official Papyrus compiler.

  python scripts/build_scripts.py [original]      scripts/src -> scripts/build   (or scripts/original -> scripts/build-original, to check the toolchain)

Import order: the scripts being built, the original mod's sources, power of three's Papyrus Extender, SKSE, then vanilla.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = r"E:\Tabula Rasa\Stock Game"
MODS = r"E:\Tabula Rasa\mods"
which = sys.argv[1] if len(sys.argv) > 1 else "src"
SRC = os.path.join(HERE, "original" if which == "original" else "src")
OUT = os.path.join(HERE, "build-original" if which == "original" else "build")
COMPILER = os.path.join(GAME, "Papyrus Compiler", "PapyrusCompiler.exe")
FLAGS = os.path.join(GAME, "Data", "Source", "Scripts", "TESV_Papyrus_Flags.flg")
IMPORTS = [
    SRC,
    os.path.join(HERE, "original"),
    os.path.join(MODS, "powerofthree's Papyrus Extender", "Source", "scripts"),
    os.path.join(MODS, "Skyrim Script Extender (SKSE64)", "Scripts", "Source"),
    os.path.join(GAME, "Data", "Source", "Scripts"),
]
for p in [COMPILER, FLAGS, SRC] + IMPORTS:
    if not os.path.exists(p):
        raise SystemExit(f"missing: {p}")
os.makedirs(OUT, exist_ok=True)
log, rc = "", 0
for name in sorted(n for n in os.listdir(SRC) if n.lower().endswith(".psc")):
    r = subprocess.run([COMPILER, name, f"-f={FLAGS}", "-i=" + ";".join(IMPORTS), f"-o={OUT}"], capture_output=True, text=True, cwd=SRC, timeout=180)
    log += (r.stdout or "") + (r.stderr or "")
    rc = rc or r.returncode
open(os.path.join(OUT, "compile.log"), "w", encoding="utf-8").write(log)
errors = [l for l in log.splitlines() if ".psc(" in l]
print(f"compiler exit {rc}: {len([n for n in os.listdir(OUT) if n.endswith('.pex')])} .pex; {len(errors)} error lines")
for l in errors[:20]:
    print("  " + l)
sys.exit(0 if rc == 0 and not errors else 1)
