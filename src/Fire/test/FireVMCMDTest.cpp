#include <gtest/gtest.h>

#include "../Ice/test/Utility.h"
#include "FireVM.h"
#include "Assembler.h"

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVMCMDTest, Run_CMD_NoOp)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    CMD NoOp
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    ASSERT_EQ(state.programCounter, 1);
    ASSERT_EQ(vmResult, 0);
}

TEST(FireVMCMDTest, Run_CMD_Kill)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    CMD Kill 34
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    ASSERT_EQ(state.programCounter, 1);
    ASSERT_EQ(vmResult, 34);
}

TEST(FireVMCMDTest, Run_CMD_Init)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    CMD Init 87 230
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    ASSERT_EQ(state.programCounter, 2);
    ASSERT_EQ(vmResult, 0);
    ASSERT_EQ(state.global.size(), 87);
    ASSERT_EQ(state.stack.size(), 230);
}

TEST(FireVMCMDTest, Run_CMD_Import)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    CMD Import testFile
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    ASSERT_EQ(state.programCounter, 5);
    ASSERT_EQ(vmResult, 0);
    // TODO
}

