#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

/*
 * Cmd_NoOp
 * Cmd_Kill
 * Cmd_Init
 * -----------
 * Cmd_Invalid
 * Cmd_NoOp_BadOperands
 * Cmd_Kill_NegativeValue
 * Cmd_Kill_OutOfBounds
 * Cmd_Init_GlobalOverSize
 * Cmd_Init_StackOverSize
 * Cmd_Init_NegativeGlobal
 * Cmd_Init_NegativeStack
 */


TEST(Assembler_CmdTest, Cmd_NoOp)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp
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
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_NoOp, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Kill_128)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Kill 128
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
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Kill | 128, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Init_8_16)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 255 65535
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
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_8_16 | 255, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], 65535, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Init_16_16)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 256 65535
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
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_16_16 , result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], 256, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 65535, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


TEST(FireVM_CmdTest, Cmd_Init_8_32)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 255 65536
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
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_8_32 | 255, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], 1, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(FireVM_CmdTest, Cmd_Init_16_32)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 256 65536
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
        EXPECT_EQ_OR_LOG(bad, codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_16_32 , result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[1], 256, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 1, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

// Error Conditions

TEST(Assembler_CmdTest, Cmd_Invalid)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD ZZZ
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_NoOp_BadOperands)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp Invalid
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(FireVM_CmdTest, Cmd_Kill_NegativeValue)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Kill -10
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(FireVM_CmdTest, Cmd_Kill_OutOfBounds)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Kill 1024
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(FireVM_CmdTest, Cmd_Init_GlobalOverSize)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 65536 65536
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(FireVM_CmdTest, Cmd_Init_StackOverSize)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 255 4294967296
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(FireVM_CmdTest, Cmd_Init_NegativeGlobal)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init -255 255
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
    std::cerr << "Result:\n=============\n" << result.rdbuf() << "\n==============\n";
}

TEST(FireVM_CmdTest, Cmd_Init_NegativeStack)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 255 -255
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
    std::cerr << "Result:\n=============\n" << result.rdbuf() << "\n==============\n";
}


