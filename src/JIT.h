#pragma once

#include "asmjit/asmjit.h"
#include <array>
#include <unordered_map>
#include <iostream>

#include <thread>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <utility>
#include <cstring>
#include <condition_variable>

#include "constants.h"
#include "types.h"
#include "Memory.h"
#include "IO.h"

#include "Instructions.h"
#include "Parser.h"

typedef int (*JITFunction)(void);

class JIT final
{
public:
    JIT(Cpu &cpu, Memory &memory, IO &io);

    ~JIT();

    JITFunction traceCall(word addr);

private:
    asmjit::JitRuntime _jitrt;

    Memory &_memory;
    Cpu &_cpu;
    IO &_io;

    bool _exit = false;

    std::array<byte, MEMORY_SIZE> _hotInsns = {};

    std::mutex _queueMutex;
    std::condition_variable _queueCondVar;
    std::queue<word> _compilationQueue;

    std::mutex _mapMutex;
    std::unordered_map<word, std::pair<JITFunction, short>> _compiledCode;

    std::thread _jitWorker;

    void _JITThreadLoop();

    JITFunction _compile(word addr, word numInsns);
};




