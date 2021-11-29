#include <gtest/gtest.h>

#include "../Ice/test/Utility.h"
#include "FireVM.h"
#include "Assembler.h"

using namespace ThorsAnvil::Anvil::Fire;

// Set Stack
TEST(FireVMLoadTest, LoadStackSetGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack = Global + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.global.data() + 12);
}

TEST(FireVMLoadTest, LoadStackSetStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack = Stack + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.stack.data() + 12);
}

TEST(FireVMLoadTest, LoadStackSetThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Stack = This + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.stack.data() + 12);
}

TEST(FireVMLoadTest, LoadStackSetExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Stack = Expr + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.stack.data() + 12);
}
// Set This
TEST(FireVMLoadTest, LoadThisSetGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Global + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.global.data() + 12);
}

TEST(FireVMLoadTest, LoadThisSetStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.stack.data() + 12);
}

TEST(FireVMLoadTest, LoadThisSetThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load This = This + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.stack.data() + 12);
}

TEST(FireVMLoadTest, LoadThisSetExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load This = Expr + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.stack.data() + 12);
}
// Set Expr
TEST(FireVMLoadTest, LoadExprSetGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Global + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.global.data() + 12);
}

TEST(FireVMLoadTest, LoadExprSetStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.stack.data() + 12);
}

TEST(FireVMLoadTest, LoadExprSetThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Expr = This + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.stack.data() + 12);
}

TEST(FireVMLoadTest, LoadExprSetExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Expr = Expr + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.stack.data() + 12);
}


// Read Stack
TEST(FireVMLoadTest, LoadStackReadGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack * Global + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.global.resize(20);
    state.global[12] = Frame{};
    MemoryLocation* regPtr = state.global[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}

TEST(FireVMLoadTest, LoadStackReadStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack * Stack + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}

TEST(FireVMLoadTest, LoadStackReadThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Stack * This + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}

TEST(FireVMLoadTest, LoadStackReadExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Stack * Expr + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}
// Read This
TEST(FireVMLoadTest, LoadThisReadGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This * Global + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.global.resize(20);
    state.global[12] = Frame{};
    MemoryLocation* regPtr = state.global[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}

TEST(FireVMLoadTest, LoadThisReadStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This * Stack + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}

TEST(FireVMLoadTest, LoadThisReadThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load This * This + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}

TEST(FireVMLoadTest, LoadThisReadExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load This * Expr + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}
// Read Expr
TEST(FireVMLoadTest, LoadExprReadGloabl1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr * Global + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.global.resize(20);
    state.global[12] = Frame{};
    MemoryLocation* regPtr = state.global[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

TEST(FireVMLoadTest, LoadExprReadStack1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr * Stack + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 3);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

TEST(FireVMLoadTest, LoadExprReadThis1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Expr * This + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

TEST(FireVMLoadTest, LoadExprReadExpr1Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Expr * Expr + 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(20);
    state.stack[12] = Frame{};
    MemoryLocation* regPtr = state.stack[12].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

// Set Stack
TEST(FireVMLoadTest, LoadStackSetGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack = Global + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.global.data() + 345);
}

TEST(FireVMLoadTest, LoadStackSetStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack = Stack + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.stack.data() + 345);
}

TEST(FireVMLoadTest, LoadStackSetThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Stack = This + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.stack.data() + 345);
}

TEST(FireVMLoadTest, LoadStackSetExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Stack = Expr + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], state.stack.data() + 345);
}
// Set This
TEST(FireVMLoadTest, LoadThisSetGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Global + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.global.data() + 345);
}

TEST(FireVMLoadTest, LoadThisSetStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.stack.data() + 345);
}

TEST(FireVMLoadTest, LoadThisSetThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load This = This + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.stack.data() + 345);
}

TEST(FireVMLoadTest, LoadThisSetExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load This = Expr + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], state.stack.data() + 345);
}
// Set Expr
TEST(FireVMLoadTest, LoadExprSetGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Global + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.global.data() + 345);
}

TEST(FireVMLoadTest, LoadExprSetStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.stack.data() + 345);
}

TEST(FireVMLoadTest, LoadExprSetThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Expr = This + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.stack.data() + 345);
}

TEST(FireVMLoadTest, LoadExprSetExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Expr = Expr + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], state.stack.data() + 345);
}


// Read Stack
TEST(FireVMLoadTest, LoadStackReadGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack * Global + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.global.resize(400);
    state.global[345] = Frame{};
    MemoryLocation* regPtr = state.global[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}

TEST(FireVMLoadTest, LoadStackReadStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Stack * Stack + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}

TEST(FireVMLoadTest, LoadStackReadThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Stack * This + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}

TEST(FireVMLoadTest, LoadStackReadExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Stack * Expr + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], regPtr);
}
// Read This
TEST(FireVMLoadTest, LoadThisReadGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This * Global + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    state.global.resize(400);
    state.global[345] = Frame{};
    MemoryLocation* regPtr = state.global[345].getObject();

    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}

TEST(FireVMLoadTest, LoadThisReadStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This * Stack + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}

TEST(FireVMLoadTest, LoadThisReadThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load This * This + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}

TEST(FireVMLoadTest, LoadThisReadExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load This * Expr + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], regPtr);
}
// Read Expr
TEST(FireVMLoadTest, LoadExprReadGloabl2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr * Global + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.global.resize(400);
    state.global[345] = Frame{};
    MemoryLocation* regPtr = state.global[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

TEST(FireVMLoadTest, LoadExprReadStack2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr * Stack + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 4);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

TEST(FireVMLoadTest, LoadExprReadThis2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load This = Stack + 0
        Load Expr * This + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

TEST(FireVMLoadTest, LoadExprReadExpr2Byte)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 0
        Load Expr * Expr + 345
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(400);
    state.stack[345] = Frame{};
    MemoryLocation* regPtr = state.stack[345].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 5);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

TEST(FireVMLoadTest, CheckNegativeOffsetsWork)
{
    std::vector<Instruction>    memory;
    std::stringstream           result;
    std::stringstream           file(buildStream(R"(
        CMD  Init   1000 1000
        Load Expr = Stack + 32
        Load Expr * Expr + -3
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.stack.resize(40);
    state.stack[29] = Frame{};
    MemoryLocation* regPtr = state.stack[29].getObject();

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 6);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], regPtr);
}

