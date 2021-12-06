#ifndef THORSANVIL_ANVIL_FIRE_FIREVM_H
#define THORSANVIL_ANVIL_FIRE_FIREVM_H

#include "Memory.h"

namespace ThorsAnvil::Anvil::Fire
{

// 8 named address registers.
enum Register {Global, FrameP, This, Reserved1, StackP, Expr1, Expr2, Expr3, MaxAddressRegister};

struct VMState
{
    VMState()
        : programCounter(0)
        , addressRegister{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
        , flagRegister(std::byte{0})
    {}
    // Note: Instruction.help
    //       Data space is not required to be contagious.
    DataFrame       global;             // Global Data Space
    DataFrame       stack;              // Stack Data Space

    // Registers.
    std::size_t     programCounter;
    MemoryLocation* addressRegister[MaxAddressRegister];
    std::byte       flagRegister;
};

using Result = std::uint32_t;

class FireVM
{
    VMState&        state;
    CodeBlock&      memory;
    Result          result;
    bool            running;

    public:
        FireVM(VMState& state, CodeBlock& memory);
        Result run();

    private:
        void runActionCMD(Instruction instruction);
            void runActionCMDKill(Instruction instruction);
            void runActionCMDInit(Instruction instruction);
        void runActionJump(Instruction instruction);
            void runActionJumpReturn(Instruction instruction);
            void runActionJumpCall(Instruction instruction);
            bool checkInstructionFlagWithRegister(Instruction instruction);

    public:
        static constexpr Result     haltRanOutOfProgramMemory       = 0xFFFF0000;
        static constexpr Result     haltInvalidAction               = 0xFFFF0001;
        static constexpr Result     haltInvalidCmd                  = 0xFFFF0002;
        static constexpr Result     haltInvalidJump                 = 0xFFFF0003;
        static constexpr Result     haltInvalidReturnState          = 0xFFFF0004;
        static constexpr Result     haltInvalidCallState            = 0xFFFF0005;
};

}

#endif
