> [!IMPORTANT]
> ## Report crashes from inside Orchid
> If a game crashes, start Orchid again and pick **Help > Report a Crash on GitHub**. It opens a bug report with your build, system, GPU and driver and the end of the crash log already filled in, and opens the log folder so you can attach the full log of the crashed run. Please use it for every crash: reports with a log get fixed much faster, and reports without one are closed.

## What's new in 0.7.2

0.7.2 brings Ocarina of Time from its intro into gameplay, adds a built-in 30 fps patch for it and
ports another round of Vulkan fixes from Eglantine forks. Each ported fix names its source and
author in its commit.

### Games

- **Ocarina of Time:** gameplay renders instead of only the HUD. Its remaining shaders are lowered:
  shadow and per-tap gathers, copied bindless handles, guarded uniform compares and selects,
  64-bit shifts, UIMAD with an immediate, F2IP byte packing, and its upscaler network through a
  software HMMA expansion that needs no cooperative matrices. Doors are no longer pure white, and a
  wrong texture handle no longer makes lit scenes glow. The recommended-settings prompt offers a
  built-in 30 fps lock (base game and current update) and keeps reactive flushing on, which
  playtested faster.
- **Hyrule Warriors:** two compute programs that fetch through uniform mirror registers lower now.

### Shaders and graphics

- A depth-compare texture always gets a float descriptor, which fixes a RADV crash that a bad
  cached pipeline repeated on every boot.
- The alpha test reference is rounded to 1/255 in the pipeline key, so games build far fewer
  duplicate pipelines.
- Vulkan: the device is idle before the swapchain is recreated, an out-of-date swapchain is
  handled before the next acquire, and ReinterpretImage copies into its destination in the right
  layout.

### System

- The game list tooltip shows the display version of NUP files.
- Unit tests build with MSVC and with GCC 13 and warnings as errors.

### Known issues

- **Mario Tennis Fever on NVIDIA:** the GPU can be lost when a match starts (Windows, RTX 30
  series). We are still looking for the cause; until then, try switching the console mode to
  **Docked** and tell us in issue #14 whether that helps.
- Ocarina of Time is still overexposed in some camera angles.
- See the performance and compatibility table in the README.

The PGO profile is the same as in 0.6.2.
