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

TEST(FireVM_JumpTest, Jump_Call_LE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call LE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_LE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call LE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_LE >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_LE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call LE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_GE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call GE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_GE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call GE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_GE >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_GE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call GE Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_LT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call LT Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_LT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call LT Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_LT >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_GT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call GT Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_Call_GT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call GT Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_GT >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

// ------
TEST(FireVM_JumpTest, Jump_CallRet)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return AL
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_EQ_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return EQ
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_EQ_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return EQ
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_NE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return NE
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_NE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return NE
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return LE
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return LE
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_LE >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return LE
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return GE
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return GE
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_GE >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return GE
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return LT
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return LT
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_LT >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return GT
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Rel func
CMD Kill 255

func:
JUMP Return GT
CMD Kill 3
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_GT >> Assembler::Action_JumpFlag_Shift);
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_Via_Reg)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init  100 100
JUMP Call AL Mem Expr-2
CMD Kill 255

func:
CMD Kill 3

data:
)");

    vm.machineState.flagRegister = static_cast<std::byte>(Assembler::Jump_Condition_GT >> Assembler::Action_JumpFlag_Shift);
    vm.machineState.addressRegister[Expr2]  = (&vm.machineState.stack[0]) + vm.stable["data"];
    vm.machineState.addressRegister[Expr2]->init(CodeAddress(vm.stable["func"]));
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
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

TEST(FireVM_JumpTest, Jump_InvalidReturn)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Return AL
)");

    ++vm.machineState.addressRegister[StackP];
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidReturnState, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_InvalidCall)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call AL Mem Expr-1
CMD Kill 8
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidCallState, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_JumpTest, Jump_InvalidCall_Length)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
Code:  JUMP Call AL Mem Expr-3
)");

    // Make the instruction a bad type of Length.
    vm.codeBlock[vm.stable["Code"]] &= 0xFCFF;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidCallState, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


