#include <gtest/gtest.h>

#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

/*
 * Construct
 * AssembleEmptyFile
 * CheckLabelsWork
 * CheckCommentsAreIgnored
 * CheckCommandBlock
 * CheckOutputStream
 * ------
 * CheckEmptyLabelIsInvalid
 * InvalidCommand
 */

TEST(AssemblerTest, Construct)
{
    SymbolTable         stable;
    std::stringstream   result;
    Assembler           assembler(result, stable);
}

TEST(AssemblerTest, AssembleEmptyFile)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"()");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 2, result);
    EXPECT_EQ_OR_LOG(bad, as.stable.size(), 0, result);
    EXPECT_SUCC(bad, result);
}

TEST(AssemblerTest, CheckLabelsWork)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
L1:L2: L3: CMD NoOp
L4:  L5:L6: CMD NoOp

        L7:L8: L9:
		LA:)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 4, result);
    EXPECT_EQ_OR_LOG(bad, as.stable.size(), 10, result);
    if (as.stable.size() == 10)
    {
        EXPECT_EQ_OR_LOG(bad, as.stable["L1"], 2, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L2"], 2, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L3"], 2, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L4"], 3, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L5"], 3, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L6"], 3, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L7"], 4, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L8"], 4, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L9"], 4, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["LA"], 4, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(AssemblerTest, CheckCommentsAreIgnored)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
// Code startup
L1: CMD NoOp
L4:CMD NoOp

// Now We are finished.
CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);

    EXPECT_EQ_OR_LOG(bad, as.stable.size(), 2, result);
    if (as.stable.size() == 2)
    {
        EXPECT_EQ_OR_LOG(bad, as.stable["L1"], 2, result);
        EXPECT_EQ_OR_LOG(bad, as.stable["L4"], 3, result);
    }
    EXPECT_SUCC(bad, result);
}


TEST(AssemblerTest, CheckCommandBlock)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
CMD NoOp
CMD NoOp
CMD NoOp
)");

    EXPECT_EQ_OR_LOG(bad, as.codeBlock.size(), 5, result);
    if (as.codeBlock.size() == 5)
    {
        // NOTE: Assembler added CMD Init 0xFF 0xFFFF
        // TODO Add instructions other than NoOp
        //      To easy to accidentally have zero in here
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[2], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[3], 0, result);
        EXPECT_EQ_OR_LOG(bad, as.codeBlock[4], 0, result);
    }
    EXPECT_SUCC(bad, result);
}

TEST(AssemblerTest, CheckOutputStream)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler<std::stringstream> as(result, bad, R"(
CMD NoOp
CMD NoOp
CMD NoOp
)");

    std::string         output = as.codeBlock.str();
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
    EXPECT_SUCC(bad, result);
}

// --------------

TEST(AssemblerTest, CheckEmptyLabelIsInvalid)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
    :
)");

    EXPECT_FAIL(bad, "Invalid Input: Bad Label", result);
}

TEST(AssemblerTest, InvalidCommand)
{
    std::stringstream   result;
    bool                bad = false;
    BuildAssembler      as(result, bad, R"(
Cmd NoOp
)");

    EXPECT_FAIL(bad, "Invalid Input: >Cmd<  NoOp", result);
}


