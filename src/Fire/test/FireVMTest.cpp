#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVMTest, NoCode)
{
    VMState     machineState;
    CodeBlock   machineProgram;

    FireVM      vm(machineState, machineProgram);
    int result = vm.run();

    EXPECT_EQ(result, FireVM::haltRanOutOfProgramMemory);
}

TEST(FireVMTest, RanOutOfCode)
{
    VMState     machineState;
    CodeBlock   machineProgram{{Assembler::Act_CMD | Assembler::Cmd_NoOp }};

    FireVM      vm(machineState, machineProgram);
    int result = vm.run();

    EXPECT_EQ(result, FireVM::haltRanOutOfProgramMemory);
}

TEST(FireVMTest, InvalidAction)
{
    VMState     machineState;
    CodeBlock   machineProgram{{Assembler::InvalidAction}};

    FireVM      vm(machineState, machineProgram);
    int result = vm.run();

    EXPECT_EQ(result, FireVM::haltInvalidAction);
}

