#include <gtest/gtest.h>

#include "test/Utility.h"

TEST(ConstructorMemberTest, DefaultAll)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  { construct{String}{} construct{}{}}
}
namespace Sys
{
    class MyStuff
    {
        data1: Std::String;
        data2: Std::String;
        data3: Std::String;
        data4: Std::String;

        construct{ Void }
            : data1("1")
            , data2("2")
            , data3("3")
            , data4("4")
        {}
    }
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorMemberTest, DefaultEven)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  { construct{String}{} construct{}{}}
}
namespace Sys
{
    class MyStuff
    {
        data1: Std::String;
        data2: Std::String;
        data3: Std::String;
        data4: Std::String;

        construct{ Void }
            : data2("2")
            , data4("4")
        {}
    }
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorMemberTest, DefaultOdd)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  { construct{String}{} construct{}{}}
}
namespace Sys
{
    class MyStuff
    {
        data1: Std::String;
        data2: Std::String;
        data3: Std::String;
        data4: Std::String;

        construct{ Void }
            : data1("1")
            , data3("3")
        {}
    }
}
    )");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorMemberTest, DefaultOutOfOrderTop)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  { construct{String}{} construct{}{}}
}
namespace Sys
{
    class MyStuff
    {
        data1: Std::String;
        data2: Std::String;
        data3: Std::String;
        data4: Std::String;

        construct{ Void }
            : data3("3")
            , data1("1")
            , data2("2")
            , data4("4")
        {}
    }
}
    )");

    SemanticCompiler  compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Members are initialized in wrong order", result);

}

TEST(ConstructorMemberTest, DefaultOutOfOrderBot)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  { construct{String}{} construct{}{}}
}
namespace Sys
{
    class MyStuff
    {
        data1: Std::String;
        data2: Std::String;
        data3: Std::String;
        data4: Std::String;

        construct{ Void }
            : data1("1")
            , data2("2")
            , data4("4")
            , data3("3")
        {}
    }
}
    )");

    SemanticCompiler  compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Members are initialized in wrong order", result);
}


