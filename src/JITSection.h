#pragma once

#include <vector>
#include "asmjit/asmjit.h"
#include "types.h"
#include <optional>
#include "Memory.h"

class Cpu;

class IO;

class JITSection final
{
public:

    JITSection(word startingAddr, word numInsns, asmjit::CodeHolder *code, bool shouldLog = false);

    std::optional<asmjit::Label> getLabelForAddress(word addr);

    asmjit::x86::Assembler assm;

private:
    std::vector<asmjit::Label> _labels;

    asmjit::FileLogger _logger;
    word _startingAddr;
    word _numInsns;
};




