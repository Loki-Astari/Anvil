#include "FireVM.h"
#include "Assembler.h"
#include <string>

#include <iostream>

using ThorsAnvil::Anvil::Fire::FireVM;

void FireVM::runActionCMD(Instruction instruction)
{
    switch (instruction & Assembler::Action_CMD_Mask)
    {
        case Assembler::Cmd_NoOp:       {return; /*Do Nothing*/}
        case Assembler::Cmd_Kill:       {return runActionCMDKill(instruction);}
        case Assembler::Cmd_Init:       {return runActionCMDInit(instruction);}
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
    result = instruction & Assembler::Cmd_Kill_Result_Mask;
}

void FireVM::runActionCMDInit(Instruction instruction)
{
    std::uint32_t globalSize = instruction & 0xFF;

    if (instruction & Assembler::Cmd_Init_GlobalMask)
    {
        ++state.programCounter;
        globalSize = memory[state.programCounter];
    }

    ++state.programCounter;
    std::uint32_t stackSize  = memory[state.programCounter];
    if (instruction & Assembler::Cmd_Init_StackMask)
    {
        ++state.programCounter;
        stackSize = (stackSize << 16) | memory[state.programCounter];
    }

    state.global.resize(globalSize);
    state.stack.resize(stackSize);
}
