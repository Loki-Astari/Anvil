#include <gtest/gtest.h>

#include "Storage.h"
#include "Declaration/Namespace.h"
#include "test/Utility.h"

using namespace ThorsAnvil::Anvil::Ice;

TEST(StorageTest, AddOneValue)
{
    Storage     storage;

    Int index = storage.add(Int{5});
    EXPECT_NE(index, 0);

    SAccessInt      value(storage, index);
    EXPECT_EQ(value.get(), 5);
    EXPECT_EQ(index, 0);
}

TEST(StorageTest, AddTwo)
{
    Storage     storage;

    Int index1 = storage.add(Int{7});
    Int index2 = storage.add(Int{6});
    EXPECT_NE(index1, 0);
    EXPECT_NE(index2, 0);

    SAccessInt      value1(storage, index1);
    SAccessInt      value2(storage, index2);

    EXPECT_EQ(value1.get(), 7);
    EXPECT_EQ(value2.get(), 6);

    EXPECT_EQ(index1, 0);
    EXPECT_EQ(index2, 0);
}

TEST(StorageTest, AddOneReleaseViaAccessesAddOne)
{
    Storage     storage;

    Int index1 = storage.add(Int{7});
    Int saveI1 = index1;

    {
        SAccessInt      value1(storage, index1);
        EXPECT_EQ(value1.get(), 7);
    }

    Int index2 = storage.add(Int{6});

    EXPECT_EQ(saveI1, index2);
}

TEST(StorageTest, AddOneReleaseViaCallAddOne)
{
    Storage     storage;

    Int index1 = storage.add(Int{7});

    storage.release(index1);

    Int index2 = storage.add(Int{6});

    EXPECT_EQ(index1, index2);
}

TEST(StorageTest, ReleaseElementZero)
{
    Storage     storage;
    EXPECT_THROW(
        storage.release(0),
        std::runtime_error
    );
}

TEST(StorageTest, ReleaseElementBeyondRange)
{
    Storage     storage;
    EXPECT_THROW(
        storage.release(100),
        std::runtime_error
    );
}

TEST(StorageTest, GetElementZero)
{
    Storage     storage;

    auto action = [&]()
    {
        Int         index = 0;
        SAccessInt  value(storage, index);
    };
    EXPECT_THROW(
        action(),
        std::runtime_error
    );
}

TEST(StorageTest, GetElementBeyondRange)
{
    Storage     storage;

    auto action = [&]()
    {
        Int         index = 100;
        SAccessInt  value(storage, index);
    };
    EXPECT_THROW(
        action(),
        std::runtime_error
    );
}

TEST(StorageTest, StoreInt)
{
    Storage     storage;

    auto action = [&]()
    {
        Int index = storage.add(Int{1});

        SAccessInt  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, StoreTypeRef)
{
    Storage     storage;

    auto action = [&]()
    {
        Class    type("Type");
        Int index = storage.add(std::ref(dynamic_cast<Type&>(type)));

        SAccessType  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, StoreObjectRef)
{
    Storage     storage;

    auto action = [&]()
    {
        Class    type("Type");
        Object   object("object", type);
        Int index = storage.add(std::ref(object));

        SAccessObject  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, StoreDeclRef)
{
    Storage     storage;

    auto action = [&]()
    {
        Class    type("Type");
        Object   object("object", type);
        Int index = storage.add(std::ref(dynamic_cast<Decl&>(object)));

        SAccessDecl  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, StoreDeclRefNotObject)
{
    Storage     storage;

    auto action = [&]()
    {
        Class    type("Type");
        Object   object("object", type);
        Int index = storage.add(std::ref(dynamic_cast<Decl&>(object)));

        // put in as a decl.
        // You must access as a decl
        SAccessObject  value(storage, index);
    };
    EXPECT_THROW(
        action(),
        std::bad_variant_access
    );
}

TEST(StorageTest, StoreDeclRefNotType)
{
    Storage     storage;

    auto action = [&]()
    {
        Class    type("Type");
        Int index = storage.add(std::ref(dynamic_cast<Decl&>(type)));

        // put in as a decl.
        // You must access as a decl
        SAccessType  value(storage, index);
    };
    EXPECT_THROW(
        action(),
        std::bad_variant_access
    );
}

TEST(StorageTest, StoreScopeRef)
{
    Storage     storage;

    auto action = [&]()
    {
        Namespace   ns("Ns");
        Int index = storage.add(std::ref(dynamic_cast<Scope&>(ns)));

        SAccessScope  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, StoreParamList)
{
    Storage     storage;

    auto action = [&]()
    {
        ParamList   paramList;
        Int index = storage.add(paramList);

        SAccessParamList  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, StoreObjectList)
{
    Storage     storage;

    auto action = [&]()
    {
        ExpressionList   paramList;
        Int index = storage.add(paramList);

        SAccessExpressionList  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, ObjectId)
{
    Storage     storage;

    auto action = [&]()
    {
        Class      type("T");
        Object     object("M", type);
        ObjectId   objectId("Name", object, {}, type);
        Expression& e = objectId;
        Int index = storage.add(ExpressionRef(e));

        SAccessExpression  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, IdentifierList)
{
    Storage     storage;

    auto action = [&]()
    {
        IdentifierList   list{"XX", "YY"};
        Int index = storage.add(list);

        SAccessIdentifierList  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

TEST(StorageTest, StoreString)
{
    Storage     storage;

    auto action = [&]()
    {
        std::string   name;
        Int index = storage.add(name);

        SAccessString  value(storage, index);
    };
    EXPECT_NO_THROW(
        action();
    );
}

