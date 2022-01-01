#include <gtest/gtest.h>

#include "test/Utility.h"

TEST(ExpressionTest, ExpressionAssign)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data = 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssign", result);
}

TEST(ExpressionTest, ExpressionAssignMul)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data *= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignMul", result);
}

TEST(ExpressionTest, ExpressionAssignDiv)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data /= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignDiv", result);
}

TEST(ExpressionTest, ExpressionAssignMod)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data %= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignMod", result);
}

TEST(ExpressionTest, ExpressionAssignAdd)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data += 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignAdd", result);
}

TEST(ExpressionTest, ExpressionAssignSub)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data -= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignSub", result);
}

TEST(ExpressionTest, ExpressionAssignLSh)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data <<= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignLSh", result);
}

TEST(ExpressionTest, ExpressionAssignRSh)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data >>= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignRSh", result);
}

TEST(ExpressionTest, ExpressionAssignAnd)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data &= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignAnd", result);
}

TEST(ExpressionTest, ExpressionAssignXOR)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data ^= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignXOR", result);
}

TEST(ExpressionTest, ExpressionAssignOR)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data |= 12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAssignOR", result);
}

TEST(ExpressionTest, ExpressionConditional)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        10 ? 12 : 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionConditional", result);
}

TEST(ExpressionTest, ExpressionLogicalOr)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 || 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionLogicalOr", result);
}

TEST(ExpressionTest, ExpressionLogocalAnd)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 && 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionLogicalAnd", result);
}

TEST(ExpressionTest, ExpressionInclusiveOr)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 | 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionInclusiveOr", result);
}

TEST(ExpressionTest, ExpressionExclusiveOr)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 ^ 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionExclusiveOr", result);
}

TEST(ExpressionTest, ExpressionAnd)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 & 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAnd", result);
}

TEST(ExpressionTest, ExpressionEqual)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 == 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionEqual", result);
}

TEST(ExpressionTest, ExpressionNotEqual)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 != 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionNotEqual", result);
}

TEST(ExpressionTest, ExpressionLess)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 < 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionLess", result);
}

TEST(ExpressionTest, ExpressionGreat)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 > 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionGreat", result);
}

TEST(ExpressionTest, ExpressionLessEqual)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 <= 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionLessEqual", result);
}

TEST(ExpressionTest, ExpressionGreatEqual)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 >= 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionGreatEqual", result);
}

TEST(ExpressionTest, ExpressionShiftLeft)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 << 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionShiftLeft", result);
}

TEST(ExpressionTest, ExpressionShigtRight)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 >> 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionShiftRight", result);
}

TEST(ExpressionTest, ExpressionAdd)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 + 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionAdd", result);
}

TEST(ExpressionTest, ExpressionSub)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 - 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionSub", result);
}

TEST(ExpressionTest, ExpressionMul)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 * 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionMul", result);
}

TEST(ExpressionTest, ExpressionDiv)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 / 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionDiv", result);
}

TEST(ExpressionTest, ExpressionMod)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12 % 13;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionMod", result);
}

TEST(ExpressionTest, ExpressionPreInc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        ++12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionPreInc", result);
}

TEST(ExpressionTest, ExpressionPreDec)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        --12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionPreDec", result);
}

TEST(ExpressionTest, ExpressionPlus)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        +12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionPlus", result);
}

TEST(ExpressionTest, ExpressionNeg)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        -12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionNeg", result);
}

TEST(ExpressionTest, ExpressionOneCompliment)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        ~12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionOneCompliment", result);
}

TEST(ExpressionTest, ExpressionNot)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        !12;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionNot", result);
}

TEST(ExpressionTest, ExpressionArrayAccess)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12[13];
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionArrayAccess", result);
}

TEST(ExpressionTest, ExpressionPostDec)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12--;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionPostDec", result);
}

TEST(ExpressionTest, ExpressionPostInc)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{} }
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        12++;
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_THROW_OR_DEBUG(compiler.compile(), "Not Implemented: expressionPostInc", result);
}

// Object as expression.
// Object member access
// method Call
TEST(ExpressionTest, ExpressionBasicTest)
{
    std::stringstream    result;
    std::stringstream    file = buildStream(R"(
namespace Std {class Integer{ test : func {Integer->Void}{} }}
namespace Name_Space
{
    data : Std::Integer;
    main : func {Void -> Std::Integer}
    {
        data.test(12);
    }
}
    )");

    FacadeCompiler  compiler(file, result);

    EXPECT_TRUE_OR_DEBUG(compiler.compile(), result);
}

#if 0

        // Expression:
        ExpressionId        expressionAssign(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignMul(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignDiv(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignMod(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignAdd(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignSub(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignLSh(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignRSh(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignAnd(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignXOR(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignOR(ExpressionId, ExpressionId);
        ExpressionId        expressionConditional(ExpressionId, ExpressionId, ExpressionId);
        ExpressionId        expressionLogicalOr(ExpressionId, ExpressionId);
        ExpressionId        expressionLogicalAnd(ExpressionId, ExpressionId);
        ExpressionId        expressionInclusiveOr(ExpressionId, ExpressionId);
        ExpressionId        expressionExclusiveOr(ExpressionId, ExpressionId);
        ExpressionId        expressionAnd(ExpressionId, ExpressionId);
        ExpressionId        expressionEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionNotEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionLess(ExpressionId, ExpressionId);
        ExpressionId        expressionGreat(ExpressionId, ExpressionId);
        ExpressionId        expressionLessEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionGreatEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionShiftLeft(ExpressionId, ExpressionId);
        ExpressionId        expressionShiftRight(ExpressionId, ExpressionId);
        ExpressionId        expressionAdd(ExpressionId, ExpressionId);
        ExpressionId        expressionSub(ExpressionId, ExpressionId);
        ExpressionId        expressionMul(ExpressionId, ExpressionId);
        ExpressionId        expressionDiv(ExpressionId, ExpressionId);
        ExpressionId        expressionMod(ExpressionId, ExpressionId);
        ExpressionId        expressionPreInc(ExpressionId);
        ExpressionId        expressionPreDec(ExpressionId);
        ExpressionId        expressionPlus(ExpressionId);
        ExpressionId        expressionNeg(ExpressionId);
        ExpressionId        expressionOneCompliment(ExpressionId);
        ExpressionId        expressionNot(ExpressionId);
        ExpressionId        expressionArrayAccess(ExpressionId, ExpressionId);
        ExpressionId        expressionFuncCall(ExpressionId id, ExpressionListId list);
        ExpressionId        expressionMemberAccess(ExpressionId id, IdentifierId mem);
        ExpressionId        expressionPostInc(ExpressionId);
        ExpressionId        expressionPostDec(ExpressionId);
        ExpressionId        expressionObject(ObjectId id);
        ExpressionId        expressionLiteralString();
        ExpressionId        expressionLiteralInt();
#endif
