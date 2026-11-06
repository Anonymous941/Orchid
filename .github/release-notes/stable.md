> [!IMPORTANT]
> ## Report crashes from inside Orchid
> If a game crashes, start Orchid again and pick **Help > Report a Crash on GitHub**. It opens a bug report with your build, system, GPU and driver and the end of the crash log already filled in, and opens the log folder so you can attach the full log of the crashed run. Please use it for every crash: reports with a log get fixed much faster, and reports without one are closed.

## What's new in 0.8.0

0.8.0 adds DLC mounting for NUP containers, remembered per-game graphics profiles,
and native shader and buffer corrections collected since 0.7.5.

### NUP DLC support

- Add-on RomFS packages embedded in a NUP are now registered with the DLC service and
  can be opened by the game. Previously the loader recognized these packages but warned
  that mounting was unsupported.
- Only DLC belonging to the running application is exposed. When a container carries
  several versions of a DLC, the newest usable RomFS is selected.
- Newly packed NUP files preserve the DLC version from the dump directory instead of
  writing every version as zero. Existing archives are left unchanged.
- LayeredFS mods also apply to NUP and other non-updatable RomFS sources.

### Graphics, shaders and settings

- The graphics quality picker remembers settings per game and includes verified profiles
  for additional supported games, including the original Kirby and the Forgotten Land.
- Native uniform shifts, carry/LEA sequences, packed conversions and local-memory
  addressing handle additional captured instruction patterns.
- Vulkan uniform reads follow the actual mapped segments of split buffers and preserve
  holes in sparse backing. Buffer readback can span separate cached buffers.
- Texture upload and vertex setup paths reduce redundant work. Custom graphics settings
  survive global settings reloads.
- Optional bounded diagnostics help attribute host stalls, frame hitches and unmapped
  GPU reads without enabling those traces for normal play.

### Known limitations

- Mounting a DLC package does not establish that its complete campaign is playable.
- Older NUPs whose packer wrote every DLC version as zero cannot recover their original
  version ordering automatically; repack from the original dump when needed.
- Kirby Air Riders still has rendering and unsupported compute-shader issues.
- Yoshi shader corrections do not establish full game compatibility.
- See the performance and compatibility table in the README for game-specific status.

The PGO profile is unchanged from 0.7.5. Ported fixes retain their source credits in the commits.
