#include <gtest/gtest.h>

#include "Memory.h"

using namespace ThorsAnvil::Anvil::Fire;


#if 0
TEST(MemoryTest, MakeSureAddressGetBehaves)
{

    Result      output = vm.run();

    EXPECT_EQ_OR_LOG(bad, output, 16, result);
    bool isIntAddress = true;
    vm.machineState.stack[0].getAddress([&](){isIntAddress = false;});
    bool isDataFrameAddress = true;
    vm.machineState.stack[1].getAddress([&](){isDataFrameAddress = false;});
    bool isDataAddressAddress = true;
    vm.machineState.stack[2].getAddress([&](){isDataAddressAddress = false;});

    EXPECT_EQ_OR_LOG(bad, isIntAddress, false, result);
    EXPECT_EQ_OR_LOG(bad, isDataFrameAddress, true, result);
    EXPECT_EQ_OR_LOG(bad, isDataAddressAddress, true, result);
    EXPECT_SUCC(bad, result);
}

    using Object = std::variant<Void,
                                CodeAddress,
                                DataAddress,
                                DataFrame,
                                Int,                    // Std::Integer
                                String,                 // Std::String
                                Array,
                                Map
                               >;
#endif

TEST(MemoryTest, MemoryDefaultVoid)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.getValue<Void>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataFrame>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getAddress(badAction);
    EXPECT_FALSE(isGood);
}

TEST(MemoryTest, MemoryCodeAddress)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.init(CodeAddress{12});

    value.getValue<Void>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataFrame>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getAddress(badAction);
    EXPECT_FALSE(isGood);
}

TEST(MemoryTest, MemoryDataAddress)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.init(DataAddress{&value});

    value.getValue<Void>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getValue<DataFrame>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    auto v = value.getAddress(badAction);
    EXPECT_TRUE(isGood);
    EXPECT_EQ(&value, v);
}

TEST(MemoryTest, MemoryDataFrame)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.init(DataFrame{12});

    value.getValue<Void>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    DataFrame& v1 = value.getValue<DataFrame>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    auto v2 = value.getAddress(badAction);
    EXPECT_TRUE(isGood);
    EXPECT_EQ(&v1[0], v2);
}

TEST(MemoryTest, MemoryInt)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.init(Int{12});

    value.getValue<Void>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataFrame>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getAddress(badAction);
    EXPECT_FALSE(isGood);
}


TEST(MemoryTest, MemoryString)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.init(String{"Hi"});

    value.getValue<Void>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataFrame>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getAddress(badAction);
    EXPECT_FALSE(isGood);
}



TEST(MemoryTest, MemoryArray)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.init(Array{});

    value.getValue<Void>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataFrame>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getAddress(badAction);
    EXPECT_FALSE(isGood);
}



TEST(MemoryTest, MemoryMap)
{
    bool                isGood = true;
    auto badAction = [&](){isGood = false;};

    MemoryLocation      value;
    value.init(Map{});

    value.getValue<Void>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<CodeAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataAddress>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<DataFrame>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Int>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<String>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Array>(badAction);
    EXPECT_FALSE(isGood);

    isGood = true;
    value.getValue<Map>(badAction);
    EXPECT_TRUE(isGood);

    isGood = true;
    value.getAddress(badAction);
    EXPECT_FALSE(isGood);
}



