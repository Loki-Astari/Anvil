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

int FireVM::run()
{
    running = true;
    for (state.programCounter = 0; running && state.programCounter < memory.size(); ++state.programCounter)
    {
        Instruction instruction = memory[state.programCounter];

        switch (instruction & Assembler::ActionMask)
        {
            case Assembler::Act_CMD: { runActionCMD(instruction);   break;}
            case Assembler::Act_Load:{ runActionLoad(instruction);  break;}
            default:
            {
                running = false;
                throw std::runtime_error("Invalid Instruction");
            }
        }
    }
    return result;
}

void FireVM::runActionCMD(Instruction instruction)
{
    switch (instruction & Assembler::ActionCMDMask)
    {
        case Assembler::Cmd_NoOp:   {/*Do Nothing*/                 break;}
        case Assembler::Cmd_Kill:   {runActionCmdKill(instruction); break;}
        case Assembler::Cmd_Init:   {runActionCmdInit(instruction); break;}
        case Assembler::Cmd_Import: {runActionCmdImport(instruction); break;}
    }
}

void FireVM::runActionCmdKill(Instruction instruction)
{
    running = false;
    result = instruction & Assembler::Cmd_KillStatusMask;
}
void FireVM::runActionCmdInit(Instruction instruction)
{
    ++state.programCounter;
    std::size_t globalSize = instruction & Assembler::Cmd_InitGlobalMask;
    std::size_t stackSize  = memory[state.programCounter];

    state.global.resize(globalSize);
    state.stack.resize(stackSize);
}
void FireVM::runActionCmdImport(Instruction instruction)
{
    Instruction fileType      = instruction & Assembler::Cmd_ImportFileMask;
    std::size_t fileNameSize  = instruction & Assembler::Cmd_ImportSizeMask;
    char*       fileNameStart = reinterpret_cast<char*>(&memory[state.programCounter + 1]);
    char*       fileNameEnd   = fileNameStart + fileNameSize;
    std::string fileName(fileNameStart, fileNameEnd);

    if (fileType == Assembler::Cmd_ImportFileFire)
    {
        fileName += ".fire";
    }

    // TODO Import file

    state.programCounter += ((fileNameSize / 2) + (fileNameSize % 2));
}

void FireVM::runActionLoad(Instruction instruction)
{
    bool absolute = true;

    Instruction dstReg = instruction & Assembler::Load_DstRegMask;
    Instruction from   = instruction & Assembler::Load_FromMask;
    Instruction srcReg = instruction & Assembler::Load_SrcRegMask;
    Instruction sizeI  = instruction & Assembler::Load_SizeMask;
    Instruction value1 = instruction & Assembler::Load_ValueMask;
    std::int32_t value = value1;
    if (sizeI == Assembler::Load_ValueLarge)
    {
        ++state.programCounter;
        value  = value << 16;
        value  = value | memory[state.programCounter];
    }


    if ((from & Assembler::Load_ValueRegMask) != 0)
    {
        // this is register value;
        value = state.registerFrame[srcReg >> Assembler::SourceShift] + value;
    }
    else
    {
        // This is a literal
        // So there is no src register and it is used to store if the value is negative.
        if (srcReg & Assembler::Load_MarkNeg)
        {
            // from != Register => Literal
            // Then srcReg is used to record if the value is negative.
            value = -value;
        }
        if (srcReg & Assembler::Load_MarkRel)
        {
            absolute = false;
        }
    }

    if ((from & Assembler::Load_ValueIndMask))
    {
        value = memory[value];
    }

    if (!absolute)
    {
        value += state.registerFrame[dstReg >> Assembler::DestinationShift];
    }

    // Load the register with calculated value.
    state.registerFrame[dstReg >> Assembler::DestinationShift] = value;
}
