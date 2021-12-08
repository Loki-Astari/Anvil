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
 * Jump_Jp_AL)
 * Jump_Jp_EQ)
 * Jump_Jp_NE)
 * Jump_Jp_LT)
 * Jump_Jp_LE)
 * Jump_Jp_GT)
 * Jump_Jp_GE)
 * Jump_Call_Global
 * Jump_Call_FrameP
 * Jump_Call_This
 * Jump_Call_Extra
 * Jump_Call_StackP
 * Jump_Call_Expr1
 * Jump_Call_Expr2
 * Jump_Call_Expr3
 * Jump_Method_GLobal
 * Jump_Method_FrameP
 * Jump_Method_This
 * Jump_Method_Extra
 * Jump_Method_StackP
 * Jump_Method_Expr1
 * Jump_Method_Expr2
 * Jump_Method_Expr3
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


TEST(Assembler_JumpTest, Jump_Jp_AL)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
JUMP Jp AL Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        std::int32_t    jump = -3;
        Instruction     jumpHi = ((jump >> 16) & 0x3F);
        Instruction     jumpLo = jump & 0xFFFF;
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Jp | Assembler::Jump_Condition_AL | jumpHi, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], jumpLo, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Jp_EQ)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Jp EQ Dest
CMD NoOp
Dest: CMD NoOp

)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        std::int32_t    jump = 1;
        Instruction     jumpHi = ((jump >> 16) & 0x3F);
        Instruction     jumpLo = jump & 0xFFFF;
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Jp | Assembler::Jump_Condition_EQ | jumpHi, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], jumpLo, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Jp_NE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Jp NE Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        std::int32_t    jump = 0;
        Instruction     jumpHi = ((jump >> 16) & 0x3F);
        Instruction     jumpLo = jump & 0xFFFF;
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Jp | Assembler::Jump_Condition_NE | jumpHi, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], jumpLo, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Jp_LT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
CMD NoOp
JUMP Jp LT Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        std::int32_t    jump = -4;
        Instruction     jumpHi = ((jump >> 16) & 0x003F);
        Instruction     jumpLo = ((jump >>  0) & 0xFFFF);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], Assembler::Act_Jump | Assembler::Jump_Jp | Assembler::Jump_Condition_LT | jumpHi, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], jumpLo, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
    //0x1B3F    0x1B00
}

TEST(Assembler_JumpTest, Jump_Jp_LE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Jp LE Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        std::int32_t    jump = 0;
        Instruction     jumpHi = ((jump >> 16) & 0x3F);
        Instruction     jumpLo = jump & 0xFFFF;
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Jp | Assembler::Jump_Condition_LE | jumpHi, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], jumpLo, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Jp_GT)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest:
CMD NoOp
JUMP Jp GT Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        std::int32_t    jump = -3;
        Instruction     jumpHi = ((jump >> 16) & 0x3F);
        Instruction     jumpLo = jump & 0xFFFF;
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Jp | Assembler::Jump_Condition_GT | jumpHi, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], jumpLo, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Jp_GE)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: JUMP Jp GE Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        std::int32_t    jump = -2;
        Instruction     jumpHi = ((jump >> 16) & 0x3F);
        Instruction     jumpLo = jump & 0xFFFF;
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Jp | Assembler::Jump_Condition_GE | jumpHi, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], jumpLo, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Global)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
JUMP Call Global Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_Global, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_FrameP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call FramePointer Dest
CMD NoOp
Dest: CMD NoOp

)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 7, result);
    if (as.codeBlock.size() == 7)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_FramePointer, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 6, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_This)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call This Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_This, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 5, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
CMD NoOp
JUMP Call Extra Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 7, result);
    if (as.codeBlock.size() == 7)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_Extra, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[6], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_StackP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
JUMP Call StackPointer Dest
Dest: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_StackPointer, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 5, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest:
CMD NoOp
JUMP Call Expr-1 Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_Expr_1, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Expr2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: JUMP Call Expr-2 Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_Expr_2, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD Init 5 10
Dest: CMD NoOp
JUMP Call Expr-3 Dest
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 6, result);
    if (as.codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], Assembler::Act_Jump | Assembler::Jump_Call | Assembler::Jump_Reg_Expr_3, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[5], 2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_Global)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method Global 3
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_Global | 3, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_FrameP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method FramePointer 8

)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_FramePointer | 8, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_This)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method This 16
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_This | 16, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method Extra 18
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_Extra | 18, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_StackP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method StackPointer 56
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_StackPointer | 56, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method Expr-1 63
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_Expr_1 | 63, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_Expr2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method Expr-2 62
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_Expr_2 | 62, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
JUMP Method Expr-3 61
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Jump | Assembler::Jump_Method | Assembler::Jump_Reg_Expr_3 | 61, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}



// Error Conditions

TEST(Assembler_JumpTest, Jump_Ret_Invalid)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Return XX
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_JP_Invalid_Dest)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Jp AL
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_JP_Invalid_Label)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result);

    as.stable["Dest"] = 0x20'0002;
    as.run(false, result, bad, R"(
JUMP Jp AL Dest
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_JP_Valid_Label_Max)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result);

    as.stable["Dest"] = 0x20'0001;
    as.run(true, result, bad, R"(
JUMP Jp AL Dest
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_JP_Invalid_Flag)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Jp XX Dest
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Invalid)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call StackPointer
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_Invalid_Reg)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Method XX 12
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Method_Invalid_Dest)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Method StackPointer
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

TEST(Assembler_JumpTest, Jump_Call_BadLabel)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(false, result, bad, R"(
Dest: JUMP Call GE LabelThatDoesNotExist
)");

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

