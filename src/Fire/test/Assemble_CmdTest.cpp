#include <gtest/gtest.h>

#include "Assembler.h"
#include "test/BuildVM.h"
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
    BuildAssembler       as(result, R"(
CMD NoOp
)");

    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, as.assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        // NOTE: Assembler added CMD Init 0xFF 0xFFFF
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_CMD | Assembler::Cmd_NoOp, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_CmdTest, Cmd_Kill_128)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Kill 128
)");

    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, as.assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        // NOTE: Assembler added CMD Init 0xFF 0xFFFF
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], Assembler::Act_CMD | Assembler::Cmd_Kill | 128, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_CmdTest, Cmd_Init_8_16)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init 255 65535
)");

    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, as.assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 2, result);
    if (as.codeBlock.size() == 2)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_8_16 | 255, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[1], 65535, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_CmdTest, Cmd_Init_16_16)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init 256 65535
)");

    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, as.assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_16_16 , result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[1], 256, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], 65535, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


TEST(Assembler_CmdTest, Cmd_Init_8_32)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init 255 65536
)");

    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, as.assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 3, result);
    if (as.codeBlock.size() == 3)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_8_32 | 255, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[1], 1, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(Assembler_CmdTest, Cmd_Init_16_32)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init 256 65536
)");

    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, as.assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    if (as.codeBlock.size() == 4)
    {
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[0], Assembler::Act_CMD | Assembler::Cmd_Init | Assembler::Cmd_Init_Size_16_32 , result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[1], 256, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], 1, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


// Error Conditions

TEST(Assembler_CmdTest, Cmd_Invalid)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD ZZZ
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_NoOp_BadOperands)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD NoOp Invalid
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_Kill_NegativeValue)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Kill -10
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_Kill_OutOfBounds)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Kill 1024
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_Init_GlobalOverSize)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init 65536 65536
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_Init_StackOverSize)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init 255 4294967296
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_Init_NegativeGlobal)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init -255 255
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}

TEST(Assembler_CmdTest, Cmd_Init_NegativeStack)
{
    std::stringstream    result;
    BuildAssembler       as(result, R"(
CMD Init 255 -255
)");

    EXPECT_FALSE_OR_DEBUG(as.assembler.isOK(), result);
}


