#include <gtest/gtest.h>

#include "Parser.h"
#include "Lexer.h"
#include "Storage.h"
#include "ice.tab.hpp"
#include "test/Utility.h"

#include <fstream>
#include <sstream>

struct ParserCompiler
{
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Namespace       globalScope;
    ThorsAnvil::Anvil::Ice::Storage         storage;
    ThorsAnvil::Anvil::Ice::Action          action;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    ParserCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , globalScope("GlobalScope")
        , action(lexer, globalScope, storage, output)
        , parser(lexer, action)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(ParserTest, NameSpaceOnly)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
    EXPECT_TRUE_OR_DEBUG(compiler.globalScope.get("Name_Space").first, result);
}

TEST(ParserTest,NameSpaceNested)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    namespace Nest_Space
    {
    }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
    EXPECT_TRUE_OR_DEBUG(compiler.globalScope.get("Name_Space").first, result);
    ThorsAnvil::Anvil::Ice::Decl& decl = *compiler.globalScope.get("Name_Space").second->second;
    ThorsAnvil::Anvil::Ice::Namespace& ns = dynamic_cast<ThorsAnvil::Anvil::Ice::Namespace&>(decl);
    EXPECT_TRUE_OR_DEBUG(ns.get("Nest_Space").first, result);
}

TEST(ParserTest,NameSpaceTwice)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
}

namespace Twice_Space
{
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
    EXPECT_TRUE_OR_DEBUG(compiler.globalScope.get("Name_Space").first, result);
    EXPECT_TRUE_OR_DEBUG(compiler.globalScope.get("Twice_Space").first, result);
}

#if 0 
TEST(ParserTest,NameStatExprFuncCall)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    Sys::console.print("Hello World");
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameTypeArray)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    array TypeArray { Std::String }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
#endif

TEST(ParserTest,NameTypeFunc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  {}
}
namespace Name_Space
{
    func TypeFunc{Std::Integer -> Std::String}
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

#if 0
TEST(ParserTest,NameTypeMap)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    map TypeMap { Std::String, Std::Integer}
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
#endif

TEST(ParserTest, ClassTrivial)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    class ClassName
    {
    }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

#if 0
TEST(ParserTest,NameTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    class ClassName
    {
        name:   Std::String;
        age:    Std::Integer;
    }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeArray)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : array { Std::Integer };
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeFunc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : func{Std::Integer, Std::String -> Std::Number};
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeMap)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : map { Std::Integer, Std::String };
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableAnonTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data: Class
    {
        name:   String;
        age:    Number;
    }
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableDecl)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data: Std::String;
}
    )");

    ParserCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
#endif


