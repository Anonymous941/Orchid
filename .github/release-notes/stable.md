## What's new in 0.6.2

A small fix release on top of 0.6.1.

### Hyrule Warriors

- **No device loss after the opening movie:** since 0.6.0, a draw that reads its arguments from a
  GPU-written record passed the record straight to the host GPU. A record the guest GPU had not
  written in time could hold any words, and on a Windows AMD driver (RX 7800 XT) that lost the
  device in the scene after the opening movie. These draws are now checked on the GPU with the
  same limits as direct draws.

Everything else is unchanged from 0.6.1, including its PGO profile.

### Known issues

- **Donkey Kong Bananza:** black gameplay on RADV; busy late areas run slowly.
- **Super Mario Galaxy 2 (Switch 2 version)** closes about 5 seconds after start (its own Vulkan
  driver fails to create its device), as in 0.6.0.
