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

TEST(SemanticTest, NameSpaceShortIsValid)
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

TEST(SemanticTest, NameSpaceLongNameIsValid)
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

TEST(SemanticTest, NameSpaceShortNameIsInValid_NoCap)
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

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoUnderScore)
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

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoSecondCap)
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

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoFirstCap)
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

TEST(SemanticTest, NameSpaceLongNameIsInValid_CapNoUnderScore)
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

TEST(SemanticTest, TypeNameObjectValid)
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

TEST(SemanticTest, TypeNameArrayValid)
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

TEST(SemanticTest, TypeNameMapValid)
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

TEST(SemanticTest, TypeNameFuncValid)
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

TEST(SemanticTest, TypeNameObjectInValid_FirstLower)
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

TEST(SemanticTest, TypeNameArrayInValid_FirstLower)
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

TEST(SemanticTest, TypeNameMapInValid_FirstLower)
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

TEST(SemanticTest, TypeNameFuncInValid_FirstLower)
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

TEST(SemanticTest, TypeNameObjectInValid_UnderScore)
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

TEST(SemanticTest, TypeNameArrayInValid_UnderScore)
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

TEST(SemanticTest, TypeNameMapInValid_UnderScore)
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

TEST(SemanticTest, TypeNameFuncInValid_UnderScore)
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


TEST(SemanticTest, TypeNameObjectInValid_Short)
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

TEST(SemanticTest, TypeNameArrayInValid_Short)
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

TEST(SemanticTest, TypeNameMapInValid_Short)
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

TEST(SemanticTest, TypeNameFuncInValid_Short)
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

TEST(SemanticTest, ObjectNameVariableValid)
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

TEST(SemanticTest, ObjectNameParameterValid)
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

TEST(SemanticTest, ObjectNameVariableInValid_UpperFirst)
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

TEST(SemanticTest, ObjectNameParameterInValid_UpperFirst)
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

TEST(SemanticTest, FindDeclInScopeValid)
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

TEST(SemanticTest, FindDeclInScopeInValid_BadNameSpace)
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

TEST(SemanticTest, FindDeclInScopeInValid_BadType)
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

TEST(SemanticTest, AddClassNormalNameSpace)
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

    EXPECT_NO_THROW(
        Class& classInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "MyClass")
    );
}

TEST(SemanticTest, AddArrayNormalNameSpace)
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

    EXPECT_NO_THROW(
        Array& classInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "MyArray")
    );
}

TEST(SemanticTest, AddMapNormalNameSpace)
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

    EXPECT_NO_THROW(
        Map& classInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "MyMap")
    );
}

TEST(SemanticTest, AddFuncNormalNameSpace)
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

    EXPECT_NO_THROW(
        Func& classInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "MyFunc")
    );
}


TEST(SemanticTest, AddClassNestedNameSpace)
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

    EXPECT_NO_THROW(
        Class& classInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyClass")
    );
}

TEST(SemanticTest, AddArrayNestedNameSpace)
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

    EXPECT_NO_THROW(
        Array& classInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyArray")
    );
}

TEST(SemanticTest, AddMapNestedNameSpace)
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

    EXPECT_NO_THROW(
        Map& classInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyMap")
    );
}

TEST(SemanticTest, AddFuncNestedNameSpace)
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

    EXPECT_NO_THROW(
        Func& classInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyFunc")
    );
}


TEST(SemanticTest, AddIdentifierLocalClass)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyClass");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameClass)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "MyClass");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameClass)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Alt", "MyClass");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonClass)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierLocalArray)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyArray");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameArray)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "MyArray");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameArray)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Alt", "MyArray");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonArray)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierLocalMap)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyMap");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameMap)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "MyMap");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameMap)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Alt", "MyMap");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonMap)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierLocalFunc)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "MyFunc");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameFunc)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "MyFunc");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameFunc)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Alt", "MyFunc");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonFunc)
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

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(compiler.globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierInvalidType)
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

