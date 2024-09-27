#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "static_allocator.h"

using namespace utils;

TEST(StaticAllocatorTest, InitializationTest)
{

    StaticAllocator<sizeof(int), 16> allocator;
    StaticAllocatorPool<int, 16> pool;
    const auto &poolAllocator = pool.getAllocator();

    EXPECT_EQ(poolAllocator.freeSlots(), 16);
    EXPECT_EQ(poolAllocator.allocatedSlots(), 0);
    EXPECT_EQ(poolAllocator.getNumAllocs(), 0);
    EXPECT_EQ(poolAllocator.getNumDeallocs(), 0);

    EXPECT_EQ(allocator.freeSlots(), 16);
    EXPECT_EQ(allocator.allocatedSlots(), 0);
    EXPECT_EQ(allocator.getNumAllocs(), 0);
    EXPECT_EQ(allocator.getNumDeallocs(), 0);

    EXPECT_EQ(poolAllocator.objectSize, sizeof(int));
    EXPECT_EQ(poolAllocator.elementSize, sizeof(int));
    EXPECT_EQ(poolAllocator.numObjects, 16);
    EXPECT_EQ(poolAllocator.memoryLength, 16 * sizeof(int));

    EXPECT_EQ(allocator.objectSize, sizeof(int));
    EXPECT_EQ(allocator.elementSize, sizeof(int));
    EXPECT_EQ(allocator.numObjects, 16);
    EXPECT_EQ(allocator.memoryLength, 16 * sizeof(int));

    EXPECT_GT(allocator.getEndMemory(), allocator.getStartMemory());
    EXPECT_EQ(allocator.getEndMemory(), (char *)allocator.getStartMemory() + allocator.memoryLength);

    EXPECT_GT(poolAllocator.getEndMemory(), poolAllocator.getStartMemory());
    EXPECT_EQ(poolAllocator.getEndMemory(), (char *)poolAllocator.getStartMemory() + poolAllocator.memoryLength);
    EXPECT_THROW(pool.deallocate(nullptr), std::invalid_argument);
    EXPECT_THROW(pool.deallocate((int *)poolAllocator.getStartMemory()), std::invalid_argument);
    EXPECT_THROW(pool.deallocate((int *)poolAllocator.getEndMemory()), std::invalid_argument);
}

template <typename T>
void _basicTypeTest(size_t elementSize)
{
    StaticAllocatorPool<T, 16> pool;
    const auto &poolAllocator = pool.getAllocator();

    EXPECT_EQ(poolAllocator.freeSlots(), 16);
    EXPECT_EQ(poolAllocator.allocatedSlots(), 0);
    EXPECT_EQ(poolAllocator.getNumAllocs(), 0);
    EXPECT_EQ(poolAllocator.getNumDeallocs(), 0);

    EXPECT_EQ(poolAllocator.objectSize, sizeof(T));
    EXPECT_EQ(poolAllocator.elementSize, elementSize);
    EXPECT_EQ(poolAllocator.numObjects, 16);
    EXPECT_EQ(poolAllocator.memoryLength, 16 * elementSize);

    T *allocated[16] = {nullptr};
    for (size_t i = 0; i < 16; i++)
    {
        EXPECT_EQ(poolAllocator.allocatedSlots(), i);
        EXPECT_EQ(poolAllocator.freeSlots(), 16 - i);
        allocated[i] = pool.allocate();
        EXPECT_NE(allocated[i], nullptr);
        EXPECT_EQ(poolAllocator.freeSlots(), 16 - i - 1);
        EXPECT_EQ(poolAllocator.getNumAllocs(), i + 1);
        EXPECT_EQ(poolAllocator.getNumDeallocs(), 0);

        if (i > 0)
        {

            EXPECT_NE(allocated[i], allocated[i - 1]);
            EXPECT_EQ((char *)allocated[i], (char *)allocated[i - 1] + 1 * elementSize);
        }
    }
    EXPECT_EQ(poolAllocator.freeSlots(), 0);
    EXPECT_EQ(poolAllocator.allocatedSlots(), 16);
    EXPECT_EQ(pool.allocate(), nullptr);

    pool.deallocate(allocated[8]);
    EXPECT_EQ(poolAllocator.freeSlots(), 1);
    EXPECT_EQ(poolAllocator.allocatedSlots(), 15);

    EXPECT_EQ(pool.allocate(), allocated[8]);
    EXPECT_EQ(poolAllocator.freeSlots(), 0);
    EXPECT_EQ(poolAllocator.allocatedSlots(), 16);

    EXPECT_EQ(poolAllocator.getNumAllocs(), 17);
    EXPECT_EQ(poolAllocator.getNumDeallocs(), 1);
    for (int i = 15; i >= 0; --i)
    {
        EXPECT_EQ(poolAllocator.allocatedSlots(), i + 1);
        EXPECT_EQ(poolAllocator.freeSlots(), 16 - i - 1);
        pool.deallocate(allocated[i]);
        EXPECT_EQ(poolAllocator.freeSlots(), 16 - i);
        EXPECT_EQ(poolAllocator.getNumAllocs(), 17);
        EXPECT_EQ(poolAllocator.getNumDeallocs(), 17 - i);
    }
    EXPECT_EQ(poolAllocator.getNumAllocs(), 17);
    EXPECT_EQ(poolAllocator.getNumDeallocs(), 17);
    EXPECT_EQ(poolAllocator.freeSlots(), 16);
    EXPECT_EQ(poolAllocator.allocatedSlots(), 0);
    EXPECT_THROW(pool.deallocate(allocated[0]), std::invalid_argument);
}

template <typename T>
void basicTypeTest()
{
    _basicTypeTest<T>(sizeof(T));
}

TEST(StaticAllocatorTest, BasicTypesTest)
{
    basicTypeTest<int>();
    basicTypeTest<double>();
    basicTypeTest<long double>();
    basicTypeTest<float>();
    basicTypeTest<long>();
    basicTypeTest<long long>();
#if 0
    _basicTypeTest<char>(sizeof(int));
#endif
}

TEST(StaticAllocatorTest, UsetTypesTest)
{
    struct MyStruct
    {
        int i;
        char a;
        char b;
    } __attribute__((packed));

    EXPECT_EQ(sizeof(MyStruct), 6);
    _basicTypeTest<MyStruct>(2 * sizeof(int));
}
