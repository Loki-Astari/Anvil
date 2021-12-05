#include "FireVM.h"
#include "Assembler.h"
#include <string>

#include <iostream>

using ThorsAnvil::Anvil::Fire::FireVM;

void FireVM::runActionCMD(Instruction instruction)
{
    switch (instruction & Assembler::ActionCMDMask)
    {
        case Assembler::Cmd_NoOp:   {/*Do Nothing*/                 return;}
        default:
        {
            running = false;
            result = haltInvalidCmd;
        }
    }
}
