#include "FireVM.h"
#include "Assembler.h"
#include <string>

#include <iostream>

using namespace ThorsAnvil::Anvil::Fire;

FireVM::FireVM(VMState& state, CodeBlock& memory)
    : state(state)
    , memory(memory)
    , result(0)
    , running(false)
{}

Result FireVM::run()
{
    running = true;
    for (state.programCounter = 0; running && state.programCounter < memory.size(); ++state.programCounter)
    {
        Instruction instruction = memory[state.programCounter];

        switch (instruction & Assembler::ActionMask)
        {
            case Assembler::Act_CMD: { runActionCMD(instruction);   break;}
            default:
            {
                running = false;
                result = haltInvalidAction;
            }
        }
    }
    if (running && state.programCounter == memory.size())
    {
        return haltRanOutOfProgramMemory;
    }
    return result;
}
