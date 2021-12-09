#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

/*
 * Jump_Call
 * Jump_Jp_EQ_NotJump
 * Jump_Jp_EQ_Jump
 * Jump_Jp_NE_NotJump
 * Jump_Jp_NE_WithLess_Jump
 * Jump_Jp_NE_WithGreat_Jump
 * Jump_Jp_LE_NotJump
 * Jump_Jp_LE_Jump
 * Jump_Jp_LE_Equal_Jump
 * Jump_Jp_GE_NotJump
 * Jump_Jp_GE_Jump
 * Jump_Jp_GE_Equal_Jump
 * Jump_Jp_LT_NotJump
 * Jump_Jp_LT_Jump
 * Jump_Jp_GT_NotJump
 * Jump_Jp_GT_Jump
 * Jump_CallRet
 * Jump_CallRet_EQ_NotJump
 * Jump_CallRet_EQ_Jump
 * Jump_CallRet_NE_NotJump
 * Jump_CallRet_NELess_Jump
 * Jump_CallRet_NEGreat_Jump
 * Jump_CallRet_LE_NotJump
 * Jump_CallRet_LE_Jump
 * Jump_CallRet_LE_Equal_Jump
 * Jump_CallRet_GE_NotJump
 * Jump_CallRet_GE_Jump
 * Jump_CallRet_GE_Equal_Jump
 * Jump_CallRet_LT_NotJump
 * Jump_CallRet_LT_Jump
 * Jump_CallRet_GT_NotJump
 * Jump_CallRet_GT_Jump
 * Jump_CallRet_Via_Reg
 * Jump_Invalid
 * Jump_InvalidReturn
 * Jump_InvalidCall
 */

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVM_JumpTest, Jump_Call)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_EQ_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp EQ func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_EQ_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp EQ func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_NE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp NE func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_NE_WithLess_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp NE func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_LT >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_NE_WithGreat_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp NE func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_GT >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_LE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp LE func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_LE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp LE func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_LE >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_LE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp LE func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_GE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp GE func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_GE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp GE func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_GE >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_GE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp GE func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_LT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp LT func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_LT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp LT func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_LT >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_GT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp GT func
CMD Kill 3

func:
CMD Kill 255
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_Jp_GT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Jp GT func
CMD Kill 3

func:
CMD Kill 255
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_GT >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

// ------
TEST(FireVM_JumpTest, Jump_CallRet)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return AL
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_EQ_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return EQ
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_EQ_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return EQ
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_NE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return NE
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_NELess_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return NE
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_LT >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_NEGreat_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return NE
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_GE >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return LE
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return LE
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_LE >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return LE
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GE_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return GE
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GE_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return GE
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_GE >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GE_Equal_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return GE
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_EQ >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return LT
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_LT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return LT
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_LT >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GT_NotJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return GT
CMD Kill 3
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_GT_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Call StackPointer func
CMD Kill 255

func:
JUMP Return GT
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_GT >> Assembler::Action_JumpFlag_Shift;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 255, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_CallRet_Via_Reg)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Init  100 100
JUMP Method Expr-2 2
CMD Kill 255

func:
CMD Kill 3
)");

    vm.machineState.flagRegister = Assembler::Jump_Condition_GT >> Assembler::Action_JumpFlag_Shift;
    vm.machineState.addressRegister[Expr2]  = &vm.machineState.stack[25];
    (vm.machineState.addressRegister[Expr2] + 2)->init(CodeAddress(vm.stable["func"]));
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 3, result);
    EXPECT_SUCC(bad, result);
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
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_JumpTest, Jump_InvalidCall)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
JUMP Method Expr-1 0
CMD Kill 8
)");

    // Note Expr-1 + 0 does not point at a memory location with an address in it.
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidMethod, result);
    EXPECT_SUCC(bad, result);
}

