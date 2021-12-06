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
    EXPECT_FALSE_OR_DEBUG(bad, result);
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
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Kill_Max)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD NoOp
CMD Kill 1023
)");

    Result      output = vm.run();
    EXPECT_EQ_OR_LOG(bad, output, 1023, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Init_8_16)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 255 256
CMD Kill 1023
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 255, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 5, result);
    EXPECT_EQ_OR_LOG(bad, output, 1023, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Init_16_16)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 256 256
CMD Kill 1023
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 6, result);
    EXPECT_EQ_OR_LOG(bad, output, 1023, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}
TEST(FireVM_CmdTest, Cmd_Init_8_32)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 255 65536
CMD Kill 1023
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 255, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 65536, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 6, result);
    EXPECT_EQ_OR_LOG(bad, output, 1023, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}
TEST(FireVM_CmdTest, Cmd_Init_16_32)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init 256 65536
CMD Kill 1023
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, vm.machineState.global.size(), 256, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack.size(), 65536, result);
    EXPECT_EQ_OR_LOG(bad, vm.codeBlock.size(), 7, result);
    EXPECT_EQ_OR_LOG(bad, output, 1023, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
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


