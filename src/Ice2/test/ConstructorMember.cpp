#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "Storage.h"
#include "Declaration.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>
#include <sstream>

struct FacadeCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Namespace       globalScope;
    ThorsAnvil::Anvil::Ice::Storage         storage;
    ThorsAnvil::Anvil::Ice::Action          action;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    FacadeCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , globalScope(ThorsAnvil::Anvil::Ice::ActionRef{}, "GlobalScope")
        , action(lexer, globalScope, storage, output)
        , parser(lexer, action)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(ConstructorMember, DefaultAll)
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

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorMember, DefaultEven)
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

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorMember, DefaultOdd)
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

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ConstructorMember, DefaultOutOfOrderTop)
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

    FacadeCompiler  compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Members are initialized in wrong order", result);

}

TEST(ConstructorMember, DefaultOutOfOrderBot)
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

    FacadeCompiler  compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Members are initialized in wrong order", result);
}


