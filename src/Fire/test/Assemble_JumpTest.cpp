#include <gtest/gtest.h>

#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

/*
 * Jump_Return_AL)
 * Jump_Return_EQ)
 * Jump_Return_NE)
 * Jump_Return_LT)
 * Jump_Return_LE)
 * Jump_Return_GT)
 * Jump_Return_GE)
 * Jump_Call_Rel_AL)
 * Jump_Call_Rel_EQ)
 * Jump_Call_Rel_NE)
 * Jump_Call_Rel_LT)
 * Jump_Call_Rel_LE)
 * Jump_Call_Rel_GT)
 * Jump_Call_Rel_GE)
 * Jump_Call_Abs_AL)
 * Jump_Call_Abs_EQ)
 * Jump_Call_Abs_NE)
 * Jump_Call_Abs_LT)
 * Jump_Call_Abs_LE)
 * Jump_Call_Abs_GT)
 * Jump_Call_Abs_GE)
 * Jump_Call_Mem_AL)
 * Jump_Call_Mem_EQ)
 * Jump_Call_Mem_NE)
 * Jump_Call_Mem_LT)
 * Jump_Call_Mem_LE)
 * Jump_Call_Mem_GT)
 * Jump_Call_Mem_GE)
 * Jump_Call_Mem_Expr3)
 * ------------------
 * Jump_Invalid
 * Jump_Invalid_Call_Condition
 * Jump_Invalid_Return_Condition
 * Jump_Return_Extra)
 * Jump_Call_Extra)
 * Jump_Call_NoDestiantion)
 * Jump_Call_BadLabel)
 * Jump_Call_BadJump)
 * Jump_Call_BadLength)
 * Jump_Call_BadRegister)
 * Jump_Call_BadRelative)
*/

using namespace ThorsAnvil::Anvil::Fire;

TEST(Assembler_JumpTest, Jump_Return_AL)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Return AL
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Return | Assembler::Jump_Condition_AL, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_EQ)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Return EQ
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Return | Assembler::Jump_Condition_EQ, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_NE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Return NE
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Return | Assembler::Jump_Condition_NE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_LT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Return LT
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Return | Assembler::Jump_Condition_LT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_LE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Return LE
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Return | Assembler::Jump_Condition_LE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_GT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Return GT
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Return | Assembler::Jump_Condition_GT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_GE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Return GE
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Return | Assembler::Jump_Condition_GE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


TEST(Assembler_JumpTest, Jump_Call_Rel_AL)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
JUMP Call AL Rel Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_AL, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], static_cast<Instruction>(-3), result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_EQ)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call EQ Rel Dest
CMD NoOp
Dest: CMD NoOp

)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_EQ, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 1, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_NE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call NE Rel Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_NE, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_LT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
CMD NoOp
JUMP Call LT Rel Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_LT, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], static_cast<Instruction>(-4), result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_LE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call LE Rel Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_LE, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_GT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest:
CMD NoOp
JUMP Call GT Rel Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_GT, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], static_cast<Instruction>(-3), result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_GE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: JUMP Call GE Rel Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_GE, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], static_cast<Instruction>(-2), result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_AL)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
JUMP Call AL Abs Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_AL, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_EQ)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call EQ Abs Dest
CMD NoOp
Dest: CMD NoOp

)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 7, result);
    if (as.codeBlock.size() == 7)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_EQ, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 6, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_NE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call NE Abs Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_NE, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 5, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_LT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
CMD NoOp
JUMP Call LT Abs Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 7, result);
    if (as.codeBlock.size() == 7)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_LT, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[6], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_LE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call LE Abs Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_LE, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 5, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_GT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest:
CMD NoOp
JUMP Call GT Abs Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_GT, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_GE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: JUMP Call GE Abs Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_GE, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_AL)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
JUMP Call AL Mem Global
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Global | Assembler::Jump_Condition_AL, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_EQ)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call EQ Mem FramePointer
CMD NoOp
Dest: CMD NoOp

)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_FramePointer | Assembler::Jump_Condition_EQ, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_NE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call NE Mem This
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_This | Assembler::Jump_Condition_NE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_LT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
CMD NoOp
JUMP Call LT Mem Extra
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Extra | Assembler::Jump_Condition_LT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_LE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call LE Mem StackPointer
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_StackPointer | Assembler::Jump_Condition_LE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_GT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest:
CMD NoOp
JUMP Call GT Mem Expr-1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Expr_1 | Assembler::Jump_Condition_GT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_GE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: JUMP Call GE Mem Expr-2
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Expr_2 | Assembler::Jump_Condition_GE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: JUMP Call GE Mem Expr-3
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Expr_3 | Assembler::Jump_Condition_GE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


// Error Conditions

TEST(Assembler_JumpTest, Jump_Invalid)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP ZZ AL Dest
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Invalid_Call_Condition)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call XX Dest
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Invalid_Return_Condition)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Return XX
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Return AL Dest
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call AL Rel Dest XX
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Invalid_Jump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
JUMP XX AL Dest
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_NoDestiantion)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call GE Abs
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_BadLabel)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call GE Abs LabelThatDoesNotExist
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_BadJump)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Spoon
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_BadLength)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call GE Ping
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_BadRegister)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call GE Mem Plop
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_BadRelative)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
JUMP Call AL Rel Dest
)");

    as.stable["Dest"] = 0x00FFFFFF;

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

