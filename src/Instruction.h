#pragma once

#include <ostream>

#include "Cpu.h"
#include "Memory.h"
#include "IO.h"
#include "JITSection.h"
#include "asmjit/asmjit.h"

namespace JIT_BASES
{
    constexpr auto DIRTY_MAP_BASE = asmjit::x86::r9;
    constexpr auto MEMORY_BASE = asmjit::x86::r10;
    constexpr auto CPU_BASE = asmjit::x86::r11;
}

class Instruction
{
public:
    virtual std::ostream &print(std::ostream &stream) const = 0;

    virtual void execute(Cpu &cpu, Memory &memory, IO &io) = 0;

    //If the compilation was successful, the assembly will be emitted and the function will return true.
    //Otherwise, assembly will not be emitted and the function will return false.
    //Compiled instructions expect to be called with the PC pointing to the next insn, like all instructions.
    virtual bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) = 0;
};

std::ostream &operator<<(std::ostream &stream, const Instruction &insn);



