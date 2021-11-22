#include <gtest/gtest.h>

#include "Semantic.h"
#include "test/Utility.h"

TEST(SemanticTest, NameSpaceShortIsValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Std
{
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, NameSpaceLongNameIsValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_Space
{
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, NameSpaceShortNameIsInValid_NoCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace std
{
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace ComplexSpace
{
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoSecondCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_space
{
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoFirstCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace complex_Space
{
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_CapNoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_SpaceX
{
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, TypeNameObjectValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, TypeNameArrayValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array ArrayName { Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, TypeNameMapValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map MapName { Std::String, Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, TypeNameFuncValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func FuncName { Std::String -> Std::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, TypeNameObjectInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameArrayInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array arrayName { Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameMapInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map mapName { Std::String, Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameFuncInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func funcName { Std::String -> Std::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameObjectInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameArrayInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array Array_Name { Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameMapInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map Map_Name { Std::String, Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameFuncInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func Func_Name { Std::String -> Std::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}


TEST(SemanticTest, TypeNameObjectInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameArrayInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array Arr { Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameMapInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map Map { Std::String, Std::Integer}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameFuncInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func Fun { Std::String -> Std::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, ObjectNameVariableValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    value : Std::String;
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, ObjectNameParameterValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    value : func { Std::String, Std::Integer -> Std::String };
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, ObjectNameVariableInValid_UpperFirst)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    Value : Std::String;
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Object", result);
}

TEST(SemanticTest, ObjectNameParameterInValid_UpperFirst)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    Value : func { Std::String, Std::Integer -> Std::String };
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Invalid Identifier for Object", result);
}

TEST(SemanticTest, FindDeclInScopeValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);
}

TEST(SemanticTest, FindDeclInScopeInValid_BadNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Ftd::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Could Not Find Type", result);
}

TEST(SemanticTest, FindDeclInScopeInValid_BadType)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::FFInteger }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Could Not Find Type", result);
}

TEST(SemanticTest, AddClassNormalNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Class;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    class MyClass { value: Std::Integer; }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    EXPECT_NO_THROW(
        Class& classInfo = getType<Class>(globalScope, "Good_Name_Space", "MyClass")
    );
}

TEST(SemanticTest, AddArrayNormalNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Array;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    EXPECT_NO_THROW(
        Array& classInfo = getType<Array>(globalScope, "Good_Name_Space", "MyArray")
    );
}

TEST(SemanticTest, AddMapNormalNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Map;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map MyMap { Std::String, Std::Integer }
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    EXPECT_NO_THROW(
        Map& classInfo = getType<Map>(globalScope, "Good_Name_Space", "MyMap")
    );
}

TEST(SemanticTest, AddFuncNormalNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Func;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func MyFunc { Std::Integer -> Std::String}
}
)");

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    EXPECT_NO_THROW(
        Func& classInfo = getType<Func>(globalScope, "Good_Name_Space", "MyFunc")
    );
}


TEST(SemanticTest, AddClassNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Class;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    EXPECT_NO_THROW(
        Class& classInfo = getType<Class>(globalScope, "Good_Name_Space", "Nest_Name", "MyClass")
    );
}

TEST(SemanticTest, AddArrayNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Array;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    EXPECT_NO_THROW(
        Array& classInfo = getType<Array>(globalScope, "Good_Name_Space", "Nest_Name", "MyArray")
    );
}

TEST(SemanticTest, AddMapNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Map;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    EXPECT_NO_THROW(
        Map& classInfo = getType<Map>(globalScope, "Good_Name_Space", "Nest_Name", "MyMap")
    );
}

TEST(SemanticTest, AddFuncNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Func;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Func& classInfo = getType<Func>(globalScope, "Good_Name_Space", "Nest_Name", "MyFunc")
    );
}


TEST(SemanticTest, AddIdentifierLocalClass)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(globalScope, "Good_Name_Space", "Nest_Name", "MyClass");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameClass)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(globalScope, "Good_Name_Space", "MyClass");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameClass)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(globalScope, "Good_Name_Space", "Alt", "MyClass");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonClass)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Class;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Class>(globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierLocalArray)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(globalScope, "Good_Name_Space", "Nest_Name", "MyArray");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameArray)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(globalScope, "Good_Name_Space", "MyArray");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameArray)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(globalScope, "Good_Name_Space", "Alt", "MyArray");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonArray)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Array;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Array>(globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierLocalMap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(globalScope, "Good_Name_Space", "Nest_Name", "MyMap");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameMap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(globalScope, "Good_Name_Space", "MyMap");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameMap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(globalScope, "Good_Name_Space", "Alt", "MyMap");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonMap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Map;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Map>(globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierLocalFunc)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(globalScope, "Good_Name_Space", "Nest_Name", "MyFunc");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierParentNameFunc)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(globalScope, "Good_Name_Space", "MyFunc");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnotherNameFunc)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(globalScope, "Good_Name_Space", "Alt", "MyFunc");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierAnonFunc)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_TRUE_OR_DEBUG(semanticAnalyser.go(), result);

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    EXPECT_NO_THROW(
        Type&   typeInfo = getType<Func>(globalScope, "Good_Name_Space", "Nest_Name", "$0000000000000000");
        Object& obectInfo = getType<Object>(globalScope, "Good_Name_Space", "Nest_Name", "value");
        ASSERT_TRUE(&typeInfo == &obectInfo.getType());
    );
}

TEST(SemanticTest, AddIdentifierInvalidType)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Type;
    using ThorsAnvil::Anvil::Ice::Func;
    using ThorsAnvil::Anvil::Ice::Object;
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

    StandardScope     globalScope;
    Semantic          semanticAnalyser(globalScope, file, result);
    EXPECT_THROW_OR_DEBUG(semanticAnalyser.go(), "Could Not Find Type", result);
}

