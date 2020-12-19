#pragma once

#include "types.h"
#include "RegID.h"
#include <array>
#include <cstdlib>
#include <ctime>

class Cpu
{
public:
    Cpu();

    [[nodiscard]] reg &getRegister(RegID reg);

    [[nodiscard]] static imm8 getRandom();

    reg registers[16] = {};
    volatile word indexRegister = 0;
    volatile word pc = 0;
    volatile word sp = 0;

    volatile byte soundTimer = 0;
    volatile byte delayTimer = 0;
};




