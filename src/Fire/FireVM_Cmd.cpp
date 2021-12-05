#include "FireVM.h"
#include "Assembler.h"
#include <string>

#include <iostream>

using ThorsAnvil::Anvil::Fire::FireVM;

void FireVM::runActionCMD(Instruction instruction)
{
    switch (instruction & Assembler::ActionCMDMask)
    {
        case Assembler::Cmd_NoOp:       {return; /*Do Nothing*/}
        case Assembler::Cmd_Kill:       {return runActionCMDKill(instruction);}
        default:
        {
            running = false;
            result = haltInvalidCmd;
        }
    }
}

void FireVM::runActionCMDKill(Instruction instruction)
{
    running = false;
    result = instruction & Assembler::Cmd_ResultMask;
}
