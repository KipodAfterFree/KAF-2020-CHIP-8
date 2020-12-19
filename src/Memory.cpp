#include "Memory.h"

opcode Memory::getOpcode(word addr) const
{
    return (buf.at(addr) << 8u) + (buf.at(addr + 1));
}
