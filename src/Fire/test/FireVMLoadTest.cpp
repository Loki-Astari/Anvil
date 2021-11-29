#include <gtest/gtest.h>

#include "../Ice/test/Utility.h"
#include "FireVM.h"
#include "Assembler.h"

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVMLoadTest, LoadGlobalPosAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Literal 6
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 6);
}
TEST(FireVMLoadTest, LoadStackPosAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Literal 18
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 18);
}
TEST(FireVMLoadTest, LoadThisPosAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Literal 22
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 22);
}
TEST(FireVMLoadTest, LoadExprPosAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Literal 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 12);
}
TEST(FireVMLoadTest, LoadGlobalNegAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Literal -8
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -8);
}
TEST(FireVMLoadTest, LoadStackNegAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Literal -30
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -30);
}
TEST(FireVMLoadTest, LoadThisNegAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Literal -11
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -11);
}
TEST(FireVMLoadTest, LoadExprNegAbsoluteLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Literal -13
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -13);
}

//----

TEST(FireVMLoadTest, LoadGlobalPosAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Literal 123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 123);
}
TEST(FireVMLoadTest, LoadStackPosAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Literal 456
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 456);
}
TEST(FireVMLoadTest, LoadThisPosAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Literal 344
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 344);
}
TEST(FireVMLoadTest, LoadExprPosAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Literal 89
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 89);
}
TEST(FireVMLoadTest, LoadGlobalNegAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Literal -123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -123);
}
TEST(FireVMLoadTest, LoadStackNegAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Literal -364
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -364);
}
TEST(FireVMLoadTest, LoadThisNegAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Literal -126
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -126);
}
TEST(FireVMLoadTest, LoadExprNegAbsoluteLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Literal -59
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -59);
}

// -----

TEST(FireVMLoadTest, LoadGlobalPosRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Literal 6
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 8);
}
TEST(FireVMLoadTest, LoadStackPosRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Literal 18
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 20);
}
TEST(FireVMLoadTest, LoadThisPosRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Literal 22
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 24);
}
TEST(FireVMLoadTest, LoadExprPosRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Literal 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 14);
}
TEST(FireVMLoadTest, LoadGlobalNegRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Literal -8
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -6);
}
TEST(FireVMLoadTest, LoadStackNegRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Literal -30
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -28);
}
TEST(FireVMLoadTest, LoadThisNegRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Literal -11
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -9);
}
TEST(FireVMLoadTest, LoadExprNegRelativeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Literal -13
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -11);
}

//----

TEST(FireVMLoadTest, LoadGlobalPosRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Literal 123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 125);
}
TEST(FireVMLoadTest, LoadStackPosRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Literal 456
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 458);
}
TEST(FireVMLoadTest, LoadThisPosRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Literal 344
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 346);
}
TEST(FireVMLoadTest, LoadExprPosRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Literal 89
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 91);
}
TEST(FireVMLoadTest, LoadGlobalNegRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Literal -123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -121);
}
TEST(FireVMLoadTest, LoadStackNegRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Literal -364
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -362);
}
TEST(FireVMLoadTest, LoadThisNegRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Literal -126
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -124);
}
TEST(FireVMLoadTest, LoadExprNegRelativeLargeLiteral)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Literal -59
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -57);
}

// ----
/*
TEST(FireVMLoadTest, LoadGlobalPosAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Read 6
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 6);
}
TEST(FireVMLoadTest, LoadStackPosAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Read 18
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 18);
}
TEST(FireVMLoadTest, LoadThisPosAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Read 22
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 22);
}
TEST(FireVMLoadTest, LoadExprPosAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Read 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 12);
}
TEST(FireVMLoadTest, LoadGlobalNegAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Read -8
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -8);
}
TEST(FireVMLoadTest, LoadStackNegAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Read -30
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -30);
}
TEST(FireVMLoadTest, LoadThisNegAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Read -11
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -11);
}
TEST(FireVMLoadTest, LoadExprNegAbsoluteRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Read -13
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -13);
}

//----

TEST(FireVMLoadTest, LoadGlobalPosAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Read 123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 123);
}
TEST(FireVMLoadTest, LoadStackPosAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Read 456
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 456);
}
TEST(FireVMLoadTest, LoadThisPosAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Read 344
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 344);
}
TEST(FireVMLoadTest, LoadExprPosAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Read 89
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 89);
}
TEST(FireVMLoadTest, LoadGlobalNegAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global = Read -123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -123);
}
TEST(FireVMLoadTest, LoadStackNegAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack = Read -364
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -364);
}
TEST(FireVMLoadTest, LoadThisNegAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This = Read -126
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -126);
}
TEST(FireVMLoadTest, LoadExprNegAbsoluteLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr = Read -59
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -59);
}

// -----

TEST(FireVMLoadTest, LoadGlobalPosRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Read 6
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;

    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 8);
}
TEST(FireVMLoadTest, LoadStackPosRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Read 18
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 20);
}
TEST(FireVMLoadTest, LoadThisPosRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Read 22
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 24);
}
TEST(FireVMLoadTest, LoadExprPosRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Read 12
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 14);
}
TEST(FireVMLoadTest, LoadGlobalNegRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Read -8
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -6);
}
TEST(FireVMLoadTest, LoadStackNegRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Read -30
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -28);
}
TEST(FireVMLoadTest, LoadThisNegRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Read -11
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -9);
}
TEST(FireVMLoadTest, LoadExprNegRelativeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Read -13
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 1);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -11);
}

//----

TEST(FireVMLoadTest, LoadGlobalPosRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Read 123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], 125);
}
TEST(FireVMLoadTest, LoadStackPosRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Read 456
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], 458);
}
TEST(FireVMLoadTest, LoadThisPosRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Read 344
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], 346);
}
TEST(FireVMLoadTest, LoadExprPosRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Read 89
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], 91);
}
TEST(FireVMLoadTest, LoadGlobalNegRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Global += Read -123
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Global]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Global], -121);
}
TEST(FireVMLoadTest, LoadStackNegRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Stack += Read -364
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Stack]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Stack], -362);
}
TEST(FireVMLoadTest, LoadThisNegRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load This += Read -126
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::This]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::This], -124);
}
TEST(FireVMLoadTest, LoadExprNegRelativeLargeRead)
{
    CodeBlock                   memory;

    std::stringstream           result;
    std::stringstream           file(buildStream(R"(

    Load Expr += Read -59
)"));

    Assembler                   assembler(file, result, memory);
    ASSERT_TRUE(assembler.isOK());

    VMState     state;
    state.registerFrame[Register::Expr]   = 2;
    FireVM      fire(state, memory);
    int         vmResult = fire.run();

    EXPECT_EQ(state.programCounter, 2);
    EXPECT_EQ(vmResult, 0);

    EXPECT_EQ(state.registerFrame[Register::Expr], -57);
}

*/
