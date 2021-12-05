#include "Assembler.h"

using ThorsAnvil::Anvil::Fire::Assembler;

int Assembler::assemble_Cmd(std::istream& lineStream)
{
    std::string     cmd;
    lineStream >> cmd;

    if (cmd == "NoOp")      {return assemble_CmdNoOp();}
    if (cmd == "Kill")      {return assemble_CmdKill(lineStream);}
    if (cmd == "Init")      {return assemble_CmdInit(lineStream);}

    // Unknown command report an error.
    errorStream << "Invalid Input: CMD >" << cmd << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}

int Assembler::assemble_CmdNoOp()
{
    instructions[0] = Act_CMD | Cmd_NoOp;
    return 1;
}

int Assembler::assemble_CmdKill(std::istream& lineStream)
{
    std::int16_t  result;
    if (!(lineStream >> result && (result & Assembler::Cmd_Kill_Result_Mask) == result))
    {
        errorStream << "Invalid Input: CMD Kill >" << result << "< " << lineStream.rdbuf() << ": value out of range.\n";
        error = true;
        return 0;
    }

    instructions[0] = Act_CMD | Cmd_Kill | result;
    return 1;
}

int Assembler::assemble_CmdInit(std::istream& lineStream)
{
    std::uint64_t    globalSize = 0xFFFF'FFFF'FFFF'FFFF;
    std::uint64_t    stackSize  = 0xFFFF'FFFF'FFFF'FFFF;

    lineStream >> globalSize >> stackSize;
    if (globalSize > 0xFFFF || stackSize > 0xFFFFFFFF)
    {
        errorStream << "Invalid Input: CMD Init >" << globalSize << " " << stackSize << "< " << lineStream.rdbuf() << ": size values out of range\n";
        error = true;
        return 0;
    }

    instructions[0] = Act_CMD | Cmd_Init;

    if (globalSize <= 0xFF && stackSize <= 0xFFFF)
    {
        instructions[0] |= Cmd_Init_Size_8_16 | (static_cast<Instruction>(globalSize & 0xFF));
        instructions[1] = static_cast<Instruction>(stackSize);
        return 2;
    }
    else if (globalSize <= 0xFF)
    {
        instructions[0] |= Cmd_Init_Size_8_32 | (static_cast<Instruction>(globalSize & 0xFF));
        instructions[1] = static_cast<Instruction>((stackSize >> 16) & 0xFFFF);
        instructions[2] = static_cast<Instruction>(stackSize & 0xFFFF);
        return 3;
    }
    else if (stackSize <= 0xFFFF)
    {
        instructions[0] |= Cmd_Init_Size_16_16;
        instructions[1] = static_cast<Instruction>(globalSize & 0xFFFF);
        instructions[2] = static_cast<Instruction>(stackSize & 0xFFFF);
        return 3;
    }
    else
    {
        instructions[0] |= Cmd_Init_Size_16_32;
        instructions[1] = static_cast<Instruction>(globalSize & 0xFFFF);
        instructions[2] = static_cast<Instruction>((stackSize >> 16) & 0xFFFF);
        instructions[3] = static_cast<Instruction>(stackSize & 0xFFFF);
        return 4;
    }
}
