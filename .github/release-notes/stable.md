## What's new in 0.6.1

0.6.1 brings in Djihads80/Eglantine up to build 177 (132 changes since the last merge) on top of
0.6.0's fixes, a much faster start of Donkey Kong Bananza, and a freshly trained PGO profile.

### Donkey Kong Bananza

- **Much faster in the first area:** the game reads GPU-written buffers every frame, and each read
  waited for the whole host GPU. The game counted that wait as frame time and dropped itself to
  20 or 30 fps. It now reads the copy that is already back, like Hyrule Warriors: about 59 instead
  of 22 frames a second after New Game (AMD, docked, 2x).
- Busy areas later in the game (for example Surf Hills, SL200) are still slow: there the GPU
  command work itself is the limit, and that is next.
- Gameplay still renders black on AMD GPUs with the RADV driver (Linux). NVIDIA renders it.

### From Djihads80/Eglantine (build 177)

- **Rumble** in Switch 2 games (Mario Kart World, Kirby Air Riders and others).
- **Faster crowded scenes:** less work per draw on the GPU thread, fewer render pass splits on tiled
  GPUs, small uniform buffers reused within a command buffer.
- **Fire Emblem Fortune's Weave:** its DLSS upscaler is replaced by AMD FSR 3.1.
- **Pokemon Pokopia:** GPU memory is mapped before later command lists are read, so it no longer
  stops after its first frame with Low GPU accuracy.
- **Yoshi and the Mysterious Book:** the demo video picks the right codec and plays.
- **Mario Kart World:** asks once at boot whether to use its recommended settings (default: No).
- Sparse tiled images are backed whole when their tiles can't be set up (AMD), more shader handle
  programs for image and texture buffer descriptors, VP9 video fixes, local wireless (LDN) and room
  network fixes, NSP containers detected as Switch 2 programs.
- New defaults: ScaleForce upscaling and FXAA.

### Kept from 0.6.0

The merge keeps 0.6.0's fixes: Mario Tennis Fever's shadow cascades (0x11F0) and grass, the
Hyrule Warriors movie-end fix (Djihads' build waits instead; set
`EDEN_NX2_VDEC_EOS_KEEP_PENDING=1` for that behaviour), Bananza's intro upscaler slots, and the
Mario Kart World syncpoint guard.

### PGO builds

- The `-pgo` builds use a new profile, trained on this build with Mario Tennis Fever, Hyrule
  Warriors, Mario Kart World, Donkey Kong Bananza and Kirby Air Riders. The old one predated the
  current GPU command and readback paths.

### Tested on this build

Headless on Linux with an AMD GPU (RADV):

- All unit tests pass.
- Mario Tennis Fever reaches its menu at 60 fps.
- Hyrule Warriors plays its opening movie to the end into the cutscene after it.
- Donkey Kong Bananza: New Game into the first area at about 59 fps; a late save loads.
- Every Switch 2 game in the test set boots to its title or menu.

### Known issues

- **Donkey Kong Bananza:** black gameplay on RADV; busy late areas run slowly.
- **Super Mario Galaxy 2 (Switch 2 version)** closes about 5 seconds after start (its own Vulkan
  driver fails to create its device), as in 0.6.0.
