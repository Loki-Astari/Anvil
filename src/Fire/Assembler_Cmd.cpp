#include "Assembler.h"

using ThorsAnvil::Anvil::Fire::Assembler;

int Assembler::assemble_Cmd(std::istream& lineStream)
{
    std::string     action;
    lineStream >> action;

    if (action == "NoOp")
    {
        instructions[0]  = 0x0000;
        return 1;
    }

    // Unknown command report an error.
    output << "Invalid Input: CMD >" << action << "< " << lineStream.rdbuf() << "\n";
    error = true;
    return 0;
}
