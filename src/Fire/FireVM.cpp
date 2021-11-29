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

    state.registerFrame[Register::Global] = &state.global[0];
    state.registerFrame[Register::Stack]  = &state.stack[0];
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
    //bool absolute = true;

    Instruction dstReg = instruction & Assembler::Load_DstRegMask;
    Instruction srcReg = instruction & Assembler::Load_SrcRegMask;
    Instruction action = instruction & Assembler::Load_ActionMask;
    Instruction sizeI  = instruction & Assembler::Load_SizeMask;
    Instruction value1 = instruction & Assembler::Load_ValueMask;

    std::int32_t value = value1;
    if (value & Assembler::Load_ValueSignBit)
    {
        // sign extend negative numbers;
        std::int32_t extend = Assembler::Load_ValueMask;
        extend = ~extend;
        value = extend | value;
    }
    if (sizeI == Assembler::Load_ValueLarge)
    {
        ++state.programCounter;
        value  = (value << 16) | memory[state.programCounter];
    }

    MemoryLocation*&    dst = state.registerFrame[dstReg >> Assembler::DestinationShift];
    MemoryLocation*     src = state.registerFrame[srcReg >> Assembler::SourceShift];

    dst = src + value;
    if (action)
    {
        dst = dst->getObject();
    }
}
