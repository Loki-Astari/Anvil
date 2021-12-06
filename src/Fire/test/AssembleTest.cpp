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
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 2, result);
    EXPECT_EQ_OR_LOG(bad, stable.size(), 0, result);
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(AssemblerTest, CheckLabelsWork)
{
    std::stringstream    result;
    std::istringstream   input(R"(
L1:L2: L3: CMD NoOp
L4:  L5:L6: CMD NoOp

        L7:L8: L9:
		LA:)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_TRUE(assembler.isOK());

    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 4, result);
    EXPECT_EQ_OR_LOG(bad, stable.size(), 10, result);
    if (stable.size() == 10)
    {
        EXPECT_EQ_OR_LOG(bad, stable["L1"], 2, result);
        EXPECT_EQ_OR_LOG(bad, stable["L2"], 2, result);
        EXPECT_EQ_OR_LOG(bad, stable["L3"], 2, result);
        EXPECT_EQ_OR_LOG(bad, stable["L4"], 3, result);
        EXPECT_EQ_OR_LOG(bad, stable["L5"], 3, result);
        EXPECT_EQ_OR_LOG(bad, stable["L6"], 3, result);
        EXPECT_EQ_OR_LOG(bad, stable["L7"], 4, result);
        EXPECT_EQ_OR_LOG(bad, stable["L8"], 4, result);
        EXPECT_EQ_OR_LOG(bad, stable["L9"], 4, result);
        EXPECT_EQ_OR_LOG(bad, stable["LA"], 4, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(AssemblerTest, CheckEmptyLabelIsInvalid)
{
    std::stringstream    result;
    std::istringstream   input(R"(
    :
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}

TEST(AssemblerTest, CheckCommentsAreIgnored)
{
    std::stringstream    result;
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
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);

    EXPECT_EQ_OR_LOG(bad, codeBlock.size(), 5, result);

    EXPECT_EQ_OR_LOG(bad, stable.size(), 2, result);
    if (stable.size() == 2)
    {
        EXPECT_EQ_OR_LOG(bad, stable["L1"], 2, result);
        EXPECT_EQ_OR_LOG(bad, stable["L4"], 3, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}


TEST(AssemblerTest, CheckCommandBlock)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp
CMD NoOp
CMD NoOp
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
        // NOTE: Assembler added CMD Init 0xFF 0xFFFF
        // TODO Add instructions other than NoOp
        //      To easy to accidentally have zero in here
        EXPECT_EQ_OR_LOG(bad, codeBlock[2], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, codeBlock[4], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(AssemblerTest, CheckOutputStream)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD NoOp
CMD NoOp
CMD NoOp
)");

    SymbolTable         stable;
    std::ostringstream  codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, assembler.isOK(), true, result);

    std::string         output = codeBlock.str();
    EXPECT_EQ_OR_LOG(bad, output.size(), 10, result);    // 6 bytes

    if (output.size() == 6)
    {
        // NOTE: Assembler added CMD Init 0xFF 0xFFFF
        // TODO Add instructions other than NoOp
        //      To easy to accidentally have zero in here
        EXPECT_EQ_OR_LOG(bad, output[4], 0, result);
        EXPECT_EQ_OR_LOG(bad, output[5], 0, result);
        EXPECT_EQ_OR_LOG(bad, output[6], 0, result);
        EXPECT_EQ_OR_LOG(bad, output[7], 0, result);
        EXPECT_EQ_OR_LOG(bad, output[8], 0, result);
        EXPECT_EQ_OR_LOG(bad, output[9], 0, result);
    }
    EXPECT_FALSE_OR_DEBUG(bad, result);
}

TEST(AssemblerTest, InvalidCommand)
{
    std::stringstream    result;
    std::istringstream   input(R"(
Cmd NoOp
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);

    assembler.assemble(input, codeBlock);
    EXPECT_FALSE_OR_DEBUG(assembler.isOK(), result);
}


