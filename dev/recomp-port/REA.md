# REA and WASM analysis lab

REA upstream: https://github.com/morluto/rea (MIT license). Its `inspect-wasm-artifact` inspects local WASM, including imports, exports, WAT and sections. It is an **inspection utility**, not a porting engine.

## Run the lab

At **GitHub Actions → REA WASM analysis lab → Run workflow**, the manually triggered workflow analyzes only our synthetic copyright-free `dev/recomp-port/safari/fixture.wasm`. It never checks Nintendo game data into GitHub.

The workflow performs a **required** WABT structural validation and Node-based return-value assertion (`42`). A separate **experimental** REA command runs read-only. An incompatible installed WABT version or REA change is reported in `rea-stderr.txt` and `rea-exit-code.txt`, not misrepresented as a successful REA integration.

Download the resulting `rea-wasm-analysis` artifact from the workflow to inspect the evidence. The workflow never changes the production game, rewrites repo files or deploys anything.

REA documents WABT **1.0.42** as its supported external WABT toolchain. The Ubuntu `apt` version may be different, so the REA portion is **not guaranteed** to pass until we pin that requirement and verify the CLI. This is a diagnostic integration trial.

**The significant remaining port blockers:** 64-bit guest pointer translation, ABI conventions, HLE/runtime services, guest memory capacity, Vulkan-to-WebGPU graphics, threading and performance. Passing this WASM analysis says nothing about whether the full game can run.
