# LAS Web Engine — Actual LASRecomp port feasibility gate (v1.0)

Status: **Not yet compiled from LASRecomp-generated output**.

## Verified upstream interfaces

Upstream: https://github.com/Nycolazs/LASRecomp-Android-Plus

- `tools/gen.py` generates C source from a Nintendo Switch `NSO` module, with function names `f_<address>`, tables of addresses and function pointers.
- `runtime/cpu.h` defines the `Cpu` structure, arithmetic/NZCV helpers and native-pointer-based guest memory macros such as `LD32`/`ST32`.
- `runtime/gamelib.h` describes a dynamically loaded library of recompiled functions built from each player's own game files.
- `tools/gen.py --simd=c` is explicitly intended as a portable C option; this alone does not make the whole runtime WebAssembly-ready.

## Proven so far, on iPhone 12 Pro Max

- WebGPU/WebGL2 availability tests
- Simple WebAssembly execution
- Isolated arithmetic helpers
- 1 MiB bounded guest memory proof-of-concept
- CPU/memory combined by JavaScript
- Synthetic WASM function dispatch

All these are demonstrations, **not** execution of Zelda game code.

## Blockers before any claim of an authentic port

1. Obtain a redistributable synthetic/test AArch64 program packaged as NSO, or run the authentic generator on locally owned game files. Do not commit copyrighted assets.
2. Provision `tools/gen.py`'s Python dependencies (including `unicorn`) on the Windows build host.
3. Compare generated source with standard `clang --target=wasm32` compilation; report incompatibilities.
4. Replace native pointer dereferences with checked guest-address-to-WASM-memory translation, without silently truncating 64-bit addresses.
5. Adapt function dispatch/tail calls, libc/HLE services, threading, SIMD and dynamic linking.
6. Build a graphics translation path (Vulkan is not available inside Safari) and quantify the browser memory budget.

## Safety and distribution

Keep proprietary files and derived copyrighted game binaries out of GitHub Pages. Sharing the homebrew test harness is fine; sharing a playable commercial game requires a separate rights analysis.

## Next verification criterion

A reproducible **generated-C-from-synthetic-NSO → wasm32 → Safari** test, with source, generated output, build commands and browser report committed to `dev/recomp-port/`. Do not label this as achieved until it is actually built and run.
