# KAF-2020-CHIP-8

CHIP-8 was a hard Pwn challenge in the KipodAfterFree CTF 2020.

### Compilation

```sh
git submodule update --init --recursive # Make sure to download submodules!
mkdir -p cmake-build-debug && cd cmake-build-debug
cmake ..
make
```

A binary with full debug info will be available in cmake-build-debug, and a binary with partially stripped debug info will be available in the root directory of the project.
