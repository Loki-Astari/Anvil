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
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findMA = gns->find("MyClass");
    ASSERT_TRUE(findGNS.first);

    Class* classInfo = dynamic_cast<Class*>(findMA.second->second.get());
    ASSERT_NE(classInfo, nullptr);
}

TEST(SemanticTest, AddArrayNormalNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findMA = gns->find("MyArray");
    ASSERT_TRUE(findGNS.first);

    Array* array = dynamic_cast<Array*>(findMA.second->second.get());
    ASSERT_NE(array, nullptr);
}

TEST(SemanticTest, AddMapNormalNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findMA = gns->find("MyMap");
    ASSERT_TRUE(findGNS.first);

    Map* map = dynamic_cast<Map*>(findMA.second->second.get());
    ASSERT_NE(map, nullptr);
}

TEST(SemanticTest, AddFuncNormalNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findMA = gns->find("MyFunc");
    ASSERT_TRUE(findGNS.first);

    Func* func = dynamic_cast<Func*>(findMA.second->second.get());
    ASSERT_NE(func, nullptr);
}


TEST(SemanticTest, AddClassNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findNested = gns->find("Nest_Name");
    ASSERT_TRUE(findNested.first);

    Namespace*  nest = dynamic_cast<Namespace*>(findNested.second->second.get());
    ASSERT_NE(nest, nullptr);

    auto findMA = nest->find("MyClass");
    ASSERT_TRUE(findGNS.first);

    Class* classInfo = dynamic_cast<Class*>(findMA.second->second.get());
    ASSERT_NE(classInfo, nullptr);
}

TEST(SemanticTest, AddArrayNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findNested = gns->find("Nest_Name");
    ASSERT_TRUE(findNested.first);

    Namespace*  nest = dynamic_cast<Namespace*>(findNested.second->second.get());
    ASSERT_NE(nest, nullptr);

    auto findMA = nest->find("MyArray");
    ASSERT_TRUE(findGNS.first);

    Array* array = dynamic_cast<Array*>(findMA.second->second.get());
    ASSERT_NE(array, nullptr);
}

TEST(SemanticTest, AddMapNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    auto findGNS = globalScope.find("Good_Name_Space");
    ASSERT_TRUE(findGNS.first);

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findNested = gns->find("Nest_Name");
    ASSERT_TRUE(findNested.first);

    Namespace*  nest = dynamic_cast<Namespace*>(findNested.second->second.get());
    ASSERT_NE(nest, nullptr);

    auto findMA = nest->find("MyMap");
    ASSERT_TRUE(findGNS.first);

    Map* map = dynamic_cast<Map*>(findMA.second->second.get());
    ASSERT_NE(map, nullptr);
}

TEST(SemanticTest, AddFuncNestedNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    using ThorsAnvil::Anvil::Ice::StandardScope;
    using ThorsAnvil::Anvil::Ice::Namespace;
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

    Namespace*  gns = dynamic_cast<Namespace*>(findGNS.second->second.get());
    ASSERT_NE(gns, nullptr);

    auto findNested = gns->find("Nest_Name");
    ASSERT_TRUE(findNested.first);

    Namespace*  nest = dynamic_cast<Namespace*>(findNested.second->second.get());
    ASSERT_NE(nest, nullptr);

    auto findMA = nest->find("MyFunc");
    ASSERT_TRUE(findGNS.first);

    Func* func = dynamic_cast<Func*>(findMA.second->second.get());
    ASSERT_NE(func, nullptr);
}


