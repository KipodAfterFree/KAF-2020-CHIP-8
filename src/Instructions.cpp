#include "Instructions.h"

namespace Instructions
{
    Invalid::Invalid(word opcode) : _opcode(opcode)
    {}

    std::ostream &Invalid::print(std::ostream &stream) const
    {
        return stream << "Invalid Instruction (0x" << std::hex << _opcode << ")";
    }

    void Invalid::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        throw std::runtime_error("Tried to run an invalid instruction " + std::to_string(_opcode));
    }

    bool Invalid::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        throw std::runtime_error("Tried to run an invalid instruction: ");
    }


    Sys::Sys(addr12 addr) : _addr(addr)
    {}

    void Sys::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        throw std::runtime_error("Sys executed");
    }

    std::ostream &Sys::print(std::ostream &stream) const
    {
        return stream << "sys 0x" << std::hex << _addr;
    }

    bool Sys::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        throw std::runtime_error("Sys executed");
    }

    void Cls::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        io.clear();
        io.draw();
    }

    std::ostream &Cls::print(std::ostream &stream) const
    {
        return stream << "cls";
    }

    bool Cls::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto io_ptr = reinterpret_cast<uint64_t>(&io);
        auto clear = &IO::clear;
        auto draw = &IO::draw;
        uint64_t clearPtr = 0;
        uint64_t drawPtr = 0;
        memcpy(&clearPtr, &clear, sizeof(clearPtr));
        memcpy(&drawPtr, &draw, sizeof(drawPtr));

        jit.assm.mov(asmjit::x86::rdi, io_ptr);
        jit.assm.mov(asmjit::x86::rax, clearPtr);
        jit.assm.call(asmjit::x86::rax);

        jit.assm.mov(asmjit::x86::rdi, io_ptr);
        jit.assm.mov(asmjit::x86::rax, drawPtr);
        jit.assm.call(asmjit::x86::rax);

        return true;
    }

    void Ret::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.pc = memory.get<word>(cpu.sp);
        cpu.sp -= sizeof(word);
    }

    std::ostream &Ret::print(std::ostream &stream) const
    {
        return stream << "ret";
    }

    bool Ret::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        return false;
    }

    Jp_imm::Jp_imm(addr12 target) : target(target)
    {}

    void Jp_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.pc = target;
    }

    std::ostream &Jp_imm::print(std::ostream &stream) const
    {
        return stream << "jp 0x" << std::hex << target;
    }

    bool Jp_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto label = jit.getLabelForAddress(target);
        if (label.has_value())
        {
            jit.assm.jmp(label.value());
            return true;
        } else
        {
            return false;
        }
    }

    Call::Call(addr12 target) : target(target)
    {}

    void Call::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.sp += sizeof(word);
        memory.put<word>(cpu.sp, cpu.pc);
        cpu.pc = target;
    }

    std::ostream &Call::print(std::ostream &stream) const
    {
        return stream << "call 0x" << std::hex << target;
    }

    bool Call::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        return false;
    }

    Se_reg_imm::Se_reg_imm(RegID reg, imm8 byte) : _reg(reg), _byte(byte)
    {}

    void Se_reg_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        if (cpu.getRegister(_reg) == _byte)
        {
            cpu.pc += sizeof(opcode);
        }
    }

    std::ostream &Se_reg_imm::print(std::ostream &stream) const
    {
        return stream << "se " << _reg << ", " << std::hex << _byte;
    }

    bool Se_reg_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto targetLabel = jit.getLabelForAddress(pc + sizeof(opcode));
        if (targetLabel.has_value())
        {
            jit.assm.cmp(getPtrForReg(_reg), _byte);
            jit.assm.jz(targetLabel.value());
            return true;
        } else
        {
            return false;
        }
    }

    Sne_reg_imm::Sne_reg_imm(RegID reg, imm8 byte) : _reg(reg), _byte(byte)
    {}

    void Sne_reg_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        if (cpu.getRegister(_reg) != _byte)
        {
            cpu.pc += sizeof(opcode);
        }
    }

    std::ostream &Sne_reg_imm::print(std::ostream &stream) const
    {
        return stream << "sne " << _reg << ", " << std::hex << _byte;
    }

    bool Sne_reg_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto regAddr = getPtrForReg(_reg);
        auto targetLabel = jit.getLabelForAddress(pc + sizeof(opcode));
        if (targetLabel.has_value())
        {
            jit.assm.cmp(regAddr, _byte);
            jit.assm.jnz(targetLabel.value());
            return true;
        } else
        {
            return false;
        }
    }

    Se_reg_reg::Se_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Se_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        if (cpu.getRegister(_reg1) == cpu.getRegister(_reg2))
        {
            cpu.pc += sizeof(opcode);
        }
    }

    std::ostream &Se_reg_reg::print(std::ostream &stream) const
    {
        return stream << "se " << _reg1 << ", " << _reg2;
    }

    bool Se_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto regAddr1 = getPtrForReg(_reg1);
        auto regAddr2 = getPtrForReg(_reg2);
        auto targetLabel = jit.getLabelForAddress(pc + sizeof(opcode));
        if (targetLabel.has_value())
        {
            jit.assm.mov(asmjit::x86::al, regAddr1);
            jit.assm.cmp(asmjit::x86::al, regAddr2);
            jit.assm.jz(targetLabel.value());
            return true;
        } else
        {
            return false;
        }
    }

    Ld_reg_imm::Ld_reg_imm(RegID reg, imm8 byte) : _reg(reg), _byte(byte)
    {}

    void Ld_reg_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg) = _byte;
    }

    std::ostream &Ld_reg_imm::print(std::ostream &stream) const
    {
        return stream << "ld " << _reg << ", " << std::hex << _byte;
    }

    bool Ld_reg_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(getPtrForReg(_reg), _byte);

        return true;
    }

    Add_reg_imm::Add_reg_imm(RegID reg, imm8 byte) : _reg(reg), _byte(byte)
    {}

    void Add_reg_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg) += _byte;
    }

    std::ostream &Add_reg_imm::print(std::ostream &stream) const
    {
        return stream << "add " << _reg << ", " << std::hex << _byte;
    }

    bool Add_reg_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.add(getPtrForReg(_reg), _byte);
        return true;
    }

    Ld_reg_reg::Ld_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Ld_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg1) = cpu.getRegister(_reg2);
    }

    std::ostream &Ld_reg_reg::print(std::ostream &stream) const
    {
        return stream << "ld " << _reg1 << ", " << _reg2;
    }

    bool Ld_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg2));
        jit.assm.mov(getPtrForReg(_reg1), asmjit::x86::al);
        return true;
    }

    Or_reg_reg::Or_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Or_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg1) |= cpu.getRegister(_reg2);
    }

    std::ostream &Or_reg_reg::print(std::ostream &stream) const
    {
        return stream << "or " << _reg1 << ", " << _reg2;
    }

    bool Or_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg2));
        jit.assm.or_(getPtrForReg(_reg1), asmjit::x86::al);
        return true;
    }

    And_reg_reg::And_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void And_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg1) &= cpu.getRegister(_reg2);
    }

    std::ostream &And_reg_reg::print(std::ostream &stream) const
    {
        return stream << "and " << _reg1 << ", " << _reg2;
    }

    bool And_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg2));
        jit.assm.and_(getPtrForReg(_reg1), asmjit::x86::al);
        return true;
    }

    Xor_reg_reg::Xor_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Xor_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg1) ^= cpu.getRegister(_reg2);
    }

    std::ostream &Xor_reg_reg::print(std::ostream &stream) const
    {
        return stream << "xor " << _reg1 << ", " << _reg2;
    }

    bool Xor_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg2));
        jit.assm.xor_(getPtrForReg(_reg1), asmjit::x86::al);
        return true;
    }

    Add_reg_reg::Add_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Add_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        reg &reg1 = cpu.getRegister(_reg1);
        reg &reg2 = cpu.getRegister(_reg2);
        //Set VF to carry flag. I.e. if the result of the operation is larger than the highest representable value.
        cpu.getRegister(RegID::VF) = (reg1 + reg2) > MAX_REG;
        reg1 += reg2;
    }

    std::ostream &Add_reg_reg::print(std::ostream &stream) const
    {
        return stream << "add " << _reg1 << ", " << _reg2;
    }

    bool Add_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg2));
        jit.assm.add(getPtrForReg(_reg1), asmjit::x86::al);
        jit.assm.setc(getPtrForReg(RegID::VF));
        return true;
    }

    Sub_reg_reg::Sub_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Sub_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        reg &reg1 = cpu.getRegister(_reg1);
        reg &reg2 = cpu.getRegister(_reg2);
        //Set VF to NOT borrow, i.e. if reg1 is larger than reg2.
        cpu.getRegister(RegID::VF) = reg1 > reg2;
        reg1 -= reg2;
    }

    std::ostream &Sub_reg_reg::print(std::ostream &stream) const
    {
        return stream << "sub " << _reg1 << ", " << _reg2;
    }

    bool Sub_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg2));
        jit.assm.sub(getPtrForReg(_reg1), asmjit::x86::al);
        jit.assm.setnc(getPtrForReg(RegID::VF));
        return true;
    }

    Shr_reg::Shr_reg(RegID reg) : _reg(reg)
    {}

    void Shr_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        reg &reg = cpu.getRegister(_reg);
        //set flag to least significant bit.
        cpu.getRegister(RegID::VF) = reg & 1u;
        //then divide
        reg = reg >> 1u;
    }

    std::ostream &Shr_reg::print(std::ostream &stream) const
    {
        return stream << "shr " << _reg;
    }

    bool Shr_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.shr(getPtrForReg(_reg), 1);
        jit.assm.setc(getPtrForReg(RegID::VF));

        return true;
    }

    Subn_reg_reg::Subn_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Subn_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        reg &reg1 = cpu.getRegister(_reg1);
        reg &reg2 = cpu.getRegister(_reg2);
        //Set VF to NOT borrow, i.e. if reg1 is larger than reg2.
        cpu.getRegister(RegID::VF) = reg2 > reg1;
        reg1 = reg2 - reg1;
    }

    std::ostream &Subn_reg_reg::print(std::ostream &stream) const
    {
        return stream << "subn " << _reg1 << ", " << _reg2;
    }

    bool Subn_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        //al = reg2
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg2));
        //cl = reg1
        jit.assm.mov(asmjit::x86::cl, getPtrForReg(_reg1));
        //al (reg2) = al (reg2) - cl(reg1)
        jit.assm.sub(asmjit::x86::al, asmjit::x86::al);
        jit.assm.setnc(getPtrForReg(RegID::VF));
        jit.assm.mov(getPtrForReg(_reg1), asmjit::x86::al);
        return true;
    }

    Shl_reg::Shl_reg(RegID reg) : _reg(reg)
    {}

    void Shl_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        reg &reg = cpu.getRegister(_reg);
        //set flag to most significant bit.
        cpu.getRegister(RegID::VF) = reg & MOST_SIGNIFICANT_BIT_MASK;
        //then multiply
        reg = reg << 1u;
    }

    std::ostream &Shl_reg::print(std::ostream &stream) const
    {
        return stream << "shl " << _reg;
    }

    bool Shl_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.shl(getPtrForReg(_reg), 1);
        jit.assm.setc(getPtrForReg(RegID::VF));

        return true;
    }

    Sne_reg_reg::Sne_reg_reg(RegID reg1, RegID reg2) : _reg1(reg1), _reg2(reg2)
    {}

    void Sne_reg_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        if (cpu.getRegister(_reg1) != cpu.getRegister(_reg2))
        {
            cpu.pc += sizeof(opcode);
        }
    }

    std::ostream &Sne_reg_reg::print(std::ostream &stream) const
    {
        return stream << "sne " << _reg1 << ", " << _reg2;
    }

    bool Sne_reg_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto targetLabel = jit.getLabelForAddress(pc + sizeof(opcode));
        if (targetLabel.has_value())
        {
            jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg1));
            jit.assm.cmp(asmjit::x86::al, getPtrForReg(_reg2));
            jit.assm.jnz(targetLabel.value());
            return true;
        } else
        {
            return false;
        }
    }

    Ld_I_imm::Ld_I_imm(addr12 addr) : _addr(addr)
    {}

    void Ld_I_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.indexRegister = _addr;
    }

    std::ostream &Ld_I_imm::print(std::ostream &stream) const
    {
        return stream << "ld I, 0x" << std::hex << _addr;
    }

    bool Ld_I_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto indexAddr = asmjit::x86::word_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, indexRegister));
        jit.assm.mov(indexAddr, _addr);

        return true;
    }

    Jp_v0_imm::Jp_v0_imm(addr12 target) : _target(target)
    {}

    void Jp_v0_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.pc = static_cast<addr12>(_target + cpu.getRegister(RegID::V0)) & MEMORY_MASK;
    }

    std::ostream &Jp_v0_imm::print(std::ostream &stream) const
    {
        return stream << "jp " << RegID::V0 << ", 0x" << std::hex << _target;
    }

    bool Jp_v0_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        return false;
    }

    Rnd_reg_imm::Rnd_reg_imm(RegID reg, imm8 byte) : _reg(reg), _byte(byte)
    {}

    void Rnd_reg_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg) = cpu.getRandom() & _byte;
    }

    std::ostream &Rnd_reg_imm::print(std::ostream &stream) const
    {
        return stream << "rnd " << _reg << ", " << std::hex << _byte;
    }

    bool Rnd_reg_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        jit.assm.mov(asmjit::x86::rax, reinterpret_cast<uint64_t>(&Cpu::getRandom));
        jit.assm.call(asmjit::x86::rax);

        jit.assm.and_(asmjit::x86::al, _byte);
        jit.assm.mov(getPtrForReg(_reg), asmjit::x86::al);

        return true;
    }

    Drw_reg_reg_imm::Drw_reg_reg_imm(RegID reg1, RegID reg2, imm4 sprite_size) : _regX(reg1), _regY(reg2),
                                                                                 _sprite_size(sprite_size)
    {}

    void Drw_reg_reg_imm::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        auto &bitmap = io.getBitmap();

        byte spriteX = cpu.getRegister(_regX);
        byte spriteY = cpu.getRegister(_regY);

        byte &cpuVF = cpu.getRegister(RegID::VF);
        cpuVF = 0;

        //Sprite size is the number of rows
        for (byte yOffset = 0; yOffset < _sprite_size; ++yOffset)
        {
            byte spriteByte = memory.get<byte>(cpu.indexRegister + yOffset);

            for (byte xOffset = 0; xOffset < CHAR_BIT; ++xOffset)
            {
                bool spriteBit = spriteByte & MOST_SIGNIFICANT_BIT_MASK;
                spriteByte = spriteByte << 1u;
                //Wrap-around property as described in the manual
                byte effectiveX = (xOffset + spriteX) % PIXEL_WIDTH, effectiveY = (yOffset + spriteY) % PIXEL_HEIGHT;

                auto index = effectiveX + (effectiveY * PIXEL_WIDTH);
                //If both are true, the bit will get erased in xor.
                cpuVF |= spriteBit && bitmap.at(index);
                bitmap.at(index) ^= spriteBit;
            }
        }

        io.draw();
    }

    std::ostream &Drw_reg_reg_imm::print(std::ostream &stream) const
    {
        return stream << "drw " << _regX << ", " << _regY << ", " << _sprite_size;
    }

    bool Drw_reg_reg_imm::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        return false;
    }

    Skp_reg::Skp_reg(RegID reg) : _reg(reg)
    {}

    void Skp_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        if (io.isPressed(cpu.getRegister(_reg)))
        {
            cpu.pc += sizeof(opcode);
        }
    }

    std::ostream &Skp_reg::print(std::ostream &stream) const
    {
        return stream << "skp " << _reg;
    }

    bool Skp_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        //TODO: Add IO call
        auto targetLabel = jit.getLabelForAddress(pc + sizeof(opcode));
        if (targetLabel.has_value())
        {
            auto io_ptr = reinterpret_cast<uint64_t>(&io);
            auto isPressed = &IO::isPressed;
            uint64_t isPressedPtr = 0;
            memcpy(&isPressedPtr, &isPressed, sizeof(isPressedPtr));

            jit.assm.mov(asmjit::x86::rdi, io_ptr);
            jit.assm.mov(asmjit::x86::rsi, getPtrForReg(_reg));
            jit.assm.mov(asmjit::x86::rax, isPressedPtr);
            jit.assm.call(asmjit::x86::rax);

            jit.assm.test(asmjit::x86::rax, asmjit::x86::rax);
            jit.assm.jnz(targetLabel.value());

            return true;
        } else
        {
            return false;
        }
    }

    Sknp_reg::Sknp_reg(RegID reg) : _reg(reg)
    {}

    void Sknp_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        if (!io.isPressed(cpu.getRegister(_reg)))
        {
            cpu.pc += sizeof(opcode);
        }
    }

    std::ostream &Sknp_reg::print(std::ostream &stream) const
    {
        return stream << "sknp " << _reg;
    }

    bool Sknp_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        //TODO: Add IO call
        auto targetLabel = jit.getLabelForAddress(pc + sizeof(opcode));
        if (targetLabel.has_value())
        {
            auto io_ptr = reinterpret_cast<uint64_t>(&io);
            auto isPressed = &IO::isPressed;
            uint64_t isPressedPtr = 0;
            memcpy(&isPressedPtr, &isPressed, sizeof(isPressedPtr));

            jit.assm.mov(asmjit::x86::rdi, io_ptr);
            jit.assm.mov(asmjit::x86::rsi, getPtrForReg(_reg));
            jit.assm.mov(asmjit::x86::rax, isPressedPtr);
            jit.assm.call(asmjit::x86::rax);

            jit.assm.test(asmjit::x86::rax, asmjit::x86::rax);
            jit.assm.jz(targetLabel.value());

            return true;
        } else
        {
            return false;
        }
    }

    Ld_reg_dt::Ld_reg_dt(RegID reg) : _reg(reg)
    {}

    void Ld_reg_dt::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.getRegister(_reg) = cpu.delayTimer;
    }

    std::ostream &Ld_reg_dt::print(std::ostream &stream) const
    {
        return stream << "ld " << _reg << ", DT";
    }

    bool Ld_reg_dt::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto dtAddr = asmjit::x86::byte_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, delayTimer));
        jit.assm.mov(asmjit::x86::al, dtAddr);
        jit.assm.mov(getPtrForReg(_reg), asmjit::x86::al);
        return true;
    }

    Ld_reg_K::Ld_reg_K(RegID reg) : _reg(reg)
    {}

    void Ld_reg_K::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        auto pressedKey = io.getPressedKey();
        if (pressedKey.has_value())
        {
            cpu.getRegister(_reg) = pressedKey.value();
        } else
        {
            cpu.pc -= sizeof(opcode);
        }
    }

    std::ostream &Ld_reg_K::print(std::ostream &stream) const
    {
        return stream << "ld " << _reg << ", K";
    }

    bool Ld_reg_K::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        return false;
    }

    Ld_dt_reg::Ld_dt_reg(RegID reg) : _reg(reg)
    {}

    void Ld_dt_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.delayTimer = cpu.getRegister(_reg);
    }

    std::ostream &Ld_dt_reg::print(std::ostream &stream) const
    {
        return stream << "ld DT, " << _reg;
    }

    bool Ld_dt_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto dtAddr = asmjit::x86::byte_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, delayTimer));
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg));
        jit.assm.mov(dtAddr, asmjit::x86::al);
        return true;
    }

    Ld_st_reg::Ld_st_reg(RegID reg) : _reg(reg)
    {}

    void Ld_st_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.soundTimer = cpu.getRegister(_reg);
    }

    std::ostream &Ld_st_reg::print(std::ostream &stream) const
    {
        return stream << "ld ST, " << _reg;
    }

    bool Ld_st_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto stAddr = asmjit::x86::byte_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, soundTimer));
        jit.assm.mov(asmjit::x86::al, getPtrForReg(_reg));
        jit.assm.mov(stAddr, asmjit::x86::al);
        return true;
    }

    Add_I_reg::Add_I_reg(RegID reg) : _reg(reg)
    {}

    void Add_I_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        cpu.indexRegister += cpu.getRegister(_reg);
    }

    std::ostream &Add_I_reg::print(std::ostream &stream) const
    {
        return stream << "add I, " << _reg;
    }

    bool Add_I_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto indexAddr = asmjit::x86::word_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, indexRegister));

        jit.assm.movzx(asmjit::x86::ax, getPtrForReg(_reg));
        jit.assm.add(asmjit::x86::ax, indexAddr);
        jit.assm.and_(asmjit::x86::ax, MEMORY_MASK);
        jit.assm.mov(indexAddr, asmjit::x86::ax);

        return true;
    }

    Ld_F_reg::Ld_F_reg(RegID reg) : _reg(reg)
    {}

    void Ld_F_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        //Every font character is composed of 5 bytes of memory.
        cpu.indexRegister = cpu.getRegister(_reg) * 5;
    }

    std::ostream &Ld_F_reg::print(std::ostream &stream) const
    {
        return stream << "ld F, " << _reg;
    }

    bool Ld_F_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto indexAddr = asmjit::x86::word_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, indexRegister));

        jit.assm.movzx(asmjit::x86::ax, getPtrForReg(_reg));
        jit.assm.imul(asmjit::x86::ax, 5);
        jit.assm.mov(indexAddr, asmjit::x86::ax);
        return true;
    }

    Ld_B_reg::Ld_B_reg(RegID reg) : _reg(reg)
    {}

    void Ld_B_reg::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        reg val = cpu.getRegister(_reg);
        for (unsigned int i = 0; i < 3; ++i)
        {
            //First loads ones digit, then tens, then hundreds.
            memory.put<byte>(cpu.indexRegister + (2 - i), val % 10);
            val = val / 10;
            memory.dirtyMap.at((cpu.indexRegister + i) >> DIRTY_MAP_SHR) = 1;
        }
    }

    std::ostream &Ld_B_reg::print(std::ostream &stream) const
    {
        return stream << "ld B, " << _reg;
    }

    bool Ld_B_reg::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto indexAddr = asmjit::x86::word_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, indexRegister));

        jit.assm.movzx(asmjit::x86::ax, getPtrForReg(_reg));
        //Divide by 100
        jit.assm.mov(asmjit::x86::cl, 100);
        jit.assm.div(asmjit::x86::cl);

        jit.assm.movzx(asmjit::x86::rdi, indexAddr);
        jit.assm.and_(asmjit::x86::rdi, MEMORY_MASK);
        jit.assm.lea(asmjit::x86::rdx, asmjit::x86::byte_ptr(JIT_BASES::MEMORY_BASE, asmjit::x86::rdi));

        //Mov al (quotient - 100s digit) to first location
        jit.assm.mov(asmjit::x86::byte_ptr(asmjit::x86::rdx), asmjit::x86::al);

        //Move remainder (tens and singles left after 100s div) to ax for next division
        jit.assm.movzx(asmjit::x86::ax, asmjit::x86::ah);

        //Divide by 10
        jit.assm.mov(asmjit::x86::cl, 10);
        jit.assm.div(asmjit::x86::cl);

        //Mov al (quotient - 10s digit) to second location
        jit.assm.mov(asmjit::x86::byte_ptr(asmjit::x86::rdx, 1), asmjit::x86::al);

        //Mov al (remainder - 1s digit) to third location
        jit.assm.mov(asmjit::x86::byte_ptr(asmjit::x86::rdx, 2), asmjit::x86::ah);


        //Set value for stosb
        jit.assm.mov(asmjit::x86::al, 1);

        //Shr stored index address
        jit.assm.shr(asmjit::x86::rdi, DIRTY_MAP_SHR);

        jit.assm.add(asmjit::x86::rdi, JIT_BASES::DIRTY_MAP_BASE);
        jit.assm.mov(asmjit::x86::ecx, (3u >> DIRTY_MAP_SHR) + 1u);

        jit.assm.cld();
        jit.assm.rep();
        jit.assm.stosb();

        return true;
    }

    Ld_I_regs::Ld_I_regs(RegID reg) : _reg(reg)
    {}

    void Ld_I_regs::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        for (unsigned int i = 0; i <= static_cast<imm4>(_reg); ++i)
        {
            memory.put<byte>(cpu.indexRegister + i, cpu.getRegister(static_cast<RegID>(i)));
            memory.dirtyMap.at((cpu.indexRegister + i) >> DIRTY_MAP_SHR) = 1;
        }

        cpu.indexRegister += static_cast<imm4>(_reg) + 1;
    }

    std::ostream &Ld_I_regs::print(std::ostream &stream) const
    {
        return stream << "ld [I], " << _reg;
    }

    bool Ld_I_regs::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto indexAddr = asmjit::x86::word_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, indexRegister));
        unsigned int numToCopy = static_cast<imm4>(_reg) + 1u;
        unsigned int currentIndex = 0;

        //Add index register - now contains the correct address
        jit.assm.movzx(asmjit::x86::r8, indexAddr);
        jit.assm.and_(asmjit::x86::r8, MEMORY_MASK);
        jit.assm.lea(asmjit::x86::rdi, asmjit::x86::byte_ptr(JIT_BASES::MEMORY_BASE, asmjit::x86::r8));

        while (currentIndex != numToCopy)
        {
            unsigned int bytesLeft = numToCopy - currentIndex;
            if (bytesLeft >= 8)
            {
                jit.assm.mov(asmjit::x86::rdx, getPtrForReg(static_cast<RegID>(currentIndex)));
                jit.assm.mov(asmjit::x86::qword_ptr(asmjit::x86::rdi, currentIndex), asmjit::x86::rdx);
                currentIndex += 8;
            } else if (bytesLeft >= 4)
            {
                jit.assm.mov(asmjit::x86::edx, getPtrForReg(static_cast<RegID>(currentIndex)));
                jit.assm.mov(asmjit::x86::dword_ptr(asmjit::x86::rdi, currentIndex), asmjit::x86::edx);
                currentIndex += 4;
            } else if (bytesLeft >= 2)
            {
                jit.assm.mov(asmjit::x86::dx, getPtrForReg(static_cast<RegID>(currentIndex)));
                jit.assm.mov(asmjit::x86::word_ptr(asmjit::x86::rdi, currentIndex), asmjit::x86::dx);
                currentIndex += 2;
            } else
            {
                jit.assm.mov(asmjit::x86::dl, getPtrForReg(static_cast<RegID>(currentIndex)));
                jit.assm.mov(asmjit::x86::byte_ptr(asmjit::x86::rdi, currentIndex), asmjit::x86::dl);
                currentIndex += 1;
            }
        }

        //Set Dirty map

        //Set value for stosb
        jit.assm.mov(asmjit::x86::al, 1);

        //Add index to target
        jit.assm.movzx(asmjit::x86::rdi, indexAddr);
        jit.assm.and_(asmjit::x86::rdi, MEMORY_MASK);

        jit.assm.shr(asmjit::x86::rdi, DIRTY_MAP_SHR);

        jit.assm.add(asmjit::x86::rdi, JIT_BASES::DIRTY_MAP_BASE);
        jit.assm.mov(asmjit::x86::ecx, (numToCopy >> DIRTY_MAP_SHR) + 1u);

        jit.assm.cld();
        jit.assm.rep();
        jit.assm.stosb();

        //Adjust index addr
        jit.assm.add(indexAddr, numToCopy);

        return true;
    }

    Ld_regs_I::Ld_regs_I(RegID reg) : _reg(reg)
    {}

    void Ld_regs_I::execute(Cpu &cpu, Memory &memory, IO &io)
    {
        for (int i = 0; i <= static_cast<imm4>(_reg); ++i)
        {
            cpu.getRegister(static_cast<RegID>(i)) = memory.get<byte>(cpu.indexRegister + i);
        }

        cpu.indexRegister += static_cast<imm4>(_reg) + 1;
    }

    std::ostream &Ld_regs_I::print(std::ostream &stream) const
    {
        return stream << "ld " << _reg << ", [I]";
    }

    bool Ld_regs_I::compile(Memory &memory, IO &io, JITSection &jit, addr12 pc)
    {
        auto indexAddr = asmjit::x86::word_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, indexRegister));
        unsigned int numToCopy = static_cast<imm4>(_reg) + 1u;
        unsigned int currentIndex = 0;

        //Add index register - now contains the correct address
        jit.assm.movzx(asmjit::x86::r8, indexAddr);
        jit.assm.and_(asmjit::x86::r8, MEMORY_MASK);
        jit.assm.lea(asmjit::x86::rdi, asmjit::x86::byte_ptr(JIT_BASES::MEMORY_BASE, asmjit::x86::r8));

        while (currentIndex != numToCopy)
        {
            unsigned int bytesLeft = numToCopy - currentIndex;
            if (bytesLeft >= 8)
            {
                jit.assm.mov(asmjit::x86::rdx, asmjit::x86::qword_ptr(asmjit::x86::rdi, currentIndex));
                jit.assm.mov(getPtrForReg(static_cast<RegID>(currentIndex)), asmjit::x86::rdx);
                currentIndex += 8;
            } else if (bytesLeft >= 4)
            {
                //asmjit::x86::dword_ptr(asmjit::x86::rdi, currentIndex)
                jit.assm.mov(asmjit::x86::edx, asmjit::x86::dword_ptr(asmjit::x86::rdi, currentIndex));
                jit.assm.mov(getPtrForReg(static_cast<RegID>(currentIndex)), asmjit::x86::edx);
                currentIndex += 4;
            } else if (bytesLeft >= 2)
            {
                jit.assm.mov(asmjit::x86::dx, asmjit::x86::word_ptr(asmjit::x86::rdi, currentIndex));
                jit.assm.mov(getPtrForReg(static_cast<RegID>(currentIndex)), asmjit::x86::dx);
                currentIndex += 2;
            } else
            {
                jit.assm.mov(asmjit::x86::dl, asmjit::x86::byte_ptr(asmjit::x86::rdi, currentIndex));
                jit.assm.mov(getPtrForReg(static_cast<RegID>(currentIndex)), asmjit::x86::dl);
                currentIndex += 1;
            }
        }

        //Adjust index addr
        jit.assm.add(indexAddr, numToCopy);

        return true;
    }
}

asmjit::x86::Mem getPtrForReg(RegID reg)
{
    return asmjit::x86::byte_ptr(JIT_BASES::CPU_BASE, offsetof(Cpu, registers) + static_cast<imm4>(reg));
}
