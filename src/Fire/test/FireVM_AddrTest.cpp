#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "test/BuildVM.h"

#include <sstream>

/*
 */

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVM_AddrTest, InitialRegState)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[This], nullptr, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Extra], nullptr, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Expr1], nullptr, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Expr2], nullptr, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Expr3], nullptr, result);


    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Global], &vm.machineState.global[0], result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[FrameP], &vm.machineState.stack[0], result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[StackP], &vm.machineState.stack[0], result);

    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LRRThisZero)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR This = Global + 0
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[This], vm.machineState.addressRegister[Global], result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LRRExtra15)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Extra = StackPointer + 15
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Extra], vm.machineState.addressRegister[StackP] + 15, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LMLInt)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 15
ADDR LML Expr-1 = Int 65535
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    MemoryLocation& valueLoc = vm.machineState.stack[15];
    bool correctValue = true;
    Int value = valueLoc.getValue<Int>([&correctValue](){correctValue = false;});

    EXPECT_EQ_OR_LOG(bad, correctValue, true, result);
    EXPECT_EQ_OR_LOG(bad, value, 65535, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Expr1], vm.machineState.addressRegister[StackP] + 15, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LMLIntLarge)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-2 = StackPointer + 15
ADDR LML Expr-2 = Int 65536
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    MemoryLocation& valueLoc = vm.machineState.stack[15];
    bool correctValue = true;
    Int value = valueLoc.getValue<Int>([&correctValue](){correctValue = false;});

    EXPECT_EQ_OR_LOG(bad, correctValue, true, result);
    EXPECT_EQ_OR_LOG(bad, value, 65536, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LMLString)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-3 = StackPointer + 15
ADDR LML Expr-3 = String Working nine to five
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    MemoryLocation& valueLoc = vm.machineState.stack[15];
    bool correctValue = true;
    String value = valueLoc.getValue<String>([&correctValue](){correctValue = false;});

    EXPECT_EQ_OR_LOG(bad, correctValue, true, result);
    EXPECT_EQ_OR_LOG(bad, value, "Working nine to five", result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LMLCodeAddress)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR FramePointer = StackPointer + 15
ADDR LML FramePointer = CodeAddress ThisPoint
CMD Kill 16

CMD NoOp
CMD NoOp
ThisPoint: CMD NoOp
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    MemoryLocation& valueLoc = vm.machineState.stack[15];
    bool correctValue = true;
    CodeAddress value = valueLoc.getValue<CodeAddress>([&correctValue](){correctValue = false;});

    EXPECT_EQ_OR_LOG(bad, correctValue, true, result);
    EXPECT_EQ_OR_LOG(bad, value, vm.stable["ThisPoint"], result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LMLDataFrame)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR StackPointer = FramePointer + 15
ADDR LML StackPointer = DataFrame 10
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    MemoryLocation& valueLoc = vm.machineState.stack[15];
    bool correctValue = true;
    DataFrame& value = valueLoc.getValue<DataFrame>([&correctValue](){correctValue = false;});

    EXPECT_EQ_OR_LOG(bad, correctValue, true, result);
    EXPECT_EQ_OR_LOG(bad, value.size(), 10, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.stack[15].getAddress(), &value[0], result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LMPExpr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 15
ADDR LRR Expr-2 = StackPointer + 16
ADDR LML Expr-1 = String Hi There
ADDR LMP Expr-2 = Expr-1 + 0
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    MemoryLocation& valueLoc = vm.machineState.stack[16];
    bool correctValue = true;
    String& value = valueLoc.getValue<String>([&correctValue](){correctValue = false;});

    EXPECT_EQ_OR_LOG(bad, correctValue, true, result);
    EXPECT_EQ_OR_LOG(bad, value, "Hi There", result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LRPExpr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 15
ADDR LRR Expr-2 = StackPointer + 16
ADDR LMR Expr-1 = Expr-2 + 12
ADDR LRP Expr-3 = Expr-1 + 0
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Expr3], &vm.machineState.stack[28], result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, LMRExpr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 15
ADDR LRR Expr-2 = StackPointer + 16
ADDR LMR Expr-2 = Expr-1 + 12
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);

    MemoryLocation& valueLoc = vm.machineState.stack[16];
    bool correctValue = true;
    DataAddress& value = valueLoc.getValue<DataAddress>([&correctValue](){correctValue = false;});

    EXPECT_EQ_OR_LOG(bad, correctValue, true, result);
    EXPECT_EQ_OR_LOG(bad, value, &vm.machineState.stack[27], result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, INCExpr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 15
ADDR INC Expr-1 22
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Expr1], &vm.machineState.stack[37], result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, DECExpr1)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 15
ADDR DEC Expr-1 8
CMD Kill 16
)");

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);
    EXPECT_EQ_OR_LOG(bad, vm.machineState.addressRegister[Expr1], &vm.machineState.stack[7], result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, InvalidAddr)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = StackPointer + 15
CMD Kill 16
)");

    vm.codeBlock[2] = Assembler::InvalidAddr;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidAddr, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, InvalidLMLLiteral)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LML Expr-1 = Int 21
CMD Kill 16
)");

    vm.codeBlock[2] = vm.codeBlock[2] | Assembler::Addr_Literal_Mask;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidLiteralType, result);
    EXPECT_SUCC(bad, result);
}

TEST(FireVM_AddrTest, InvalidLRRAddType)
{
    std::stringstream   result;
    bool                bad = false;
    BuildVM             vm(result, bad, R"(
ADDR LRR Expr-1 = Expr-2 + 12
CMD Kill 16
)");

    vm.codeBlock[2] = vm.codeBlock[2] | Assembler::Addr_AddType_Mask;
    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, FireVM::haltInvalidAddType, result);
    EXPECT_SUCC(bad, result);
}

