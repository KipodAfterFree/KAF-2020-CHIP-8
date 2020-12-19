#include "types.h"

byte getNibble(word in, word nibbleShift)
{
    return (in >> (4u * nibbleShift)) & 0xf;
}

byte getByte(word in, word nibbleShift)
{
    return (in >> (4u * nibbleShift)) & 0xff;
}

addr12 getAddress(word in, word nibbleShift)
{
    return (in >> (4u * nibbleShift)) & 0xfff;;
}

std::ostream &operator<<(std::ostream &stream, imm8 byte)
{
    return stream << "0x" << std::hex << static_cast<size_t>(byte);
}