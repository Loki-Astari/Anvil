#ifndef THORSANVIL_ANVIL_FIRE_FIREVM_H
#define THORSANVIL_ANVIL_FIRE_FIREVM_H

#include "Memory.h"
#include <tuple>

namespace ThorsAnvil::Anvil::Fire
{

// 8 named address registers.
enum Register {Global, FrameP, This, Extra, StackP, Expr1, Expr2, Expr3, MaxAddressRegister};

struct VMState
{
    VMState()
        : programCounter(0)
        , addressRegister{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
        , flagRegister(0)
    {}
    // Note: Instruction.help
    //       Data space is not required to be contagious.
    DataFrame       global;             // Global Data Space
    DataFrame       stack;              // Stack Data Space

    // Registers.
    std::size_t     programCounter;
    MemoryLocation* addressRegister[MaxAddressRegister];
    std::uint16_t   flagRegister;
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
            void runActionCMDImport(Instruction instruction);
                void runActionCMDImportLoad(Instruction instruction);
                void runActionCMDImportGetSymbol(Instruction instruction);
                void runActionCMDImportCall(Instruction instruction);
        void runActionJump(Instruction instruction);
            void runActionJumpReturn(Instruction instruction);
            void runActionJumpCall(Instruction instruction);
            void runActionJumpJp(Instruction instruction);
            void runActionJumpMethod(Instruction instruction);
                bool checkInstructionFlagWithRegister(Instruction instruction);
                void runActionJumpDoCall(Instruction instruction, std::uint32_t position);
        void runActionAddr(Instruction instruction);
            void runActionAddrLRR(Instruction instruction);
            void runActionAddrLRP(Instruction instruction);
            void runActionAddrLMR(Instruction instruction);
            void runActionAddrLMP(Instruction instruction);
            void runActionAddrLML(Instruction instruction);
            void runActionAddrInc(Instruction instruction, int sign);
                std::tuple<int, int, Instruction> runActionAddrGetReg(Instruction instruction);

    public:
        static constexpr Result     haltRanOutOfProgramMemory       = 0xFFFF0000;
        static constexpr Result     haltInvalidAction               = 0xFFFF0001;
        static constexpr Result     haltInvalidCmd                  = 0xFFFF0002;
        static constexpr Result     haltInvalidCmdLoadType          = 0xFFFF0003;
        static constexpr Result     haltInvalidJump                 = 0xFFFF0004;
        static constexpr Result     haltInvalidAddr                 = 0xFFFF0005;
        static constexpr Result     haltInvalidReturnState          = 0xFFFF0006;
        static constexpr Result     haltInvalidMethod               = 0xFFFF0007;
        static constexpr Result     haltInvalidAddType              = 0xFFFF0008;
        static constexpr Result     haltInvalidLiteralType          = 0xFFFF0009;
        static constexpr Result     haltMemoryNotAddress            = 0xFFFF000A;
        static constexpr Result     haltInvalidImportLoadBadLibrary = 0xFFFF000B;
        static constexpr Result     haltInvalidImportGetSymbolFailed= 0xFFFF000C;
        static constexpr Result     haltInvalidImportNotFunc        = 0xFFFF000C;
};

}

#endif
