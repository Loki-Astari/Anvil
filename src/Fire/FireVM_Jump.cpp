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
    std::byte instructionFlag = static_cast<std::byte>(((instruction & Assembler::Action_JumpFlag_Mask) >> Assembler::Action_JumpFlag_Shift) & 0x07);
    std::byte test = instructionFlag & state.flagRegister;
    bool      result = (test != std::byte{0});

    if (instruction & Assembler::Action_JumpFNot_Mask)
    {
        result = !result;
    }
    return !result;
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
    if (checkInstructionFlagWithRegister(instruction))
    {
        return;
    }

    auto failAction = [&]()
    {
        running = false;
        result = haltInvalidCallState;
    };

    state.addressRegister[StackP]->init(state.programCounter);
    ++state.addressRegister[StackP];

    state.addressRegister[StackP]->init(state.addressRegister[FrameP]);
    ++state.addressRegister[StackP];

    state.addressRegister[StackP]->init(state.addressRegister[This]);
    ++state.addressRegister[StackP];

    state.addressRegister[FrameP] = state.addressRegister[StackP];

    switch (instruction & Assembler::Action_JumpSize_Mask)
    {
        case Assembler::JumpSize_Rel:
        {
            ++state.programCounter;
            std::int16_t  offset = memory[state.programCounter];
            state.programCounter += offset;
            break;
        }
        case Assembler::JumpSize_Abs:
        {
            ++state.programCounter;
            std::uint32_t  position = memory[state.programCounter];
            ++state.programCounter;
            position = (position << 16) | memory[state.programCounter];

            state.programCounter = position - 1; // Remember we will increment this inside the main loop.
            break;
        }
        case Assembler::JumpSize_Mem:
        {
            int regValue = instruction & Assembler::Action_JumpReg_Mask;
            std::uint32_t position = state.addressRegister[regValue]->getValue<CodeAddress>(failAction);

            state.programCounter = position - 1; // Remember we will increment this inside the main loop.
            break;
        }
        default:
            failAction();
    }
}
