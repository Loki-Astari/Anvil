#include <gtest/gtest.h>

#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

/*
 * LRR_Global_FrameP
 * LRR_This_Extra
 * LRR_StackP_Expr1
 * LRR_Expr2_Expr3
 * LRR_Exp3_Global_WithOffset
 * LRR_Expr2_FramePointer_WithOffset
 * LRR_Expr1_This_WithOffset
 * LRR_StackPointer_Extra_WithOffset
 * LRP_Global_FrameP
 * LRP_This_Extra
 * LRP_StackP_Expr1
 * LRP_Expr2_Expr3
 * LRP_Exp3_Global_WithOffset
 * LRP_Expr2_FramePointer_WithOffset
 * LRP_Expr1_This_WithOffset
 * LRP_StackPointer_Extra_WithOffset
 * LMR_Global_FrameP
 * LMR_This_Extra
 * LMR_StackP_Expr1
 * LMR_Expr2_Expr3
 * LMR_Exp3_Global_WithOffset
 * LMR_Expr2_FramePointer_WithOffset
 * LMR_Expr1_This_WithOffset
 * LMR_StackPointer_Extra_WithOffset
 * LMP_Global_FrameP
 * LMP_This_Extra
 * LMP_StackP_Expr1
 * LMP_Expr2_Expr3
 * LMP_Exp3_Global_WithOffset
 * LMP_Expr2_FramePointer_WithOffset
 * LMP_Expr1_This_WithOffset
 * LMP_StackPointer_Extra_WithOffset
 * INC_Global
 * INC_FramePointer
 * INC_This
 * INC_Extra
 * INC_StackPointer
 * INC_Expr1
 * INC_Expr2
 * INC_Expr3
 * DEC_Global
 * DEC_FramePointer
 * DEC_This
 * DEC_Extra
 * DEC_StackPointer
 * DEC_Expr1
 * DEC_Expr2
 * DEC_Expr3
 * LML_Gloabl_CodeAddr
 * LML_Gloabl_DataFrame
 * LML_Gloabl_Int
 * LML_Gloabl_String
 * ----------
*/

using namespace ThorsAnvil::Anvil::Fire;

TEST(Assembler_AddrTest, LRR_Global_FrameP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Global = FramePointer + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRR_This_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR This = Extra + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_This << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRR_StackP_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR StackPointer = Expr-1 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRR_Expr2_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-2 = Expr-3 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRR_Exp3_Global_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-3 = Global + 5
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 5, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRR_Expr2_FramePointer_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-2 = FramePointer + 245
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 245, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRR_Expr1_This_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-1 = This + 1024
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_This << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 1024, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRR_StackPointer_Extra_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR StackPointer = Extra + 8675
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 8675, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_Global_FrameP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Global = FramePointer + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_This_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP This = Extra + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_This << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_StackP_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP StackPointer = Expr-1 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_Expr2_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-2 = Expr-3 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_Exp3_Global_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-3 = Global + 5
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 5, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_Expr2_FramePointer_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-2 = FramePointer + 245
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 245, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_Expr1_This_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-1 = This + 1024
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_This << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 1024, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LRP_StackPointer_Extra_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP StackPointer = Extra + 8675
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LRP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 8675, result);
    }
    EXPECT_SUCC(bad, result);
}
TEST(Assembler_AddrTest, LMR_Global_FrameP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Global = FramePointer + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMR_This_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR This = Extra + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_This << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMR_StackP_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR StackPointer = Expr-1 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMR_Expr2_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-2 = Expr-3 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMR_Exp3_Global_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-3 = Global + 5
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 5, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMR_Expr2_FramePointer_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-2 = FramePointer + 245
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 245, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMR_Expr1_This_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-1 = This + 1024
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_This << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 1024, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMR_StackPointer_Extra_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR StackPointer = Extra + 8675
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMR | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 8675, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_Global_FrameP)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Global = FramePointer + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_This_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP This = Extra + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_This << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_StackP_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP StackPointer = Expr-1 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_Expr2_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-2 = Expr-3 + 0
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Zero
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg2_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_Exp3_Global_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-3 = Global + 5
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 5, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_Expr2_FramePointer_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-2 = FramePointer + 245
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 245, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_Expr1_This_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-1 = This + 1024
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_This << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 1024, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LMP_StackPointer_Extra_WithOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP StackPointer = Extra + 8675
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LMP | Assembler::Addr_AddType_Value
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift)
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg2_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 8675, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, INC_Global)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC Global 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, INC_FramePointer)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC FramePointer 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, INC_This)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC This 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_This << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, INC_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC Extra 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, INC_StackPointer)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC StackPointer 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, INC_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC Expr-1 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, INC_Expr2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC Expr-2 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}


TEST(Assembler_AddrTest, INC_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC Expr-3 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_INC | 1
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, DEC_Global)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC Global 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, DEC_FramePointer)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC FramePointer 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_FramePointer << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, DEC_This)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC This 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_This << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, DEC_Extra)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC Extra 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_Extra << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, DEC_StackPointer)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC StackPointer 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_StackPointer << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, DEC_Expr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC Expr-1 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_Expr_1 << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, DEC_Expr2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC Expr-2 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_Expr_2 << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}


TEST(Assembler_AddrTest, DEC_Expr3)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR DEC Expr-3 1
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_DEC | 1
                                                | (Assembler::Reg_Expr_3 << Assembler::Addr_Reg1_Shift), result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LML_Gloabl_CodeAddr)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Global = CodeAddress Stuff
CMD NoOp
Stuff: CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 7, result);
    if (as.codeBlock.size() == 7)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LML | Assembler::Addr_Literal_CodeAddr
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 6, result);
    }
    EXPECT_SUCC(bad, result);
}


TEST(Assembler_AddrTest, LML_Gloabl_DataFrame)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Global = DataFrame 2
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LML | Assembler::Addr_Literal_DataFrame
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 2, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LML_Gloabl_Int)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Global = Int 2
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LML | Assembler::Addr_Literal_Int
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 2, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(Assembler_AddrTest, LML_Gloabl_String)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Global = String Stuff Is Good
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 11, result);
    if (as.codeBlock.size() == 11)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_Addr | Assembler::Addr_LML | Assembler::Addr_Literal_String
                                                | (Assembler::Reg_Global << Assembler::Addr_Reg1_Shift), result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 13, result);
        EXPECT_EQ_OR_LOG(bad, std::string("Stuff Is Good"), std::string(reinterpret_cast<char const*>(&as.codeBlock[4]), 13), result);
    }
    EXPECT_SUCC(bad, result);
}

// -----------------------

TEST(Assembler_AddrTest, Invalid_Action)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR XX Expr-2 = Expr-1 + 0
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR >", result);
}

TEST(Assembler_AddrTest, Invalid_INCRegister)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC Expr-5 12
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR INC >", result);
}

TEST(Assembler_AddrTest, Invalid_INCOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR INC Expr-2 64
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR INC Expr-2 >", result);
}

TEST(Assembler_AddrTest, Invalid_LRRRegister1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-5 = Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRR >", result);
}

TEST(Assembler_AddrTest, Invalid_LRPRegister1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-5 = Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRP >", result);
}

TEST(Assembler_AddrTest, Invalid_LMRRegister1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-5 = Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMR >", result);
}

TEST(Assembler_AddrTest, Invalid_LMPRegister1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-5 = Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMP >", result);
}

TEST(Assembler_AddrTest, Invalid_LMLRegister1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Expr-5 = DataFrame 2
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LML >", result);
}

TEST(Assembler_AddrTest, Invalid_LRRNotEqual)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-3 % Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRR Expr-3 >", result);
}

TEST(Assembler_AddrTest, Invalid_LRPNotEqual)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-3 # Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRP Expr-3 >", result);
}

TEST(Assembler_AddrTest, Invalid_LMRNotEqual)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-3 & Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMR Expr-3 >", result);
}

TEST(Assembler_AddrTest, Invalid_LMPNotEqual)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-3 ( Expr-2 + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMP Expr-3 >", result);
}

TEST(Assembler_AddrTest, Invalid_LMLNotEqual)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Expr-3 X DataFrame 2
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LML Expr-3 >", result);
}

TEST(Assembler_AddrTest, Invalid_LRRRegister2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-3 = Expr-X + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRR Expr-3 = >", result);
}

TEST(Assembler_AddrTest, Invalid_LRPRegister2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-3 = Expr-X + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRP Expr-3 = >", result);
}

TEST(Assembler_AddrTest, Invalid_LMRRegister2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-3 = Expr-X + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMR Expr-3 = >", result);
}

TEST(Assembler_AddrTest, Invalid_LMPRegister2)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-3 = Expr-X + 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMP Expr-3 = >", result);
}

TEST(Assembler_AddrTest, Invalid_LRROffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-3 = StackPointer + F15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRR Expr-3 = StackPointer + >", result);
}

TEST(Assembler_AddrTest, Invalid_LRPOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRP Expr-3 = StackPointer + F15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRP Expr-3 = StackPointer + >", result);
}

TEST(Assembler_AddrTest, Invalid_LMROffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMR Expr-3 = StackPointer + F15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMR Expr-3 = StackPointer + >", result);
}

TEST(Assembler_AddrTest, Invalid_LMPOffset)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LMP Expr-3 = StackPointer + F15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LMP Expr-3 = StackPointer + >", result);
}

TEST(Assembler_AddrTest, Invalid_PlusSign)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LRR Expr-3 = StackPointer - 15
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LRR Expr-3 = StackPointer >", result);
}



TEST(Assembler_AddrTest, Invalid_LMLType)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Expr-3 = FGHH 111
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LML Expr-3 = >", result);
}

TEST(Assembler_AddrTest, Invalid_LMLCodeLiteral)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
ADDR LML Expr-3 = CodeAddress Shamoo
)");

    EXPECT_FAIL(bad, "Invalid Input: ADDR LML Expr-3 = CodeAddress >", result);
}


