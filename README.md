# KAF-2020-CHIP-8

A semi-functional JIT CHIP-8 emulator, created for the KipodAfterFree CTF 2020 as a difficult Pwn challenge.

### Compilation

```sh
git submodule update --init --recursive # Make sure to download submodules!
mkdir -p cmake-build-debug && cd cmake-build-debug
cmake ..
make
```

A binary with full debug info will be available in cmake-build-debug, and a binary with partially stripped debug info will be available in the root directory of the project.
