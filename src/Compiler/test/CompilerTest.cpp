#include <gtest/gtest.h>

#include "Compiler.h"
#include "test/CompileAction.h"

TEST(CompilerTest, ShortNameSpaceIsValid)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Std
{
}
)");

    Compiler          compiler(file, result);
    EXPECT_TRUE_OR_DEBUG(compiler.go(), result);
}

TEST(CompilerTest, LongNameSpaceIsValid)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_Space
{
}
)");

    Compiler          compiler(file, result);
    EXPECT_TRUE_OR_DEBUG(compiler.go(), result);
}

TEST(CompilerTest, ShortNameSpaceIsInValid_NoCap)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace std
{
}
)");

    Compiler          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(CompilerTest, LongNameSpaceIsInValid_NoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace ComplexSpace
{
}
)");

    Compiler          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(CompilerTest, LongNameSpaceIsInValid_NoSecondCap)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_space
{
}
)");

    Compiler          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(CompilerTest, LongNameSpaceIsInValid_NoFirstCap)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace complex_Space
{
}
)");

    Compiler          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(CompilerTest, LongNameSpaceIsInValid_CapNoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Compiler;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_SpaceX
{
}
)");

    Compiler          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

