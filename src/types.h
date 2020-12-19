#pragma once

#include <cstdint>
#include <cstddef>
#include <ostream>

using imm4 = uint8_t;
using imm8 = uint8_t;
using addr12 = uint16_t;

using byte = uint8_t;
using reg = byte;
using word = uint16_t;
using opcode = word;

constexpr size_t MAX_REG = 0xff;

byte getNibble(word in, word nibbleShift = 0);

byte getByte(word in, word nibbleShift = 0);

addr12 getAddress(word in, word nibbleShift = 0);

std::ostream &operator<<(std::ostream &stream, imm8 byte);