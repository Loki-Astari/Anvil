#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(Assembler_CmdTest, InvalidCmd)
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

TEST(Assembler_CmdTest, CMD_NoOp_BadOperands)
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

TEST(Assembler_CmdTest, CMD_NoOp)
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

TEST(FireVM_CmdTest, CmdKill_NegativeValue)
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

TEST(FireVM_CmdTest, CmdKill_OutOfBounds)
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

TEST(FireVM_CmdTest, CmdKill_128)
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

