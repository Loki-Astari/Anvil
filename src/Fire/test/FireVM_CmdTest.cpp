#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

/*
 * Cmd_NoOp)
 * Cmd_Kill_10)
 * Cmd_Kill_Max)
 * Cmd_Init_8_16
 * Cmd_Init_16_16
 * Cmd_Init_8_32
 * Cmd_Init_16_32
 * -----------
 * Cmd_Invalid)
 */

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVM_CmdTest, Cmd_NoOp)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD NoOp
CMD Kill 0
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, 0, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Kill_10)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD NoOp
CMD Kill 10
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, 10, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Kill_Max)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD NoOp
CMD Kill 511
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, 511, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Init_8_16)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 255 256
CMD Kill 511
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 255, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 5, result);
    EXPECT_EQ_OR_LOG(bad, output, 511, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Init_16_16)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 256 256
CMD Kill 511
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 6, result);
    EXPECT_EQ_OR_LOG(bad, output, 511, result);
    EXPECT_SUCC(bad, result);
}
TEST(FireVM_CmdTest, Cmd_Init_8_32)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 255 65536
CMD Kill 511
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 255, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 65536, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 6, result);
    EXPECT_EQ_OR_LOG(bad, output, 511, result);
    EXPECT_SUCC(bad, result);
}
TEST(FireVM_CmdTest, Cmd_Init_16_32)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 256 65536
CMD Kill 511
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 65536, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 7, result);
    EXPECT_EQ_OR_LOG(bad, output, 511, result);
    EXPECT_SUCC(bad, result);
}


// Error Conditions

TEST(FireVM_CmdTest, Cmd_Invalid)
{
    VMState     machineState;
    CodeBlock   machineProgram{Assembler::InvalidCmd};

    FireVM      vm(machineState, machineProgram);
    Result      result = vm.run();

    EXPECT_EQ(result, FireVM::haltInvalidCmd);
}

TEST(FireVM_CmdTest, Cmd_ImportInvalid)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Import Call Expr-1 Expr-2
)");

    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 3, result);
    if (vm.codeBlock.size() == 3)
    {
        vm.codeBlock[2] |= Assembler::Cmd_Import_Mask;
        Result      output = vm.run();
        EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidCmdLoadType, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_ImportValidLibrary)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 0
CMD Import Load Expr-1 Anvil_System
CMD Kill 33
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, 33, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_ImportValidLibraryAndSymbol)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 0
ADDR LRR Expr-2 = StackPointer + 1
CMD Import Load Expr-1 Anvil_System
CMD Import GetSymbol Expr-2 Expr-1 _ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
CMD Kill 34
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, 34, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_ImportValidLibraryAndSymbolThenCall)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 0
ADDR LRR Expr-2 = StackPointer + 1
ADDR LRR Expr-3 = StackPointer + 2
ADDR LML Expr-3 = String Hello World
ADDR DEC Expr-3 1
CMD Import Load Expr-1 Anvil_System
CMD Import GetSymbol Expr-2 Expr-1 _ZN3Ice6System5printERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE
CMD Import Call Expr-2 Expr-3
CMD Kill 34
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, 34, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_ImportInvalidLib)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 0
CMD Import Load Expr-1 Nothing
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidImportLoadBadLibrary, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_CmdTest, Cmd_ImportValidLibraryBadSymbol)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 0
ADDR LRR Expr-2 = StackPointer + 1
CMD Import Load Expr-1 Anvil_System
CMD Import GetSymbol Expr-1 Expr-2 Thing
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidImportGetSymbolFailed, result);
    EXPECT_SUCC(bad, result);
}

