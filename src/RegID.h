#pragma once

#include <sstream>

enum class RegID
{
    V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF
};

std::ostream &operator<<(std::ostream &stream, const RegID reg);