#pragma once
#include <stddef.h>
#include <stdexcept>
#include <bitset>
#include <cstring>

namespace utils
{

    template <size_t ObjectSize, size_t NumObjects>
    struct StaticAllocator
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

        void *allocate()
        {
            if (freeSlots)
            {

                unsigned int slot = memoryUsage.find_first_unset();
                memoryUsage.set(slot);
                numAllocs++;
                off_t offset = slot + elementSize;
                std::memset(startMemory + offset, 0, elementSize);
                return startMemory + offset;
            }
            return nullptr;
        }

        void deallocate(void *ptr)
        {
            if (ptr < startMemory || ptr >= endMemory || ((size_t)ptr % elementSize != 0))
            {
                throw std::invalid_argument("received ptr outside allocator");
            }

            size_t slot = ((size_t)ptr - (size_t)memory) / elementSize;

            if (slot >= numObjects)
            {
                throw std::invalid_argument("received ptr outside allocator");
            }

            if (memoryUsage.test(slot))
            {
                throw std::invalid_argument("received ptr allready freed");
            }
            numDeallocs++;
            memoryUsage.reset(slot);
        }

        size_t allocatedSlots() const
        {
            return memoryUsage.count();
        }

        size_t freeSlots() const
        {
            return memoryUsage.size() - memoryUsage.count();
        }

        size_t getNumAllocs() const
        {
            return numAllocs;
        }

        size_t getNumDeallocs() const
        {
            return numDeallocs;
        }

    private:
        size_t numAllocs{0};
        size_t numDeallocs{0};

        int memory[(elementSize * numObjects) / sizeof(int)];
        void *startMemory = memory;
        void *endMemory = startMemory + memoryLength;
        std::bitset<numObjects> memoryUsage;
    };

    template <typename T, size_t NumObjects>
    struct StaticAllocatorPool
    {
        constexpr StaticAllocatorPool() = default;
        constexpr StaticAllocatorPool(const StaticAllocatorPool &) = delete;
        constexpr StaticAllocatorPool &operator=(const StaticAllocatorPool &) = delete;

        T *allocate()
        {
            return static_cast<T *>(pool.allocate());
        }

        void deallocate(T *ptr)
        {
            pool.deallocate(ptr);
        }
        const StaticAllocator<sizeof(T), NumObjects> &getAllocator() const
        {
            return pool;
        }

    private:
        StaticAllocator<sizeof(T), NumObjects> pool;
    };
} // namespace utils