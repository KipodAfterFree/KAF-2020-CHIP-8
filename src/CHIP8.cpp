#include "CHIP8.h"

static const std::vector<byte> FONT = {
        0xf0, 0x90, 0x90, 0x90, 0xf0, //0
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xf0, 0x10, 0xf0, 0x80, 0xf0, //2
        0xf0, 0x10, 0xf0, 0x10, 0xf0, //3
        0x90, 0x90, 0xf0, 0x10, 0x10, //4
        0xf0, 0x80, 0xf0, 0x10, 0xf0, //5
        0xf0, 0x80, 0xf0, 0x90, 0xf0, //6
        0xf0, 0x10, 0x20, 0x40, 0x40, //7
        0xf0, 0x90, 0xf0, 0x90, 0xf0, //8
        0xf0, 0x90, 0xf0, 0x10, 0xf0, //9
        0xf0, 0x90, 0xf0, 0x90, 0x90, //A
        0xe0, 0x90, 0xe0, 0x90, 0xe0, //B
        0xf0, 0x80, 0x80, 0x80, 0xf0, //C
        0xe0, 0x90, 0x90, 0x90, 0xe0, //D
        0xf0, 0x80, 0xf0, 0x80, 0xf0, //E
        0xf0, 0x80, 0xf0, 0x80, 0x80  //F
};

CHIP8::CHIP8(const std::vector<byte> &ROM) : _cpu{}, _memory{}, _io{"CHIP-8"}, _jit(_cpu, _memory, _io)
{
    std::copy(FONT.cbegin() + FONT_START, FONT.cend(), _memory.buf.begin());

    //The stack shall reside after the font. We have 512 bytes of space, and only 80 bytes are consumed by
    //the font, so this should be okay.
    assert((ROM_START - FONT.size()) > 64);     //Sanity check just in case
    _cpu.sp = FONT_START + FONT.size();
    _cpu.pc = ROM_START;

    //Copy the ROM over to the memory
    if (ROM.size() > (MEMORY_SIZE - ROM_START))
    {
        throw std::runtime_error("The ROM is too big. The max size for roms is 0xE00 bytes.");
    }

    std::copy(ROM.cbegin(), ROM.cend(), _memory.buf.begin() + ROM_START);
}

void CHIP8::printSingleInstruction(word addr) const
{
    auto insn = parseInstruction(_memory.getOpcode(addr));
    std::cout << std::hex << "0x" << addr << ": " << *insn << std::endl;
}

void CHIP8::run()
{
    unsigned int clockCounter = 0;

    while (!_io.getExitFlag())
    {
        if (_cpu.pc & 1u) throw std::runtime_error("Odd address executed");

        auto cycleStart = std::chrono::high_resolution_clock::now();

        auto insn = parseInstruction(_memory.getOpcode(_cpu.pc));

        JITFunction jitFunctionPtr = nullptr;

        if (auto *callInsn = dynamic_cast<Instructions::Call *>(insn.get()))
        {
            jitFunctionPtr = _jit.traceCall(callInsn->target);
        }

        //Execute insn normally. Even if there's a JIT block, the CALL insn still needs to be executed.
        //printSingleInstruction(_cpu.pc);
        _cpu.pc += sizeof(opcode);
        insn->execute(_cpu, _memory, _io);

        if (jitFunctionPtr != nullptr)
        {
            jitFunctionPtr();
        }

        _io.pollEvents();

        if (_cpu.soundTimer) IO::beep();

        if (++clockCounter == CLOCKS_PER_TIMER)
        {
            if (_cpu.delayTimer) _cpu.delayTimer--;
            if (_cpu.soundTimer) _cpu.soundTimer--;
            clockCounter = 0;
        }

        auto currentCycleDuration = std::chrono::high_resolution_clock::now() - cycleStart;

        auto sleepDuration = std::chrono::duration_cast<std::chrono::microseconds>(
                CYCLE_DURATION - currentCycleDuration).count();

        if (sleepDuration > 0)
        {
            usleep(sleepDuration);
        }
    }
}