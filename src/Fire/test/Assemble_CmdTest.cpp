#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(Assembler_CmdTest, InvalidCmd)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
CMD ZZZ
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE(assembler.isOK());
}

TEST(Assembler_CmdTest, CMD_NoOp)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_TRUE(assembler.isOK());

    EXPECT_EQ(codeBlock.size(), 1);
    ASSERT_EQ(codeBlock[0], 0);
}

