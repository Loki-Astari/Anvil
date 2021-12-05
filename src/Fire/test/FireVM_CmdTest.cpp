#include <gtest/gtest.h>

#include "FireVM.h"
#include "Assembler.h"
#include "../Ice/test/Utility.h"

#include <sstream>

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVM_CmdTest, InvalidCmd)
{
    VMState     machineState;
    CodeBlock   machineProgram{{Assembler::InvalidCmd}};

    FireVM      vm(machineState, machineProgram);
    int result = vm.run();

    EXPECT_EQ(result, FireVM::haltInvalidCmd);
}

