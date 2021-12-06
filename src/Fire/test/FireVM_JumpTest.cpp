#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

/*
 * Jump_Call)
 * -----------
 * Jump_Invalid)
 */

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVM_JumpTest, Jump_Call)
{
    std::stringstream    result;
    std::istringstream   input(R"(
CMD Init 100 100
JUMP Call AL Abs func
CMD Kill 3

func:
CMD Kill 255
)");

    SymbolTable         stable;
    CodeBlock           codeBlock;

    Assembler           assembler(result, stable);
    assembler.assemble(input, codeBlock);

    VMState     machineState;
    FireVM      vm(machineState, codeBlock);
    Result      output = vm.run();
    bool bad = false;
    EXPECT_EQ_OR_LOG(bad, output, 255, result);

    EXPECT_FALSE_OR_DEBUG(bad, result);
}

// Error Conditions

TEST(FireVM_JumpTest, Jump_Invalid)
{
    VMState     machineState;
    CodeBlock   machineProgram{Assembler::InvalidJump};

    FireVM      vm(machineState, machineProgram);
    Result      result = vm.run();

    EXPECT_EQ(result, FireVM::haltInvalidJump);
}


