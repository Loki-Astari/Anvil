#include <gtest/gtest.h>

#include "Declaration/StandardScope.h"
#include "Declaration/Namespace.h"
#include "Declaration/Types.h"
#include "Declaration/Objects.h"
#include "Declaration/Code.h"
#include "Declaration/Inline.h"

#include "test/Utility.h"

using namespace ThorsAnvil::Anvil::Ice;

TEST(DeclarationsTest, DeclNamespace)
{
    Namespace   ns("Name");
    Class& member = ns.add<Class>("Member");

    EXPECT_EQ(ns.declName(), "Name");
    EXPECT_EQ(ns.contName(), "Name");
    EXPECT_EQ(ns.declType(), DeclType::Namespace);

    auto find1 = ns.find("Member");
    ASSERT_TRUE(find1.first);
    EXPECT_EQ(dynamic_cast<Class*>(find1.second->second.get()), &member);

    auto find2 = ns.find("NotMember");
    EXPECT_FALSE(find2.first);
}

TEST(DeclarationsTest, DeclClass)
{
    Class   cs("Name");
    Class& member = cs.add<Class>("Member");

    EXPECT_EQ(cs.declName(), "Name");
    EXPECT_EQ(cs.contName(), "Name");
    EXPECT_EQ(cs.declType(), DeclType::Class);

    auto find1 = cs.find("Member");
    ASSERT_TRUE(find1.first);
    EXPECT_EQ(dynamic_cast<Class*>(find1.second->second.get()), &member);

    auto find2 = cs.find("NotMember");
    EXPECT_FALSE(find2.first);
}

TEST(DeclarationsTest, DeclArray)
{
    Class   typeInfo("Type");
    Array   array("Name", typeInfo);

    EXPECT_EQ(array.declName(), "Name");
    EXPECT_EQ(array.declType(), DeclType::Array);
}

TEST(DeclarationsTest, DeclMap)
{
    Class   key("Key");
    Class   value("Value");
    Map   map("Name", key, value);

    EXPECT_EQ(map.declName(), "Name");
    EXPECT_EQ(map.declType(), DeclType::Map);
}

TEST(DeclarationsTest, DeclFunc)
{   
    ParamList   param;
    Class       ret("Ret");
    Func        func("Name",param, ret);

    EXPECT_EQ(func.declName(), "Name");
    EXPECT_EQ(func.declType(), DeclType::Func);
}

TEST(DeclarationsTest, DeclVoid)
{
    Void   voidO;

    EXPECT_EQ(voidO.declType(), DeclType::Void);
}

TEST(DeclarationsTest, DeclObject)
{
    Class   type("Type");
    Class& member = type.add<Class>("Member");
    Object  object("Name", type);

    EXPECT_EQ(object.declName(), "Name");
    EXPECT_EQ(object.declType(), DeclType::Object);
    EXPECT_EQ(&object.getType(), dynamic_cast<Type*>(&type));

    auto find1 = object.find("Member");
    ASSERT_TRUE(find1.first);
    EXPECT_EQ(dynamic_cast<Class*>(find1.second->second.get()), &member);

    auto find2 = object.find("NotMember");
    EXPECT_FALSE(find2.first);
}

TEST(DeclarationsTest, DeclLiteral)
{
    Class   type("Type");
    Class& member = type.add<Class>("Member");
    Literal<std::string>   literal("Name", type, std::string("Hi There"));

    EXPECT_EQ(literal.declName(), "Name");
    EXPECT_EQ(literal.declType(), DeclType::Object);
    EXPECT_EQ(&literal.getType(), dynamic_cast<Type*>(&type));

    auto find1 = literal.find("Member");
    ASSERT_TRUE(find1.first);
    EXPECT_EQ(dynamic_cast<Class*>(find1.second->second.get()), &member);

    auto find2 = literal.find("NotMember");
    EXPECT_FALSE(find2.first);
}

TEST(DeclarationsTest, DeclCodeBlock)
{
    CodeBlock   codeblock("Name");
    Class& member = codeblock.add<Class>("Member");

    EXPECT_EQ(codeblock.declName(), "Name");
    EXPECT_EQ(codeblock.contName(), "Name");
    EXPECT_EQ(codeblock.declType(), DeclType::CodeBlock);

    auto find1 = codeblock.find("Member");
    ASSERT_TRUE(find1.first);
    EXPECT_EQ(dynamic_cast<Class*>(find1.second->second.get()), &member);

    auto find2 = codeblock.find("NotMember");
    EXPECT_FALSE(find2.first);
}

TEST(DeclarationsTest, DeclCodeBlockAddCodeInitObject)
{
    CodeBlock       codeBlock("Name");
    Class           type("Type");
    Object          object("Object",type);
    ObjectIdList    param;
    codeBlock.addCode<true, StatExprFunctionCall>(ObjectId(object, IdentifierList{}, type), std::move(param));
}

TEST(DeclarationsTest, DeclCodeBlockAddCodeFunctionCall)
{
    CodeBlock       codeBlock("Name");
    Class           type("Type");
    Object          object("Object",type);
    ObjectIdList    param;
    codeBlock.addCode<true, StatExprFunctionCall>(ObjectId(object, IdentifierList{}, type), std::move(param));
}


TEST(DeclarationsTest, DeclStandardScope)
{
    StandardScope       standardScope;

    auto findVoid = standardScope.find("Void");
    EXPECT_TRUE(findVoid.first);

    auto findSys = standardScope.find("Sys");
    EXPECT_TRUE(findSys.first);

    auto findStd = standardScope.find("Std");
    EXPECT_TRUE(findStd.first);


    ASSERT_TRUE(findVoid.first && findSys.first && findStd.first);

    Namespace*      sys = dynamic_cast<Namespace*>(findSys.second->second.get());
    Namespace*      std = dynamic_cast<Namespace*>(findStd.second->second.get());

    EXPECT_NE(sys, nullptr);
    EXPECT_NE(std, nullptr);

    ASSERT_TRUE((sys != nullptr) && (std != nullptr));

    auto findConsole = sys->find("console");
    EXPECT_TRUE(findConsole.first);

    auto findInteger = std->find("Integer");
    EXPECT_TRUE(findInteger.first);

    auto findString = std->find("String");
    EXPECT_TRUE(findString.first);

}
