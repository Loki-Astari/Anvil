#include <gtest/gtest.h>

#include "Memory.h"
#include "../Ice/test/Utility.h"

using namespace ThorsAnvil::Anvil::Fire;

TEST(MemoryTest, MemoryLocationDefaultsToVoid)
{
    MemoryLocation  m1;

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::Void);
}

TEST(MemoryTest, InitMemoryLocationWithVoid)
{
    MemoryLocation  m1;
    m1.init(CodeAddress{});
    m1.init(Void{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::Void);
}

TEST(MemoryTest, InitMemoryLocationWithCodeAddress)
{
    MemoryLocation  m1;

    m1.init(CodeAddress{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::CodeAddress);
}

TEST(MemoryTest, InitMemoryLocationWithDataAddress)
{
    MemoryLocation  m1;

    m1.init(DataAddress{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::DataAddress);
}

TEST(MemoryTest, InitMemoryLocationWithDataFrame)
{
    MemoryLocation  m1;

    m1.init(DataFrame{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::DataFrame);
}

TEST(MemoryTest, InitMemoryLocationWithInt)
{
    MemoryLocation  m1;

    m1.init(Int{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::Int);
}

TEST(MemoryTest, InitMemoryLocationWithString)
{
    MemoryLocation  m1;

    m1.init(String{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::String);
}

TEST(MemoryTest, InitMemoryLocationWithArray)
{
    MemoryLocation  m1;

    m1.init(Array{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::Array);
}

TEST(MemoryTest, InitMemoryLocationWithMap)
{
    MemoryLocation  m1;

    m1.init(Map{});

    MemoryType  type = m1.visit(MemoryLocationVisitorCheckType{});
    ASSERT_EQ(type, MemoryType::Map);
}

