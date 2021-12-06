#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

/*
 * Jump_Call)
 * -----------
 * Jump_Invalid)
 */

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVM_JumpTest, Jump_Call)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_EQ_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call EQ Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_NE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call NE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_NE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call NE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_EQ_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call EQ Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

// Error Conditions

TEST(FireVM_JumpTest, Jump_Invalid)
{
    VMState     machineState;
    CodeBlock   machineProgram{Assembler::InvalidJump};

    FireVM      vm(machineState, machineProgram);
    Result      result = vm.run();

    EXPECT_EQ(result, FireVM::haltInvalidJump);
}


