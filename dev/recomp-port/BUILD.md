# LASRecomp generator preflight

The workflow [LASRecomp generator preflight](../../.github/workflows/lasrecomp-preflight.yml) is deliberately **manual**, so it does not run on every game deployment.

It clones the public upstream repository, installs Python's `unicorn` and `lz4` packages, imports `tools/gen.py`, and invokes its CLI help. This establishes whether its *toolchain can start* on GitHub's Linux runner; it does **not** yet create a synthetic NSO or compile generated code to WebAssembly.

Run: repository **Actions → LASRecomp generator preflight → Run workflow**. On failure inspect the step logs.

Follow-on task: synthesize a minimal legal AArch64 NSO with a valid MOD0 layout and use that as generator input. Keep games and game-derived binaries outside the public repository.
