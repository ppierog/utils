#pragma once
#include <stddef.h>
#include <stdexcept>
#include <bitset>
#include <cstring>

template <size_t Size>
struct BitSet
{
    static constexpr size_t objectSize = ObjectSize;
    static constexpr size_t elementSize = objectSize / sizeof(int) + objectSize % sizeof(int) ? 1 : 0;
    static constexpr size_t numObjects = NumObjects;
    static constexpr size_t memoryLength = elementSize * numObjects;

    static_assert(objectSize >= sizeof(int));
    static_assert(numObjects > 0);

    constexpr StaticAllocator() = default;
    constexpr StaticAllocator(const StaticAllocator &) = delete;
    constexpr StaticAllocator &operator=(const StaticAllocator &) = delete;
};