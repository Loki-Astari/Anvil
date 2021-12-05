#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVM_CmdTest, InvalidCmd)
{
    VMState     machineState;
    CodeBlock   machineProgram{Assembler::InvalidCmd};

    FireVM      vm(machineState, machineProgram);
    Result      result = vm.run();

    EXPECT_EQ(result, FireVM::haltInvalidCmd);
}

TEST(FireVM_CmdTest, CmdNoOp)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
CMD NoOp
CMD Kill 0
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);

    VMState     machineState;
    FireVM      vm(machineState, codeBlock);
    Result      output = vm.run();

    EXPECT_EQ(output, 0);
}

TEST(FireVM_CmdTest, CmdKill_10)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
CMD NoOp
CMD Kill 10
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);

    VMState     machineState;
    FireVM      vm(machineState, codeBlock);
    Result      output = vm.run();

    EXPECT_EQ(output, 10);
}

TEST(FireVM_CmdTest, CmdKill_Max)
{
    std::ostringstream   result;
    std::istringstream   input(R"(
CMD NoOp
CMD Kill 1023
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);

    VMState     machineState;
    FireVM      vm(machineState, codeBlock);
    Result      output = vm.run();

    EXPECT_EQ(output, 1023);
}


