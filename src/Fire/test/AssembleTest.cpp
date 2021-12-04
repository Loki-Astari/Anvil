#include <gtest/gtest.h>

#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(AssemblerTest, Construct)
{
    SymbolTable         stable;
    std::stringstream   result;
    Assembler           assembler(result, stable);
}

TEST(AssemblerTest, AssembleEmptyFile)
{
    std::stringstream   result;
    std::stringstream   input(buildStream(R"()"));

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_TRUE(assembler.isOK());

    EXPECT_EQ(codeBlock.size(), 0);
    EXPECT_EQ(stable.size(), 0);
}

TEST(AssemblerTest, CheckLabelsWork)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
L1:L2: L3: CMD NoOp
L4:  L5:L6: CMD NoOp

        L7:L8: L9:
		LA:)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_TRUE(assembler.isOK());

    EXPECT_EQ(codeBlock.size(), 2);
    ASSERT_EQ(stable.size(), 10);

    EXPECT_EQ(stable["L1"], 0);
    EXPECT_EQ(stable["L2"], 0);
    EXPECT_EQ(stable["L3"], 0);
    EXPECT_EQ(stable["L4"], 1);
    EXPECT_EQ(stable["L5"], 1);
    EXPECT_EQ(stable["L6"], 1);
    EXPECT_EQ(stable["L7"], 2);
    EXPECT_EQ(stable["L8"], 2);
    EXPECT_EQ(stable["L9"], 2);
    EXPECT_EQ(stable["LA"], 2);
}

TEST(AssemblerTest, CheckCommentsAreIgnored)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
// Code startup
L1: CMD NoOp
L4:CMD NoOp

// Now We are finished.
CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_TRUE(assembler.isOK());

    EXPECT_EQ(codeBlock.size(), 3);

    ASSERT_EQ(stable.size(), 2);
    EXPECT_EQ(stable["L1"], 0);
    EXPECT_EQ(stable["L4"], 1);
}


TEST(AssemblerTest, CheckCommandBlock)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
CMD NoOp
CMD NoOp
CMD NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_TRUE(assembler.isOK());

    ASSERT_EQ(codeBlock.size(), 3);

    // TODO Add instructions other than NoOp
    //      To easy to accidentally have zero in here
    EXPECT_EQ(codeBlock[0], 0);
    EXPECT_EQ(codeBlock[1], 0);
    EXPECT_EQ(codeBlock[2], 0);
}

TEST(AssemblerTest, CheckOutputStream)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
CMD NoOp
CMD NoOp
CMD NoOp
)");

    SymbolTable         stable;
    std::ostringstream  codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_TRUE(assembler.isOK());

    std::string         output = codeBlock.str();
    ASSERT_EQ(output.size(), 6);    // 6 bytes

    // TODO Add instructions other than NoOp
    //      To easy to accidentally have zero in here
    EXPECT_EQ(output[0], 0);
    EXPECT_EQ(output[1], 0);
    EXPECT_EQ(output[2], 0);
    EXPECT_EQ(output[3], 0);
    EXPECT_EQ(output[4], 0);
    EXPECT_EQ(output[5], 0);
}

TEST(AssemblerTest, InvalidCommand)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
Cmd NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE(assembler.isOK());
}


