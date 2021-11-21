#include <gtest/gtest.h>

#include "Semantic.h"
#include "test/Utility.h"

TEST(SemanticTest, NameSpaceShortIsValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Std
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE_OR_DEBUG(compiler.go(), result);
}

TEST(SemanticTest, NameSpaceLongNameIsValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_Space
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE_OR_DEBUG(compiler.go(), result);
}

TEST(SemanticTest, NameSpaceShortNameIsInValid_NoCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace std
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace ComplexSpace
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoSecondCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_space
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_NoFirstCap)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace complex_Space
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, NameSpaceLongNameIsInValid_CapNoUnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Complex_SpaceX
{
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Namespace", result);
}

TEST(SemanticTest, TypeNameObjectValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    type TypeName
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE(compiler.go());
}

TEST(SemanticTest, TypeNameArrayValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array ArrayName { Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE(compiler.go());
}

TEST(SemanticTest, TypeNameMapValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map MapName { Std::String, Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE(compiler.go());
}

TEST(SemanticTest, TypeNameFuncValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func FuncName { value: Std::String -> Std::Integer }
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE(compiler.go());
}

TEST(SemanticTest, TypeNameObjectInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    type typeName
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameArrayInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array arrayName { Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameMapInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map mapName { Std::String, Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameFuncInValid_FirstLower)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func funcName { value: Std::String -> Std::Integer }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameObjectInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    type Type_Name
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameArrayInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array Array_Name { Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameMapInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map Map_Name { Std::String, Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameFuncInValid_UnderScore)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func Func_Name { value: Std::String -> Std::Integer }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}


TEST(SemanticTest, TypeNameObjectInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    type Typ
    {
        name : Std::String;
        age  : Std::Integer;
    }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameArrayInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array Arr { Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameMapInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    map Map { Std::String, Std::Integer}
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, TypeNameFuncInValid_Short)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    func Fun { value: Std::String -> Std::Integer }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Type", result);
}

TEST(SemanticTest, ObjectNameVariableValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    value : Std::String;
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE(compiler.go());
}

TEST(SemanticTest, ObjectNameParameterValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    value : func { name: Std::String, age: Std::Integer -> Std::String };
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE(compiler.go());
}

TEST(SemanticTest, ObjectNameVariableInValid_UpperFirst)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    Value : Std::String;
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Object", result);
}

TEST(SemanticTest, ObjectNameParameterInValid_UpperFirst)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    Value : func { name: Std::String, age: Std::Integer -> Std::String };
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Object", result);
}

TEST(SemanticTest, FindDeclInScopeValid)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::Integer }
}
)");

    Semantic          compiler(file, result);
    EXPECT_TRUE(compiler.go());
    //EXPECT_THROW_OR_DEBUG(compiler.go(), "Invalid Identifier for Object", result);
}

TEST(SemanticTest, FindDeclInScopeInValid_BadNameSpace)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Ftd::Integer }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Could Not Find Type", result);
}

TEST(SemanticTest, FindDeclInScopeInValid_BadType)
{
    using ThorsAnvil::Anvil::Ice::Semantic;
    std::stringstream   result;
    std::stringstream   file = buildStream(R"(
namespace Good_Name_Space
{
    array MyArray { Std::FFInteger }
}
)");

    Semantic          compiler(file, result);
    EXPECT_THROW_OR_DEBUG(compiler.go(), "Could Not Find Type", result);
}

