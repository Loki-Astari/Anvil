#include <gtest/gtest.h>

#include "Compiler.h"
#include "test/CompileAction.h"

TEST(CompilerTest, XX)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   file = buildStream(R"(
namespace Name
{
}
)");

    Compiler          compiler(file);
    compiler.go();
}

