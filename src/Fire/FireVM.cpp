#include "FireVM.h"
#include "Assembler.h"
#include <string>

using namespace ThorsAnvil::Anvil::Fire;

FireVM::FireVM(CodeBlock& memory)
    : memory(memory)
    , programCounter(0)
{}

void FireVM::run()
{
    bool running = true;
    for (programCounter = 0; running && programCounter < memory.size(); ++programCounter)
    {
        Instruction& instruction = memory[programCounter];
        Instruction  cmd = (instruction & Assembler::ActionMask);
        //Instruction  reg = (instruction & regMask) >> regShift;
        //Instruction  off = (instruction & offMask) >> offShift;
        //Instruction  dat = (instruction & datMask) >> datShift;

        switch (cmd)
        {
            case Assembler::Act_CMD:
            {
                break;
            }
            case Assembler::Act_Load:
            {
                break;
            }
            default:
            {
                running = false;
                throw std::runtime_error("Invalid Instruction");
            }
        }
    }
}
