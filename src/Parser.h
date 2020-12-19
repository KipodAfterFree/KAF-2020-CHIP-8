#pragma once

#include "Instructions.h"
#include "Instruction.h"
#include "types.h"
#include "RegID.h"

#include <stdexcept>
#include <memory>

using InstructionPtr = std::unique_ptr<Instruction>;

InstructionPtr parseInstruction(opcode opcode);