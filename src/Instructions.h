#pragma once

#include "Instruction.h"
#include "types.h"
#include "constants.h"
#include "RegID.h"

#include "Cpu.h"
#include "Memory.h"
#include "IO.h"
#include "JITSection.h"

#include <cstddef>
#include <cstring>
#include <sstream>
#include <climits>

asmjit::x86::Mem getPtrForReg(RegID reg);

namespace Instructions
{
    constexpr auto MOST_SIGNIFICANT_BIT_MASK = 0x80u;

    class Invalid final : public Instruction
    {
    public:
        explicit Invalid(word opcode);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        word _opcode;
    };

    class Sys final : public Instruction
    {
    public:
        explicit Sys(addr12 addr);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        addr12 _addr;
    };

    class Cls final : public Instruction
    {
    public:
        Cls() = default;

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;
    };

    class Ret final : public Instruction
    {
    public:
        Ret() = default;

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;
    };

    class Jp_imm final : public Instruction
    {
    public:
        explicit Jp_imm(addr12 target);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

        addr12 target;

    private:
        std::ostream &print(std::ostream &stream) const override;

    };

    class Call final : public Instruction
    {
    public:
        explicit Call(addr12 target);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

        addr12 target;

    private:
        std::ostream &print(std::ostream &stream) const override;
    };

    class Se_reg_imm final : public Instruction
    {
    public:
        explicit Se_reg_imm(RegID reg, imm8 byte);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
        imm8 _byte;
    };

    class Sne_reg_imm final : public Instruction
    {
    public:
        explicit Sne_reg_imm(RegID reg, imm8 byte);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
        imm8 _byte;
    };

    class Se_reg_reg final : public Instruction
    {
    public:
        explicit Se_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Ld_reg_imm final : public Instruction
    {
    public:
        explicit Ld_reg_imm(RegID reg, imm8 byte);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
        imm8 _byte;
    };

    class Add_reg_imm final : public Instruction
    {
    public:
        explicit Add_reg_imm(RegID reg, imm8 byte);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
        imm8 _byte;
    };

    class Ld_reg_reg final : public Instruction
    {
    public:
        explicit Ld_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Or_reg_reg final : public Instruction
    {
    public:
        explicit Or_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class And_reg_reg final : public Instruction
    {
    public:
        explicit And_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Xor_reg_reg final : public Instruction
    {
    public:
        explicit Xor_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Add_reg_reg final : public Instruction
    {
    public:
        explicit Add_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Sub_reg_reg final : public Instruction
    {
    public:
        explicit Sub_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Shr_reg final : public Instruction
    {
    public:
        explicit Shr_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Subn_reg_reg final : public Instruction
    {
    public:
        explicit Subn_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Shl_reg final : public Instruction
    {
    public:
        explicit Shl_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Sne_reg_reg final : public Instruction
    {
    public:
        explicit Sne_reg_reg(RegID reg1, RegID reg2);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg1;
        RegID _reg2;
    };

    class Ld_I_imm final : public Instruction
    {
    public:
        explicit Ld_I_imm(addr12 addr);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        addr12 _addr;
    };

    class Jp_v0_imm final : public Instruction
    {
    public:
        explicit Jp_v0_imm(addr12 target);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        addr12 _target;
    };

    class Rnd_reg_imm final : public Instruction
    {
    public:
        explicit Rnd_reg_imm(RegID reg, imm8 byte);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
        imm8 _byte;
    };

    class Drw_reg_reg_imm final : public Instruction
    {
    public:
        explicit Drw_reg_reg_imm(RegID reg1, RegID reg2, imm4 sprite_size);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _regX;
        RegID _regY;
        imm4 _sprite_size;
    };

    class Skp_reg final : public Instruction
    {
    public:
        explicit Skp_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Sknp_reg final : public Instruction
    {
    public:
        explicit Sknp_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_reg_dt final : public Instruction
    {
    public:
        explicit Ld_reg_dt(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_reg_K final : public Instruction
    {
    public:
        explicit Ld_reg_K(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_dt_reg final : public Instruction
    {
    public:
        explicit Ld_dt_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_st_reg final : public Instruction
    {
    public:
        explicit Ld_st_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Add_I_reg final : public Instruction
    {
    public:
        explicit Add_I_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_F_reg final : public Instruction
    {
    public:
        explicit Ld_F_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_B_reg final : public Instruction
    {
    public:
        explicit Ld_B_reg(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_I_regs final : public Instruction
    {
    public:
        explicit Ld_I_regs(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };

    class Ld_regs_I final : public Instruction
    {
    public:
        explicit Ld_regs_I(RegID reg);

        void execute(Cpu &cpu, Memory &memory, IO &io) override;

        bool compile(Memory &memory, IO &io, JITSection &jit, addr12 pc) override;

    private:
        std::ostream &print(std::ostream &stream) const override;

        RegID _reg;
    };
}

