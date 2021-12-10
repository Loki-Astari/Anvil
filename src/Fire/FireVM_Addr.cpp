#include "FireVM.h"
#include "Assembler.h"
#include <string>

#include <iostream>

using ThorsAnvil::Anvil::Fire::FireVM;

void FireVM::runActionAddr(Instruction instruction)
{
    switch (instruction & Assembler::Action_Jump_Mask)
    {
        case Assembler::Addr_LRR:       {return runActionAddrLRR(instruction);}
        case Assembler::Addr_LRP:       {return runActionAddrLRP(instruction);}
        case Assembler::Addr_INC:       {return runActionAddrInc(instruction, +1);}
        case Assembler::Addr_DEC:       {return runActionAddrInc(instruction, -1);}
        case Assembler::Addr_LMR:       {return runActionAddrLMR(instruction);}
        case Assembler::Addr_LMP:       {return runActionAddrLMP(instruction);}
        case Assembler::Addr_LML:       {return runActionAddrLML(instruction);}
        default:
        {
            running = false;
            result = haltInvalidAddr;
        }
    }
}

void FireVM::runActionAddrInc(Instruction instruction, int sign)
{
    int dstReg = (instruction & Assembler::Addr_Reg1_Mask) >> Assembler::Addr_Reg1_Shift;
    Instruction increment = (instruction & Assembler::Addr_Increment_Mask);

    state.addressRegister[dstReg] += (sign * increment);
}

void FireVM::runActionAddrLRR(Instruction instruction)
{
    int dstReg;
    int srcReg;
    Instruction offset;

    std::tie(dstReg, srcReg, offset) = runActionAddrGetReg(instruction);
    state.addressRegister[dstReg] = state.addressRegister[srcReg] + offset;
}

void FireVM::runActionAddrLMR(Instruction instruction)
{
    int dstReg;
    int srcReg;
    Instruction offset;

    std::tie(dstReg, srcReg, offset) = runActionAddrGetReg(instruction);
    state.addressRegister[dstReg]->init(state.addressRegister[srcReg] + offset);
}

void FireVM::runActionAddrLRP(Instruction instruction)
{
    int dstReg;
    int srcReg;
    Instruction offset;

    std::tie(dstReg, srcReg, offset) = runActionAddrGetReg(instruction);
    state.addressRegister[dstReg] = (state.addressRegister[srcReg] + offset)->getAddress([&](){running = false;result = haltMemoryNotAddress;});
}

void FireVM::runActionAddrLMP(Instruction instruction)
{
    int dstReg;
    int srcReg;
    Instruction offset;

    std::tie(dstReg, srcReg, offset) = runActionAddrGetReg(instruction);
    *(state.addressRegister[dstReg]) = *(state.addressRegister[srcReg] + offset);
}

void FireVM::runActionAddrLML(Instruction instruction)
{
    int dstReg = (instruction & Assembler::Addr_Reg1_Mask) >> Assembler::Addr_Reg1_Shift;
    Instruction literalType = (instruction & Assembler::Addr_Literal_Mask);
    switch (literalType)
    {
        case Assembler::Addr_Literal_CodeAddr:
        {
            ++state.programCounter;
            std::int32_t    value = memory[state.programCounter];
            ++state.programCounter;
            value = (value << 16) | memory[state.programCounter];
            state.addressRegister[dstReg]->init(CodeAddress(value));
            break;
        }
        case Assembler::Addr_Literal_DataFrame:
        {
            ++state.programCounter;
            Instruction    value = memory[state.programCounter];
            state.addressRegister[dstReg]->init(DataFrame(value));
            break;
        }
        case Assembler::Addr_Literal_Int:
        {
            ++state.programCounter;
            std::int32_t    value = memory[state.programCounter];
            ++state.programCounter;
            value = (value << 16) | memory[state.programCounter];
            state.addressRegister[dstReg]->init(Int(value));
            break;
        }
        case Assembler::Addr_Literal_String:
        {
            ++state.programCounter;
            std::size_t size = memory[state.programCounter];
            std::size_t offset = size + ((size % 2 == 1) ? 1 : 0);  // If odd move to the next full instruction
            char const* strValue = reinterpret_cast<char const*>(&memory[state.programCounter + 1]);
            state.programCounter += offset / 2;
            state.addressRegister[dstReg]->init(String(strValue, size));
            break;
        }
        default:
        {
            running = false;
            result = haltInvalidLiteralType;
        }
    }
}
std::tuple<int, int, ThorsAnvil::Anvil::Fire::Instruction> FireVM::runActionAddrGetReg(Instruction instruction)
{
    int dstReg = (instruction & Assembler::Addr_Reg1_Mask) >> Assembler::Addr_Reg1_Shift;
    int srcReg = (instruction & Assembler::Addr_Reg2_Mask) >> Assembler::Addr_Reg2_Shift;

    Instruction addType = (instruction & Assembler::Addr_AddType_Mask);
    Instruction offset = 0;
    switch (addType)
    {
        case Assembler::Addr_AddType_Zero:
        {
            break;
        }
        case Assembler::Addr_AddType_Value:
        {
            ++state.programCounter;
            offset = memory[state.programCounter];
            break;
        }
        default:
        {
            running = false;
            result = haltInvalidAddType;
            return {0,0,0};
        }
    }
    return {dstReg, srcReg, offset};
}
