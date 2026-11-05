> [!IMPORTANT]
> ## Report crashes from inside Orchid
> If a game crashes, start Orchid again and pick **Help > Report a Crash on GitHub**. It opens a bug report with your build, system, GPU and driver and the end of the crash log already filled in, and opens the log folder so you can attach the full log of the crashed run. Please use it for every crash: reports with a log get fixed much faster, and reports without one are closed.

## What's new

This release corrects invalid Vulkan operations found while investigating Kirby Air Riders and
expands native shader pointer tracking. Shader caches rebuild once because the pointer tracking
change records additional shader information.

### Vulkan stability

- ASTC textures recompressed to BC1 or BC3 no longer request incompatible RGBA8 image views.
- Large textures that keep only their smaller mip levels retain the correct host mip offset,
  so views, barriers and uploads stay within the allocated image.
- Query-buffer copies no longer write overlapping destination regions in one command. When
  queries update the same address, the last value is retained.
- Dynamic vertex strides are declared alongside dynamic vertex input, matching the buffer
  binding commands.

The invalid Vulkan calls were reproduced in Kirby Air Riders on Linux/RADV and were absent from
the patched validation runs. The reported Windows/NVIDIA menu crash still needs confirmation;
issue #19 remains open.

### Ocarina of Time

- Native storage-buffer tracking now follows additional loaded pointers and pointer tables used
  by vegetation shaders. Ported from zortos293/Eglantine, with credits and source commits recorded
  in commit 812a4e1eb.

### Interface

- The frame-generation toggle is available in Graphics settings again.

### Known issues

- **Kirby Air Riders:** characters and lighting can still render incorrectly, and some compute
  shaders are still unsupported. The NVIDIA device-loss report is awaiting a tester's result.
- **Ocarina of Time:** the exposure/fog adjustment remains a visual workaround and also thins fog
  in interiors. `EDEN_NX2_OOT_VISUAL_WORKAROUND=0` disables it.
- **Mario Tennis Fever on NVIDIA:** the GPU can be lost when a match starts (Windows, RTX 30
  series). We are still looking for the cause; until then, try switching the console mode to
  **Docked** and tell us in issue #14 whether that helps.
- See the performance and compatibility table in the README.

The PGO profile is the same as in 0.6.2.
