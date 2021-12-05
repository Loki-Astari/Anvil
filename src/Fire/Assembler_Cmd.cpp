#include "Assembler.h"

using ThorsAnvil::Anvil::Fire::Assembler;

int Assembler::assemble_Cmd(std::istream& lineStream)
{
    std::string     cmd;
    lineStream >> cmd;

    if (cmd == "NoOp")
    {
        return assemble_CmdNoOp(lineStream);
    }
    if (cmd == "Kill")
    {
        return assemble_CmdKill(lineStream);
    }

    // Unknown command report an error.
    output << "Invalid Input: CMD >" << cmd << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}

int Assembler::assemble_CmdNoOp(std::istream& lineStream)
{
    char invalid;
    if (lineStream >> invalid)
    {
        output << "Invalid Input: CMD NoOp >" << invalid << lineStream.rdbuf() << "<\n";
        error = true;
        return 0;
    }

    instructions[0] = Act_CMD | Cmd_NoOp;
    return 1;
}

int Assembler::assemble_CmdKill(std::istream& lineStream)
{
    std::int16_t  result;
    if (!(lineStream >> result && (result & Assembler::Cmd_ResultMask) == result))
    {
        output << "Invalid Input: CMD Kill >" << result << "< " << lineStream.rdbuf() << ": value out of range.\n";
        error = true;
        return 0;
    }

    instructions[0] = Act_CMD | Cmd_Kill | result;
    return 1;
}
