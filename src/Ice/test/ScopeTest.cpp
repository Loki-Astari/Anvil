#include <gtest/gtest.h>

#include "test/Utility.h"

TEST(ScopeTest, OneObjectDataInScope)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : Void;
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ScopeTest, OneObjectDataInScopeTwice)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : Void;
    data : Void;
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Object Already defined:", result);
}

TEST(ScopeTest, OneFunctionDataInScope)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : func { Void -> Void}{}
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ScopeTest, OneFunctionDataInScopeTwiceDifferentParam)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : func { Void -> Void}{}
    data : func { Void, Void -> Void}{}
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
TEST(ScopeTest, OneFunctionDataInScopeTwiceSameParam)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : func { Void -> Void}{}
    data : func { Void -> Void}{}
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Function Already defined:", result);
}
