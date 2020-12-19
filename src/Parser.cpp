#include "Parser.h"

InstructionPtr parseInstruction(opcode opcode)
{
    auto reg1 = static_cast<RegID>(getNibble(opcode, 2));
    auto reg2 = static_cast<RegID>(getNibble(opcode, 1));

    switch (getNibble(opcode, 3))
    {
        case 0x0:
            if (opcode == 0x00e0) return std::make_unique<Instructions::Cls>();
            else if (opcode == 0x00ee) return std::make_unique<Instructions::Ret>();
            else return std::make_unique<Instructions::Sys>(getAddress(opcode));

        case 0x1:
            return std::make_unique<Instructions::Jp_imm>(getAddress(opcode));

        case 0x2:
            return std::make_unique<Instructions::Call>(getAddress(opcode));

        case 0x3:
        {
            //3xkk - SE Vx, kk
            byte imm = getByte(opcode);
            return std::make_unique<Instructions::Se_reg_imm>(reg1, imm);
        }

        case 0x4:
        {
            //4xkk - SNE Vx, kk
            byte imm = getByte(opcode);
            return std::make_unique<Instructions::Sne_reg_imm>(reg1, imm);
        }

        case 0x5:
        {
            //5xy0 - SE Vx, Vy
            if (getNibble(opcode) != 0) break;

            return std::make_unique<Instructions::Se_reg_reg>(reg1, reg2);
        }

        case 0x6:
        {
            //6xkk - LD Vx, kk
            byte imm = getByte(opcode);
            return std::make_unique<Instructions::Ld_reg_imm>(reg1, imm);
        }

        case 0x7:
        {
            //7xkk - ADD Vx, kk
            byte imm = getByte(opcode);
            return std::make_unique<Instructions::Add_reg_imm>(reg1, imm);
        }

        case 0x8:
        {
            switch (getNibble(opcode))
            {
                case 0x0:
                    return std::make_unique<Instructions::Ld_reg_reg>(reg1, reg2);
                case 0x1:
                    return std::make_unique<Instructions::Or_reg_reg>(reg1, reg2);
                case 0x2:
                    return std::make_unique<Instructions::And_reg_reg>(reg1, reg2);
                case 0x3:
                    return std::make_unique<Instructions::Xor_reg_reg>(reg1, reg2);
                case 0x4:
                    return std::make_unique<Instructions::Add_reg_reg>(reg1, reg2);
                case 0x5:
                    return std::make_unique<Instructions::Sub_reg_reg>(reg1, reg2);
                case 0x6:
                    return std::make_unique<Instructions::Shr_reg>(reg1);
                case 0x7:
                    return std::make_unique<Instructions::Subn_reg_reg>(reg1, reg2);
                case 0xe:
                    return std::make_unique<Instructions::Shl_reg>(reg1);

                default:
                    break;
            }
        }

        case 0x9:
        {
            if (getNibble(opcode) != 0) break;
            return std::make_unique<Instructions::Sne_reg_reg>(reg1, reg2);
        }

        case 0xA:
            return std::make_unique<Instructions::Ld_I_imm>(getAddress(opcode));

        case 0xB:
            return std::make_unique<Instructions::Jp_v0_imm>(getAddress(opcode));

        case 0xC:
            return std::make_unique<Instructions::Rnd_reg_imm>(reg1, getByte(opcode));

        case 0xD:
            return std::make_unique<Instructions::Drw_reg_reg_imm>(reg1, reg2, getNibble(opcode));

        case 0xE:
        {
            switch (getByte(opcode))
            {
                case 0x9E:
                    return std::make_unique<Instructions::Skp_reg>(reg1);
                case 0xA1:
                    return std::make_unique<Instructions::Sknp_reg>(reg1);
                default:
                    break;
            }
        }

        case 0xF:
        {
            switch (getByte(opcode))
            {
                case 0x07:
                    return std::make_unique<Instructions::Ld_reg_dt>(reg1);
                case 0x0A:
                    return std::make_unique<Instructions::Ld_reg_K>(reg1);
                case 0x15:
                    return std::make_unique<Instructions::Ld_dt_reg>(reg1);
                case 0x18:
                    return std::make_unique<Instructions::Ld_st_reg>(reg1);
                case 0x1E:
                    return std::make_unique<Instructions::Add_I_reg>(reg1);
                case 0x29:
                    return std::make_unique<Instructions::Ld_F_reg>(reg1);
                case 0x33:
                    return std::make_unique<Instructions::Ld_B_reg>(reg1);
                case 0x55:
                    return std::make_unique<Instructions::Ld_I_regs>(reg1);
                case 0x65:
                    return std::make_unique<Instructions::Ld_regs_I>(reg1);
                default:
                    break;
            }
        }
        default:
            break;
    }

    return std::make_unique<Instructions::Invalid>(opcode);
}
