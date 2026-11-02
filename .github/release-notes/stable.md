## What's new in 0.5

One branch from now on: `main` (formerly `mario-tennis-fever`). The experimental channel and the
Switch 2 work from the separate worktrees are merged into it, so 0.5 is the first stable build
with the Mario Tennis Fever performance pass.

### Hyrule Warriors: Age of Imprisonment (new, early state)

Boots in about 5 seconds and runs through the first-time setup, the autosave notice and the
opening cutscene at a steady 60 fps (tested for about 3.5 minutes on Linux with an AMD GPU).
Combat was not played with this build yet, so expect problems from there on and please report
what you see.

- **Shaders:** 5503 of the 5523 shaders recorded from this game now translate to the Maxwell path
  (before: 4589). The other 20 are still skipped, so a few effects can be missing: constant LEA
  with carry (7), dependency barrier modes (7), shaders that need the translator's scratch
  registers (4), cooperative matrix multiply (2).
- **Indirect draws:** the game's indirect quad draws (MME macros) are converted on the GPU by a new
  compute pass instead of being dropped.
- **Fewer stalls:** wait-for-idle calls with nothing recorded since the last one are skipped;
  draws with corrupt parameters are skipped instead of taking the GPU down.
- **Per-game defaults** (your own per-game setting always wins): normal CPU clock (Boost sped up
  game time to 1.7x), low GPU accuracy, balanced fences, and preemptive CPU reads (the game reads
  GPU-written buffers every frame; each read waited for the whole GPU queue, 45 instead of 60 fps
  in combat).

### Other Switch 2 games

- **Shader translation:** asynchronous global-to-shared copies (LDGSTS, LDGDEPBAR, DEPBAR),
  MUFU.TANH, HSET2 with an immediate, uniform registers UR28 to UR62, F2I with an absolute value,
  half-float texture gathers, and clip-state updates (CSMTEST). Of the shaders recorded from
  Yoshi, Fire Emblem, Kirby, Drag x Drive and Pokopia, 1037 of 1094 now translate (before: 953).
- **Switch 2 Edition upgrade packs** in a game's folder are loaded as add-on content.
- **Services:** hwopus command 100, vi2 command 106, the controller applet's strap guide and dual
  Joy-Con 2 mode, and a lock around play-time tracking.
- **Debug tracing** for these games, off unless set: `EDEN_PKP_BT` (Pokopia), `EDEN_YO_STATS`
  (Yoshi), `EDEN_DXD_BT` (Drag x Drive).

### Mario Tennis Fever

- The performance pass from 0.4.0 experimental is now part of the stable build: fewer GPU thread
  waits, presents on a host thread, cheaper balanced fences, read traps on fastmem, less work per
  draw. Details and the switch for each change: `NX2_NOTES.md`, section "Mario Tennis Fever
  performance pass". If 0.5 runs worse than 0.4.1 for you, please report it with fps for both.
