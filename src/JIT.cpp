#include "JIT.h"


JIT::JIT(Cpu &cpu, Memory &memory, IO &io) : _cpu(cpu), _memory(memory), _io(io)
{
    _jitWorker = std::thread(&JIT::_JITThreadLoop, this);
}

JITFunction JIT::traceCall(word addr)
{
    auto invocations = _hotInsns.at(addr);
    JITFunction fptr = nullptr;
    if (invocations >= 10)
    {
        bool shouldCompile = invocations == 10;

        //Check if dirty
        {
            std::lock_guard<std::mutex> mapLock(_mapMutex);
            auto funcAndNumInsns = _compiledCode[addr];

            fptr = funcAndNumInsns.first;

            //Check for dirty bits
            for (int i = addr; i < addr + (funcAndNumInsns.second * sizeof(opcode)); i++)
            {
                if (_memory.dirtyMap[i >> DIRTY_MAP_SHR])
                {
                    _jitrt.release(fptr);

                    //Clear all dirty bits
                    memset(&_memory.dirtyMap[i >> DIRTY_MAP_SHR], 0, funcAndNumInsns.second);
                    shouldCompile = true;
                    break;
                }
            }
        }

        if (shouldCompile)
        {
            _hotInsns.at(addr)++;

            //work order is enqueued to JIT
            {
                std::lock_guard<std::mutex> queueLock(_queueMutex);
                _compilationQueue.push(addr);
            }
            _queueCondVar.notify_one();
        }
    } else
    {
        _hotInsns.at(addr)++;
    }

    return fptr;
}

JITFunction JIT::_compile(word addr, word numInsns)
{
    asmjit::CodeHolder code;
    code.init(_jitrt.environment());
    JITSection jit(addr, numInsns, &code);

    jit.assm.mov(JIT_BASES::CPU_BASE, &_cpu);
    jit.assm.mov(JIT_BASES::MEMORY_BASE, _memory.buf.data());
    jit.assm.mov(JIT_BASES::DIRTY_MAP_BASE, _memory.dirtyMap.data());

    word currentPC = addr;

    word numCompiled = 0;
    for (; numCompiled < numInsns; ++numCompiled)
    {

        //Bind label to current location - this is okay even in case of a vmexit since the instruction that triggered
        //the vmexit will be executed after ret.
        jit.assm.bind(jit.getLabelForAddress(currentPC).value());
        auto insn = parseInstruction(_memory.getOpcode(currentPC));

        currentPC += sizeof(opcode);

        if (!insn->compile(_memory, _io, jit, currentPC))
        {
            //Reconcile PC before vmexit, we failed so we need to go one insn back
            currentPC -= sizeof(opcode);
            break;
        }
    }

    //Set PC
    jit.assm.mov(asmjit::x86::word_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, pc)), currentPC);

    jit.assm.ret();

    //Minimum number of instructions
    if (numCompiled < 2) return nullptr;

    JITFunction func;
    asmjit::Error err = _jitrt.add(&func, &code);
    if (err) throw std::runtime_error("asmjit::Error : " + std::to_string(err));
    return func;
}

void JIT::_JITThreadLoop()
{
    while (!_exit)
    {
        word addr = 0;
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _queueCondVar.wait(lock, [&] { return !_compilationQueue.empty() || _exit; });
            //Another _exit check
            if (_exit) break;

            addr = _compilationQueue.front();
            _compilationQueue.pop();
        }


        size_t numInsns = 0;

        //Count until ret
        for (int currAddr = addr;; currAddr += sizeof(word))
        {
            numInsns++;
            auto insn = parseInstruction(_memory.getOpcode(currAddr));
            if (dynamic_cast<Instructions::Ret *>(insn.get()) != nullptr) break;
        }

        JITFunction fptr = _compile(addr, numInsns);

        {
            std::lock_guard lock(_mapMutex);
            _compiledCode[addr] = std::pair<JITFunction, short>(fptr, numInsns);
        }
    }
}

JIT::~JIT()
{
    _exit = true;

    _queueCondVar.notify_one();

    //Wait until it exits
    _jitWorker.join();
}

