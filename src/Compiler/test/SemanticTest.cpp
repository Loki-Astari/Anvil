#include <gtest/gtest.h>

#include "Semantic.h"
#include "test/Utility.h"

TEST(SemanticTest, ShortNameSpaceIsValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Std
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE_OR_DEBUG(compiler.go(), result);
}

TEST(SemanticTest, LongNameSpaceIsValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_Space
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE_OR_DEBUG(compiler.go(), result);
}

TEST(SemanticTest, ShortNameSpaceIsInValid_NoCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace std
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, LongNameSpaceIsInValid_NoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace ComplexSpace
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, LongNameSpaceIsInValid_NoSecondCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_space
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, LongNameSpaceIsInValid_NoFirstCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace complex_Space
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, LongNameSpaceIsInValid_CapNoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_SpaceX
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

