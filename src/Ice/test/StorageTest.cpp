#include <gtest/gtest.h>

#include "Action.h"
#include "StorageAccess.h"

using namespace ThorsAnvil::Anvil::Ice;

TEST(StorageTest, Int)
{
    Storage     store;
    Id<Int> id(-1);
    Int     save;
    {
        Int         value = 5;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Int>   access(store, id);
        EXPECT_EQ(static_cast<Int>(access), 5);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, IntWithReuse)
{
    Storage     store;
    Id<Int> id(-1);
    Int     save;
    {
        Int         value = 5;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Int>   access(store, id);
        EXPECT_EQ(static_cast<Int>(access), 5);
        EXPECT_EQ(access.reuse(), save);
        EXPECT_EQ(id.value, 0);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(store.get<Int>(save), 5);
}

TEST(StorageTest, DeclList)
{
    Storage     store;
    Id<DeclList> id(-1);
    Int     save;
    {
        DeclList         value;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<DeclList>   access(store, id);
        DeclList result = access;
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}


TEST(StorageTest, NamespaceList)
{
    Storage     store;
    Id<NamespaceList> id(-1);
    Int     save;
    {
        NamespaceList         value;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<NamespaceList>   access(store, id);
        NamespaceList result = access;
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, TypeCList)
{
    Storage     store;
    Id<TypeCList> id(-1);
    Int     save;
    {
        TypeCList         value;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<TypeCList>   access(store, id);
        TypeCList result = access;
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, ExpressionList)
{
    Storage     store;
    Id<ExpressionList> id(-1);
    Int     save;
    {
        ExpressionList         value;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<ExpressionList>   access(store, id);
        ExpressionList result = access;
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, StatementList)
{
    Storage     store;
    Id<StatementList> id(-1);
    Int     save;
    {
        StatementList         value;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<StatementList>   access(store, id);
        StatementList result = access;
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, MemberInitList)
{
    Storage     store;
    Id<MemberInitList> id(-1);
    Int     save;
    {
        MemberInitList         value;
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<MemberInitList>   access(store, id);
        MemberInitList result = access;
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Decl)
{
    Storage     store;
    Id<Decl> id(-1);
    Int     save;
    {
        Void&   value = Void::getInstance();
        id = store.add(DeclRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Decl>   access(store, id);
        Decl& result = access;
        EXPECT_EQ(&result, &dynamic_cast<Decl&>(value));
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Scope)
{
    Storage     store;
    Id<Scope> id(-1);
    Int     save;
    {
        Void&   value = Void::getInstance();
        id = store.add(ScopeRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Scope>   access(store, id);
        Scope& result = access;
        EXPECT_EQ(&result, &dynamic_cast<Scope&>(value));
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, CodeBlock)
{
    Storage     store;
    Id<CodeBlock> id(-1);
    Int     save;
    {
        CodeBlock    value{nullptr};
        id = store.add(CodeBlockRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<CodeBlock>   access(store, id);
        CodeBlock& result = access;
        EXPECT_EQ(&result, &value);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, MemberInit)
{
    Storage     store;
    Id<MemberInit> id(-1);
    Int     save;
    {
        MemberInit    value{nullptr, "data", ExpressionList{}};
        id = store.add(MemberInitRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<MemberInit>   access(store, id);
        MemberInit& result = access;
        EXPECT_EQ(&result, &value);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Namespace)
{
    Storage     store;
    Id<Namespace> id(-1);
    Int     save;
    {
        Namespace    value{nullptr, "NS"};
        id = store.add(NamespaceRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Namespace>   access(store, id);
        Namespace& result = access;
        EXPECT_EQ(&result, &value);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Type)
{
    Storage     store;
    Id<Type> id(-1);
    Int     save;
    {
        Void&   value = Void::getInstance();
        id = store.add(TypeRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Type>   access(store, id);
        Type& result = access;
        EXPECT_EQ(&result, &dynamic_cast<Type&>(value));
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Class)
{
    Storage     store;
    Id<Class> id(-1);
    Int     save;
    {
        Class    value{nullptr, "Class"};
        id = store.add(ClassRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Class>   access(store, id);
        Class& result = access;
        EXPECT_EQ(&result, &value);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Function)
{
    Storage     store;
    Id<Function> id(-1);
    Int     save;
    {
        Void&       voidType = Void::getInstance();
        Function    value{nullptr, "Function", TypeCList{}, voidType};
        id = store.add(FunctionRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Function>   access(store, id);
        Function& result = access;
        EXPECT_EQ(&result, &value);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Object)
{
    Storage     store;
    Id<Object> id(-1);
    Int     save;
    {
        Void&       voidType = Void::getInstance();
        ObjectVariable    value{nullptr, "data", voidType, ExpressionList{}};
        id = store.add(ObjectRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Object>   access(store, id);
        Object& result = access;
        EXPECT_EQ(&result, &dynamic_cast<Object&>(value));
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Statement)
{
    Storage     store;
    Id<Statement> id(-1);
    Int     save;
    {
        CodeBlock             codeBlock(nullptr);
        StatementCodeBlock    value{nullptr, codeBlock, StatementList{}};
        id = store.add(StatementRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Statement>   access(store, id);
        Statement& result = access;
        EXPECT_EQ(&result, &dynamic_cast<StatementCodeBlock&>(value));
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, StatementCodeBlock)
{
    Storage     store;
    Id<StatementCodeBlock> id(-1);
    Int     save;
    {
        CodeBlock             codeBlock(nullptr);
        StatementCodeBlock    value{nullptr, codeBlock, StatementList{}};
        id = store.add(StatementCodeBlockRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<StatementCodeBlock>   access(store, id);
        StatementCodeBlock& result = access;
        EXPECT_EQ(&result, &value);
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Expression)
{
    Storage     store;
    Id<Expression> id(-1);
    Int     save;
    {
        Void&               voidType = Void::getInstance();
        Object              object(nullptr, "Data", voidType);
        ExpressionObject    value{nullptr, object};
        id = store.add(ExpressionRef(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Expression>   access(store, id);
        Expression& result = access;
        EXPECT_EQ(&result, &dynamic_cast<Expression&>(value));
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

TEST(StorageTest, Identifier)
{
    Storage     store;
    Id<Identifier> id(-1);
    Int     save;
    {
        Identifier    value = "Name";
        id = store.add(std::move(value));
        save = id.value;
        EXPECT_NE(id.value, -1);

        IdAccess<Identifier>   access(store, id);
        Identifier& result = access;
        EXPECT_EQ(result, "Name");
    }
    // The IdAccess object should reset the id back to zero
    EXPECT_EQ(id.value, 0);
    EXPECT_EQ(store.get<Int>(save), 0);
}

