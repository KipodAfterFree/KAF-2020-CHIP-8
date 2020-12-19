#include "Cpu.h"

Cpu::Cpu()
{
    std::srand(std::time(nullptr));
}

[[nodiscard]] reg &Cpu::getRegister(RegID reg)
{
    return registers[static_cast<size_t>(reg)];
}

[[nodiscard]] imm8 Cpu::getRandom()
{
    return std::rand() & 0xff;
}

