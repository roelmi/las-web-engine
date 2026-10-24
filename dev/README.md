# LAS Web Engine — Development

This folder contains experimental browser ports. It does not modify the root Island Trial game.

## CPU microport v0.5

Preview: https://roelmi.github.io/las-web-engine/dev/cpu/

The embedded WebAssembly binary was compiled from a port of open-source ARM64 arithmetic helpers, adapted from LASRecomp's MIT-licensed runtime/cpu.h. The source file here is for study and maintenance. The runtime is **not** yet running Zelda: Link's Awakening.

Next technical milestones: guest memory address translation, bounds validation, dispatch, and renderer adaptation. No proprietary game files are included.
