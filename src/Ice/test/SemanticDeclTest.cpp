#include <gtest/gtest.h>

#include "Semantic.h"
#include "test/Utility.h"

struct SemanticCompiler
{
    ThorsAnvil::Anvil::Ice::StandardScope   globalScope;
    ThorsAnvil::Anvil::Ice::Lexer           lexer;
    ThorsAnvil::Anvil::Ice::Semantic        semanticAnalyser;
    ThorsAnvil::Anvil::Ice::Parser          parser;

    SemanticCompiler(std::istream& input, std::ostream& output)
        : lexer(input, output)
        , semanticAnalyser(lexer, globalScope, output)
        , parser(lexer, semanticAnalyser)
    {}

    bool compile()
    {
        return parser.parse();
    }
};

TEST(SemanticDeclTest, NameSpaceShortIsValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Std
{
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, NameSpaceLongNameIsValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_Space
{
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, NameSpaceShortNameIsInValid_NoCap)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace std
{
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticDeclTest, NameSpaceLongNameIsInValid_NoUnderScore)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace ComplexSpace
{
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticDeclTest, NameSpaceLongNameIsInValid_NoSecondCap)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_space
{
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticDeclTest, NameSpaceLongNameIsInValid_NoFirstCap)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace complex_Space
{
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticDeclTest, NameSpaceLongNameIsInValid_CapNoUnderScore)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_SpaceX
{
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticDeclTest, TypeNameObjectValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    class ClassName
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, TypeNameArrayValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array ArrayName { Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, TypeNameMapValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map MapName { Std::String, Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, TypeNameFuncValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func FuncName { Std::String -> Std::Integer }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, TypeNameObjectInValid_FirstLower)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    class className
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameArrayInValid_FirstLower)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array arrayName { Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameMapInValid_FirstLower)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map mapName { Std::String, Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameFuncInValid_FirstLower)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func funcName { Std::String -> Std::Integer }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameObjectInValid_UnderScore)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    class Class_Name
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameArrayInValid_UnderScore)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array Array_Name { Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameMapInValid_UnderScore)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map Map_Name { Std::String, Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameFuncInValid_UnderScore)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func Func_Name { Std::String -> Std::Integer }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}


TEST(SemanticDeclTest, TypeNameObjectInValid_Short)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    class Cla
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameArrayInValid_Short)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array Arr { Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameMapInValid_Short)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map Map { Std::String, Std::Integer}
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, TypeNameFuncInValid_Short)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func Fun { Std::String -> Std::Integer }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Type", result);
}

TEST(SemanticDeclTest, ObjectNameVariableValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    value : Std::String;
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, ObjectNameParameterValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    value : func { Std::String, Std::Integer -> Std::String };
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, ObjectNameVariableInValid_UpperFirst)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    Value : Std::String;
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Object", result);
}

TEST(SemanticDeclTest, ObjectNameParameterInValid_UpperFirst)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    Value : func { Std::String, Std::Integer -> Std::String };
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Invalid Identifier for Object", result);
}

TEST(SemanticDeclTest, FindDeclInScopeValid)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::Integer }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

TEST(SemanticDeclTest, FindDeclInScopeInValid_BadNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Ftd::Integer }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Could Not Find Type", result);
}

TEST(SemanticDeclTest, FindDeclInScopeInValid_BadType)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::FFInteger }
}
)");

    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Could Not Find Type", result);
}

TEST(SemanticDeclTest, AddClassNormalNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    class MyClass { value: Std::Integer; }
}
)");

    using ThorsAnvil::Anvil::Ice::Class;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto action = [&](){Class& classInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "MyClass");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddArrayNormalNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::Integer }
}
)");

    using ThorsAnvil::Anvil::Ice::Array;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto action = [&](){Array& classInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "MyArray");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddMapNormalNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map MyMap { Std::String, Std::Integer }
}
)");

    using ThorsAnvil::Anvil::Ice::Map;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto action = [&](){Map& classInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "MyMap");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddFuncNormalNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func MyFunc { Std::Integer -> Std::String}
}
)");

    using ThorsAnvil::Anvil::Ice::Func;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto action = [&](){Func& classInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "MyFunc");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}


TEST(SemanticDeclTest, AddClassNestedNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        class MyClass { value: Std::Integer; }
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Class;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto action = [&](){Class& classInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyClass");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddArrayNestedNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        array MyArray { Std::Integer }
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Array;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto action = [&](){Array& classInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyArray");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddMapNestedNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        map MyMap { Std::String, Std::Integer }
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Map;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto action = [&](){Map& classInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyMap");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddFuncNestedNameSpace)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        func MyFunc { Std::Integer -> Std::String}
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Func;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){Func& classInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyFunc");};
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}


TEST(SemanticDeclTest, AddIdentifierLocalClass)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        class MyClass {}
        value: MyClass;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyClass");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierParentNameClass)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    class MyClass {}

    namespace Nest_Name
    {
        value: MyClass;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "MyClass");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnotherNameClass)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Alt
    {
        class MyClass {}
    }

    namespace Nest_Name
    {
        value: Alt::MyClass;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Alt", "MyClass");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnonClass)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        value: class {};
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&]() {
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierLocalArray)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        array MyArray {Std::Integer}
        value: MyArray;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyArray");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierParentNameArray)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray {Std::Integer}

    namespace Nest_Name
    {
        value: MyArray;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "MyArray");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnotherNameArray)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Alt
    {
        array MyArray {Std::Integer}
    }

    namespace Nest_Name
    {
        value: Alt::MyArray;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Alt", "MyArray");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnonArray)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        value: array {Std::Integer};
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierLocalMap)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        map MyMap {Std::Integer, Std::String}
        value: MyMap;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyMap");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierParentNameMap)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map MyMap {Std::Integer, Std::String}

    namespace Nest_Name
    {
        value: MyMap;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "MyMap");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnotherNameMap)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Alt
    {
        map MyMap {Std::Integer, Std::String}
    }

    namespace Nest_Name
    {
        value: Alt::MyMap;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Alt", "MyMap");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnonMap)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        value: map {Std::Integer, Std::String};
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierLocalFunc)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        func MyFunc {Std::Integer, Std::String -> Std::Integer}
        value: MyFunc;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyFunc");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierParentNameFunc)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func MyFunc {Std::Integer, Std::String -> Std::Integer}

    namespace Nest_Name
    {
        value: MyFunc;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "MyFunc");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnotherNameFunc)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Alt
    {
        func MyFunc {Std::Integer, Std::String -> Std::Integer}
    }

    namespace Nest_Name
    {
        value: Alt::MyFunc;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Alt", "MyFunc");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierAnonFunc)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        value: func {Std::Integer, Std::String -> Std::Integer};
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);

    auto findGNS = compiler.globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    auto action = [&](){
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    };
    EXPECT_NO_THROW_OR_DEBUG(action(), result);
}

TEST(SemanticDeclTest, AddIdentifierInvalidType)
{
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    namespace Nest_Name
    {
        value: ABadType;
    }
}
)");

    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
    SemanticCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Could Not Find Type", result);
}

