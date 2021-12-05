#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(Assembler_JumpTest, Invalid_Jump)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP XX AL Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Return_AL)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP Return AL
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 1, result);
    if (codeBlock.size() == 1)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_Jump | Assembler::JumpType_Return | Assembler::Jump_Condition_AL, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_EQ)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP Return EQ
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 1, result);
    if (codeBlock.size() == 1)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_Jump | Assembler::JumpType_Return | Assembler::Jump_Condition_EQ, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_NE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP Return NE
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 1, result);
    if (codeBlock.size() == 1)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_Jump | Assembler::JumpType_Return | Assembler::Jump_Condition_NE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_LT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP Return LT
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 1, result);
    if (codeBlock.size() == 1)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_Jump | Assembler::JumpType_Return | Assembler::Jump_Condition_LT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_LE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP Return LE
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 1, result);
    if (codeBlock.size() == 1)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_Jump | Assembler::JumpType_Return | Assembler::Jump_Condition_LE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_GT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP Return GT
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 1, result);
    if (codeBlock.size() == 1)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_Jump | Assembler::JumpType_Return | Assembler::Jump_Condition_GT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Return_GE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
JUMP Return GE
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 1, result);
    if (codeBlock.size() == 1)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_Jump | Assembler::JumpType_Return | Assembler::Jump_Condition_GE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


TEST(Assembler_JumpTest, Jump_Call_Rel_AL)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: CMD NoOp
JUMP Call AL Rel Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    if (codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_AL, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], -3, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_EQ)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call EQ Rel Dest
CMD NoOp
Dest: CMD NoOp

)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 5, result);
    if (codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_EQ, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 1, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_NE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call NE Rel Dest
Dest: CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    if (codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_NE, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_LT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: CMD NoOp
CMD NoOp
JUMP Call LT Rel Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 5, result);
    if (codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_LT, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[4], -4, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_LE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call LE Rel Dest
Dest: CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    if (codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_LE, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_GT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest:
CMD NoOp
JUMP Call GT Rel Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    if (codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_GT, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], -3, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Rel_GE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Rel Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 3, result);
    if (codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Rel | Assembler::Jump_Condition_GE, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], -2, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_AL)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: CMD NoOp
JUMP Call AL Abs Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 5, result);
    if (codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_AL, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[4], 1, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_EQ)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call EQ Abs Dest
CMD NoOp
Dest: CMD NoOp

)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 6, result);
    if (codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_EQ, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 5, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_NE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call NE Abs Dest
Dest: CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 5, result);
    if (codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_NE, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 4, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_LT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: CMD NoOp
CMD NoOp
JUMP Call LT Abs Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 6, result);
    if (codeBlock.size() == 6)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_LT, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[4], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[5], 1, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_LE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call LE Abs Dest
Dest: CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 5, result);
    if (codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_LE, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 4, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_GT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest:
CMD NoOp
JUMP Call GT Abs Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 5, result);
    if (codeBlock.size() == 5)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_GT, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[4], 1, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Abs_GE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Abs Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    if (codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Abs | Assembler::Jump_Condition_GE, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 1, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_AL)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: CMD NoOp
JUMP Call AL Mem Global
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 3, result);
    if (codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Global | Assembler::Jump_Condition_AL, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_EQ)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call EQ Mem FramePointer
CMD NoOp
Dest: CMD NoOp

)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    if (codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_FramePointer | Assembler::Jump_Condition_EQ, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_NE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call NE Mem This
Dest: CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 3, result);
    if (codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_This | Assembler::Jump_Condition_NE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_LT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: CMD NoOp
CMD NoOp
JUMP Call LT Mem Extra
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    if (codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Extra | Assembler::Jump_Condition_LT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_LE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call LE Mem StackPointer
Dest: CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 3, result);
    if (codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_StackPointer | Assembler::Jump_Condition_LE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_GT)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest:
CMD NoOp
JUMP Call GT Mem Expr-1
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 3, result);
    if (codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Expr_1 | Assembler::Jump_Condition_GT, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_GE)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Mem Expr-2
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 2, result);
    if (codeBlock.size() == 2)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Expr_2 | Assembler::Jump_Condition_GE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_Mem_Expr3)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Mem Expr-3
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);
    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 2, result);
    if (codeBlock.size() == 2)
    {
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], Assembler::Act_Jump | Assembler::JumpType_Call | Assembler::JumpSize_Mem | Assembler::Jump_Reg_Expr_3 | Assembler::Jump_Condition_GE, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_JumpTest, Jump_Call_NoDestiantion)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Abs
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Call_BadLabel)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Abs LabelThatDoesNotExist
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Call_BadJump)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Spoon
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Call_BadLength)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Ping
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Call_BadRegister)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call GE Mem Plop
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Return_Extra)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Return AL Dest
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Call_Extra)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
Dest: JUMP Call AL Rel Dest XX
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_JumpTest, Jump_Call_BadRelative)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp                // TODO Remove once we have CMD Init
JUMP Call AL Rel Dest
)");

    SymbolTable         stable;
    stable["Dest"] = 0x00FFFFFF;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

