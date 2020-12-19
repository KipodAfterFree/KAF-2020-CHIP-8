#pragma once

#include "constants.h"
#include "types.h"
#include <array>

class Memory
{
public:
    //Instructions have to be parsed as big-endian, but I'll use little-endian for the rest of the data
    //for better compatibility with the JIT later.
    [[nodiscard]] opcode getOpcode(word addr) const;

    template<class T>
    void put(word addr, T val)
    {
        if (addr + sizeof(T) > buf.size())
        {
            throw std::runtime_error("Out of bounds");
        }
        *(reinterpret_cast<T *>(static_cast<byte *>(buf.data()) + addr)) = val;
    }

    template<class T>
    [[nodiscard]] T get(word addr) const
    {
        if (addr + sizeof(T) > buf.size())
        {
            throw std::runtime_error("Out of bounds");
        }
        return *(reinterpret_cast<const T *>(static_cast<const byte *>(buf.data()) + addr));
    }

    std::array<byte, MEMORY_SIZE> buf = {};

    //Map for dirty instructions. This will be marked if an instruction is written to, for recompilation.
    std::array<byte, MEMORY_SIZE / 2> dirtyMap = {};
};




