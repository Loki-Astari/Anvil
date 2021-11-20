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
    type object ObjectName
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
    type array ArrayName { Std::Integer}
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
    type map MapName { Std::String, Std::Integer}
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
    type func FuncName { value: Std::String -> Std::Integer }
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
    type object objectName
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
    type array arrayName { Std::Integer}
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
    type map mapName { Std::String, Std::Integer}
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
    type func funcName { value: Std::String -> Std::Integer }
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
    type object Object_Name
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
    type array Array_Name { Std::Integer}
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
    type map Map_Name { Std::String, Std::Integer}
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
    type func Func_Name { value: Std::String -> Std::Integer }
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
    type object Obj
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
    type array Arr { Std::Integer}
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
    type map Map { Std::String, Std::Integer}
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
    type func Fun { value: Std::String -> Std::Integer }
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



