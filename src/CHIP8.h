#pragma once

#include "Parser.h"
#include "Cpu.h"
#include "Memory.h"
#include "IO.h"
#include "Instructions.h"
#include "JIT.h"
#include "types.h"
#include "constants.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <cstddef>

using namespace std::chrono_literals;

constexpr auto TIMER_HZ = 60;
constexpr auto CLOCK_HZ = TIMER_HZ * 8;
constexpr auto CLOCKS_PER_TIMER = CLOCK_HZ / TIMER_HZ;
constexpr auto CYCLE_DURATION = 1s / static_cast<double>(CLOCK_HZ);

class CHIP8
{
public:
    explicit CHIP8(const std::vector<byte> &ROM);

    void printSingleInstruction(word addr) const;

    void run();

private:
    Cpu _cpu;
    Memory _memory;
    IO _io;
    JIT _jit;
};




