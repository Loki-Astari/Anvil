#include "FireVM.h"
#include "Assembler.h"
#include <string>

#include <iostream>

using ThorsAnvil::Anvil::Fire::FireVM;

void FireVM::runActionJump(Instruction instruction)
{
    switch (instruction & Assembler::Action_Jump_Mask)
    {
        case Assembler::Jump_Return:    {return runActionJumpReturn(instruction);}
        case Assembler::Jump_Method:    {return runActionJumpMethod(instruction);}
        case Assembler::Jump_Jp:        {return runActionJumpJp(instruction);}
        case Assembler::Jump_Call:      {return runActionJumpCall(instruction);}
        default:
        {
            running = false;
            result = haltInvalidJump;
        }
    }
}

bool FireVM::checkInstructionFlagWithRegister(Instruction instruction)
{
    std::uint16_t instructionFlag = ((instruction & Assembler::Action_JumpFlag_Mask) >> Assembler::Action_JumpFlag_Shift) & 0x07;

    return !((instructionFlag == Assembler::Jump_Condition_AL) || ((instructionFlag & state.flagRegister) != 0));
}

void FireVM::runActionJumpReturn(Instruction instruction)
{
    if (checkInstructionFlagWithRegister(instruction))
    {
        return;
    }
    auto failAction = [&]()
    {
        running = false;
        result = haltInvalidReturnState;
    };

    if (state.addressRegister[FrameP] != state.addressRegister[StackP])
    {
        failAction();
    }

    --state.addressRegister[StackP];
    state.addressRegister[This]     = state.addressRegister[StackP]->getValue<DataAddress>(failAction);
    state.addressRegister[StackP]->init(Void{});

    --state.addressRegister[StackP];
    state.addressRegister[FrameP]   = state.addressRegister[StackP]->getValue<DataAddress>(failAction);
    state.addressRegister[StackP]->init(Void{});

    --state.addressRegister[StackP];
    state.programCounter            = state.addressRegister[StackP]->getValue<CodeAddress>(failAction);
    state.addressRegister[StackP]->init(Void{});
}

void FireVM::runActionJumpCall(Instruction instruction)
{
    ++state.programCounter;
    std::uint32_t  position = memory[state.programCounter];
    ++state.programCounter;
    position = (position << 16) | memory[state.programCounter];

    runActionJumpDoCall(instruction, position);
}

void FireVM::runActionJumpMethod(Instruction instruction)
{
    auto failAction = [&]()
    {
        running = false;
        result = haltInvalidMethod;
    };

    std::int32_t offset = instruction & Assembler::Jump_Offset_Mask;

    int regValue = (instruction & Assembler::Action_JumpReg_Mask) >> Assembler::Action_JumpReg_Shift;
    MemoryLocation* functionInfo = state.addressRegister[regValue] + offset;

    CodeAddress position = functionInfo->getValue<CodeAddress>(failAction);

    runActionJumpDoCall(instruction, position);
}

void FireVM::runActionJumpDoCall(Instruction instruction, std::uint32_t position)
{
    state.addressRegister[StackP]->init(state.programCounter);
    ++state.addressRegister[StackP];

    state.addressRegister[StackP]->init(state.addressRegister[FrameP]);
    ++state.addressRegister[StackP];

    state.addressRegister[StackP]->init(state.addressRegister[This]);
    ++state.addressRegister[StackP];

    int regValue = (instruction & Assembler::Action_JumpReg_Mask) >> Assembler::Action_JumpReg_Shift;
    state.addressRegister[FrameP] = state.addressRegister[StackP];
    state.addressRegister[This]   = state.addressRegister[regValue];

    state.programCounter = position - 1; // Remember we will increment this inside the main loop.
}

void FireVM::runActionJumpJp(Instruction instruction)
{
    std::int32_t offset = instruction & Assembler::Jump_Offset_Mask;
    ++state.programCounter;
    offset = (offset << 16) | memory[state.programCounter];

    if (checkInstructionFlagWithRegister(instruction))
    {
        return;
    }

    state.programCounter += offset;
}
