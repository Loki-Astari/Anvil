#include <gtest/gtest.h>

#include "Declaration.h"
#include "Action.h"

using namespace ThorsAnvil::Anvil::Ice;

TEST(DeclarationPrintTest, ScopePrint)
{
    Scope   scope(nullptr);
    std::string output = (std::stringstream{} << scope).str();
    auto find = output.find("Decl::Scope\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ScopeMember)
{
    Scope   scope(nullptr);
    scope.add<Class>(nullptr, "Class");
    std::string output = (std::stringstream{} << scope).str();
    auto find = output.find("Decl::Scope\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, CodeBlock)
{
    CodeBlock   codeBlock(nullptr);
    std::string output = (std::stringstream{} << codeBlock).str();
    auto find = output.find("Decl::Scope::CodeBlock\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, MemberInitEmpty)
{
    MemberInit   memberInit(nullptr, "Member", ExpressionList{});
    std::string output = (std::stringstream{} << memberInit).str();
    auto find = output.find("Decl::MemberInit\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, MemberInitEmptyExpression)
{
    ExpressionLiteral<void*>  value(nullptr, nullptr);
    MemberInit   memberInit(nullptr, "Member", ExpressionList{ExpressionRef{value}});
    std::string output = (std::stringstream{} << memberInit).str();
    auto find = output.find("Decl::MemberInit\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, NamedScope)
{
    NamedScope   namedScope(nullptr, "NamedScope");
    std::string output = (std::stringstream{} << namedScope).str();
    auto find = output.find("Decl::Scope::NamedScope\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Namespace)
{
    Namespace   ns(nullptr, "NamedScope");
    std::string output = (std::stringstream{} << ns).str();
    auto find = output.find("Decl::Scope::NamedScope::Namespace\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Type)
{
    Type   type(nullptr, "Type");
    std::string output = (std::stringstream{} << type).str();
    auto find = output.find("Decl::Scope::NamedScope::Type\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Void)
{
    Void   voidType(nullptr);
    std::string output = (std::stringstream{} << voidType).str();
    auto find = output.find("Decl::Scope::NamedScope::Type::Void\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Class)
{
    Class   classType(nullptr, "Class");
    std::string output = (std::stringstream{} << classType).str();
    auto find = output.find("Decl::Scope::NamedScope::Type::Class\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Function)
{
    Void       voidType(nullptr);
    Function   funcType(nullptr, "Function", TypeCList{}, voidType);
    std::string output = (std::stringstream{} << funcType).str();
    auto find = output.find("Decl::Scope::NamedScope::Type::Function\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, FunctionParam)
{
    Void       voidType(nullptr);
    Function   funcType(nullptr, "Function", TypeCList{TypeCRef{voidType}}, voidType);
    std::string output = (std::stringstream{} << funcType).str();
    auto find = output.find("Decl::Scope::NamedScope::Type::Function\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Overload)
{
    Overload   overload(nullptr);
    std::string output = (std::stringstream{} << overload).str();
    auto find = output.find("Decl::Scope::NamedScope::Type::Overload\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, OverloadWithFunc)
{
    Void       voidType(nullptr);
    Function   funcType(nullptr, "Function", TypeCList{TypeCRef{voidType}}, voidType);
    Overload   overload(nullptr);
    overload.addOverload(funcType);
    std::string output = (std::stringstream{} << overload).str();
    auto find = output.find("Decl::Scope::NamedScope::Type::Overload\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Object)
{
    Void       voidType(nullptr);
    Object     object(nullptr, "Object", voidType);
    std::string output = (std::stringstream{} << object).str();
    auto find = output.find("Decl::Object\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectVariable)
{
    Void            voidType(nullptr);
    ObjectVariable  variable(nullptr, "Object", voidType, ExpressionList{});
    std::string output = (std::stringstream{} << variable).str();
    auto find = output.find("Decl::Object::ObjectVariable\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectVariableWithInit)
{
    Void                        voidType(nullptr);
    ExpressionLiteral<void*>    value(nullptr, nullptr);
    ObjectVariable              variable(nullptr, "Object", voidType, ExpressionList{ExpressionRef{value}});
    std::string output = (std::stringstream{} << variable).str();
    auto find = output.find("Decl::Object::ObjectVariable\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectFunction)
{
    Void                        voidType(nullptr);
    ExpressionLiteral<void*>    value(nullptr, nullptr);
    CodeBlock                   codeBlock(nullptr);
    StatementCodeBlock          code(nullptr, codeBlock, StatementList{});
    ObjectFunction              function(nullptr, "Function", voidType, code);
    std::string output = (std::stringstream{} << function).str();
    auto find = output.find("Decl::Object::ObjectFunction\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectFunctionSpecial)
{
    Void                        voidType(nullptr);
    ExpressionLiteral<void*>    value(nullptr, nullptr);
    CodeBlock                   codeBlock(nullptr);
    StatementCodeBlock          code(nullptr, codeBlock, StatementList{});
    ObjectFunctionSpecial       function(nullptr, "Function", voidType, MemberInitList{}, code);
    std::string output = (std::stringstream{} << function).str();
    auto find = output.find("Decl::Object::ObjectFunction::ObjectFunctionSpecial\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectFunctionSpecialWithInit)
{
    Void                        voidType(nullptr);
    ExpressionLiteral<void*>    value(nullptr, nullptr);
    CodeBlock                   codeBlock(nullptr);
    StatementCodeBlock          code(nullptr, codeBlock, StatementList{});
    MemberInit                  member(nullptr, "Member", ExpressionList{});
    ObjectFunctionSpecial       function(nullptr, "Function", voidType, MemberInitList{MemberInitRef{member}}, code);
    std::string output = (std::stringstream{} << function).str();
    auto find = output.find("Decl::Object::ObjectFunction::ObjectFunctionSpecial\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectFunctionConstructor)
{
    Void                        voidType(nullptr);
    ExpressionLiteral<void*>    value(nullptr, nullptr);
    CodeBlock                   codeBlock(nullptr);
    StatementCodeBlock          code(nullptr, codeBlock, StatementList{});
    MemberInit                  member(nullptr, "Member", ExpressionList{});
    ObjectFunctionConstructor   function(nullptr, "Function", voidType, MemberInitList{MemberInitRef{member}}, code);
    std::string output = (std::stringstream{} << function).str();
    auto find = output.find("Decl::Object::ObjectFunction::ObjectFunctionSpecial::ObjectFunctionConstructor\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectFunctionDestructor)
{
    Void                        voidType(nullptr);
    ExpressionLiteral<void*>    value(nullptr, nullptr);
    CodeBlock                   codeBlock(nullptr);
    StatementCodeBlock          code(nullptr, codeBlock, StatementList{});
    MemberInit                  member(nullptr, "Member", ExpressionList{});
    ObjectFunctionDestructor    function(nullptr, "Function", voidType, MemberInitList{MemberInitRef{member}}, code);
    std::string output = (std::stringstream{} << function).str();
    auto find = output.find("Decl::Object::ObjectFunction::ObjectFunctionSpecial::ObjectFunctionDestructor\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectOverload)
{
    ObjectOverload    function(nullptr, "function");
    std::string output = (std::stringstream{} << function).str();
    auto find = output.find("Decl::Object::ObjectOverload\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ObjectOverloadWithMember)
{
    Void                voidType(nullptr);
    Function            functionType(nullptr, "Function", TypeCList{}, voidType);
    Class               classType(nullptr, "Class");
    CodeBlock           codeBlock(nullptr);
    StatementCodeBlock  code(nullptr, codeBlock, StatementList{});
    ObjectOverload      functionOverload(nullptr, "function");
    std::unique_ptr<Decl> function = std::make_unique<ObjectFunction>(nullptr, "function", functionType, code);
    functionOverload.addOverload(std::move(function));

    std::string output = (std::stringstream{} << functionOverload).str();
    auto find = output.find("Decl::Object::ObjectOverload\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, Statement)
{
    Statement    statement(nullptr);
    std::string output = (std::stringstream{} << statement).str();
    auto find = output.find("Decl::Statement\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, StatementExpression)
{
    ExpressionLiteral<void*>  value(nullptr, nullptr);
    StatementExpression    statement(nullptr, value);
    std::string output = (std::stringstream{} << statement).str();
    auto find = output.find("Decl::Statement::StatementExpression\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, StatementReturn)
{
    ExpressionLiteral<void*>  value(nullptr, nullptr);
    StatementReturn    statement(nullptr, value);
    std::string output = (std::stringstream{} << statement).str();
    auto find = output.find("Decl::Statement::StatementReturn\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, StatementAssembley)
{
    StatementAssembley    statement(nullptr, "Assembley Instructions");
    std::string output = (std::stringstream{} << statement).str();
    auto find = output.find("Decl::Statement::StatementAssembley\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, StatementCodeBlock)
{
    CodeBlock           codeBlock(nullptr);
    StatementCodeBlock  statement(nullptr, codeBlock, StatementList{});
    std::string output = (std::stringstream{} << statement).str();
    auto find = output.find("Decl::Statement::StatementCodeBlock\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, StatementCodeBlockWithStatment)
{
    ExpressionLiteral<void*>  value(nullptr, nullptr);
    StatementReturn    statement(nullptr, value);

    CodeBlock           codeBlock(nullptr);
    StatementCodeBlock  statementBlock(nullptr, codeBlock, StatementList{StatementRef{statement}});
    std::string output = (std::stringstream{} << statementBlock).str();
    auto find = output.find("Decl::Statement::StatementCodeBlock\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ExpressionObject)
{
    Void              voidType(nullptr);
    Object            object(nullptr, "Object", voidType);
    ExpressionObject  expression(nullptr, object);
    std::string output = (std::stringstream{} << expression).str();
    auto find = output.find("Decl::Expression::ExpressionObject\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ExpressionMemberAccess)
{
    Void              voidType(nullptr);
    Class             classType(nullptr, "Class");
    classType.add<ObjectVariable>(nullptr, "data", voidType, ExpressionList{});
    Object            object(nullptr, "Object", classType);

    ExpressionObject  expressionObject(nullptr, object);
    ExpressionMemberAccess  expression(nullptr, expressionObject, "data");
    std::string output = (std::stringstream{} << expression).str();
    auto find = output.find("Decl::Expression::ExpressionMemberAccess\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ExpressionFuncCall)
{
    Void                voidType(nullptr);
    Function            functionType(nullptr, "Function", TypeCList{}, voidType);
    Class               classType(nullptr, "Class");
    CodeBlock           codeBlock(nullptr);
    StatementCodeBlock  code(nullptr, codeBlock, StatementList{});
    ObjectOverload&     functionOverload = classType.add<ObjectOverload>(nullptr, "function");

    std::unique_ptr<Decl> function = std::make_unique<ObjectFunction>(nullptr, "function", functionType, code);
    functionOverload.addOverload(std::move(function));

    Object            object(nullptr, "Object", classType);

    ExpressionObject  expressionObject(nullptr, object);
    ExpressionMemberAccess  expressionAccess(nullptr, expressionObject, "function");
    ExpressionFuncCall      expression(nullptr, expressionAccess, ExpressionList{});
    std::string output = (std::stringstream{} << expression).str();
    auto find = output.find("Decl::Expression::ExpressionFuncCall\n");
    EXPECT_TRUE(find != std::string::npos);
}

TEST(DeclarationPrintTest, ExpressionFuncCallWithParam)
{
    Void                voidType(nullptr);
    Function            functionType(nullptr, "Function", TypeCList{TypeCRef{voidType}, TypeCRef{voidType}}, voidType);
    Class               classType(nullptr, "Class");
    CodeBlock           codeBlock(nullptr);
    StatementCodeBlock  code(nullptr, codeBlock, StatementList{});
    ObjectOverload&     functionOverload = classType.add<ObjectOverload>(nullptr, "function");

    std::unique_ptr<Decl> function = std::make_unique<ObjectFunction>(nullptr, "function", functionType, code);
    functionOverload.addOverload(std::move(function));

    Object              object(nullptr, "Object", classType);
    Object              param(nullptr, "Param", voidType);

    ExpressionObject        expressionObject(nullptr, object);
    ExpressionObject        expressionParam(nullptr, param);
    ExpressionMemberAccess  expressionAccess(nullptr, expressionObject, "function");
    ExpressionFuncCall      expression(nullptr, expressionAccess, ExpressionList{ExpressionRef{expressionParam}, ExpressionRef{expressionParam}});
    std::string output = (std::stringstream{} << expression).str();
    auto find = output.find("Decl::Expression::ExpressionFuncCall\n");
    EXPECT_TRUE(find != std::string::npos);
}



