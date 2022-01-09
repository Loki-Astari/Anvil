#include <gtest/gtest.h>

#include "test/Utility.h"

TEST(GeneratorTest, Std)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  {}
}
    )");

    GeneratorCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(std::cout), result);

}
TEST(GeneratorTest, CallPrintOnConsole)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  {}
}
namespace Sys
{
    class Console {
        print : func{ Std::String -> Void }{}
    }
    console : Console;
}
namespace Name_Space
{
    main : func {Void -> Std::Integer}
    {
        Sys::console.print("Hello World");
        return 0;
    }
}
    )");

    GeneratorCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(std::cout), result);

}
