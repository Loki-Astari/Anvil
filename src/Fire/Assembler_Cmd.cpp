#include "Assembler.h"

using ThorsAnvil::Anvil::Fire::Assembler;

int Assembler::assemble_Cmd(std::istream& lineStream)
{
    std::string     cmd;
    lineStream >> cmd;

    if (cmd == "NoOp")
    {
        instructions[0]  = 0x0000;
        return 1;
    }

    // Unknown command report an error.
    output << "Invalid Input: CMD >" << cmd << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}
