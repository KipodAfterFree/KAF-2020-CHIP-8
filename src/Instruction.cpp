#include "Instruction.h"

std::ostream &operator<<(std::ostream &stream, const Instruction &insn)
{
    return insn.print(stream);
}