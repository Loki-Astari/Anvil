#include <gtest/gtest.h>

#include "test/Utility.h"

TEST(ParserTest, NameSpaceOnly)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
}
    )");

    FacadeCompiler  compiler(file, result);

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

    FacadeCompiler  compiler(file, result);

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

    FacadeCompiler  compiler(file, result);

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

    FacadeCompiler  compiler(file, result);

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

    FacadeCompiler  compiler(file, result);

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

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,ReopenStdToAddMoreInfo)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
}
namespace Std
{
    class String  {}
}
namespace Name_Space
{
    func TypeFunc{Std::Integer -> Std::String}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NestedNamespace)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    namespace Ext
    {
        class String  {}
    }
}
namespace Name_Space
{
    func TypeFunc{Std::Integer -> Std::Ext::String}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,ReopenNameSpaceToAddMoreInfo)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
}
namespace Name_Space
{
    class String  {}
}
namespace Name_Space
{
    func TypeFunc{Std::Integer -> String}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,InvalidNamespaceName)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    func TypeFunc{Std::Integer -> Std::String}
}
    )");

    FacadeCompiler  compiler(file, result);
    auto action = [&](){compiler.compile();};
    EXPECT_THROW_OR_DEBUG(action(), "Invalid Identifier Name: Std (Namespace)", result);
}

TEST(ParserTest,InvalidNamespaceInNamespace)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
}
namespace Name_Space
{
    func TypeFunc{Std::Ext::Integer -> Std::String}
}
    )");

    FacadeCompiler  compiler(file, result);
    auto action = [&](){compiler.compile();};
    EXPECT_THROW_OR_DEBUG(action(), "The Identifier >Ext< can not be found in >Std<", result);
}

TEST(ParserTest,InvalidType)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    func TypeFunc{Integer -> Std::String}
}
    )");

    FacadeCompiler  compiler(file, result);
    auto action = [&](){compiler.compile();};
    EXPECT_THROW_OR_DEBUG(action(), "Error: Invalid Identifier Name: Integer (Type)", result);
}

TEST(ParserTest,InvalidTypeInScope)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {}
namespace Name_Space
{
    func TypeFunc{Std::Integer -> Std::String}
}
    )");

    FacadeCompiler  compiler(file, result);
    auto action = [&](){compiler.compile();};
    EXPECT_THROW_OR_DEBUG(action(), "The Identifier >Integer< can not be found in >Std<", result);
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

    FacadeCompiler  compiler(file, result);

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

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class String {}
    class Integer {}
}
namespace Name_Space
{
    class ClassName
    {
        name:   Std::String;
        age:    Std::Integer;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

#if 0
TEST(ParserTest,NameVariableAnonTypeArray)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : array { Std::Integer };
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
#endif

TEST(ParserTest,NameVariableAnonTypeFunc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class Integer {}
    class String  {}
    class Number  {}
}
namespace Name_Space
{
    data : func{Std::Integer, Std::String -> Std::Number}
    {}
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

#if 0
TEST(ParserTest,NameVariableAnonTypeMap)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Name_Space
{
    data : map { Std::Integer, Std::String };
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}
#endif

TEST(ParserTest,NameVariableAnonTypeObject)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class String {}
    class Number {}
}
namespace Name_Space
{
    data: class
    {
        name:   Std::String;
        age:    Std::Number;
    };
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(ParserTest,NameVariableDecl)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std
{
    class String {}
}
namespace Name_Space
{
    data: Std::String;
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}


