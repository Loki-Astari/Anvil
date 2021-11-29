#include <gtest/gtest.h>

#include "FireVM.h"

using namespace ThorsAnvil::Anvil::Fire;

TEST(FireVMTest, CreaateFireVM)
{
    VMState     state;
    CodeBlock   code;
    FireVM      fire(state, code);
}

