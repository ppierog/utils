#pragma once
#include <stddef.h>
#include <stdexcept>
#include <bitset>
#include <cstring>
#include <cassert>
namespace utils
{

    template <size_t ObjectSize, size_t NumObjects>
    struct StaticAllocator
    {
        static constexpr size_t const &objectSize = ObjectSize;
        static constexpr size_t const &elementSize = (ObjectSize / sizeof(int)) * sizeof(int) + sizeof(int) * (bool)(ObjectSize % sizeof(int));
        static constexpr size_t const &numObjects = NumObjects;
        static constexpr size_t const &memoryLength = elementSize * numObjects;

        static_assert(numObjects > 0);
        static_assert(objectSize >= sizeof(int));

        StaticAllocator() = default;
        StaticAllocator(const StaticAllocator &) = delete;
        StaticAllocator &operator=(const StaticAllocator &) = delete;

        void *allocate()
        {
            if (0 == freeSlots())
                return nullptr;

            auto findFirstUnset = [](std::bitset<numObjects> &bits)
            {
                for (size_t i = 0; i < bits.size(); ++i)
                {
                    if (!bits.test(i))
                        return i;
                }
                assert(nullptr == "Thera are free slots but not found in btset");
            };

            unsigned int slot = findFirstUnset(memoryUsage);
            memoryUsage.set(slot);
            numAllocs++;
            off_t offset = slot * elementSize;
            char *memory = (char *)startMemory + offset;
            std::memset(memory, 0, elementSize);
            return memory;
        }

        void deallocate(void *ptr)
        {
            if (ptr < startMemory || ptr >= endMemory || ((size_t)ptr % elementSize != 0))
            {
                throw std::invalid_argument("received ptr outside allocator");
            }

            size_t slot = ((size_t)ptr - (size_t)memory) / elementSize;

            if (!memoryUsage.test(slot))
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

        const void *const getStartMemory() const
        {
            return (const void *const)startMemory;
        }

        const void *const getEndMemory() const
        {
            return (const void *const)endMemory;
        }

    private:
        size_t numAllocs{0};
        size_t numDeallocs{0};

        int memory[(elementSize * numObjects) / sizeof(int)];
        void *startMemory = memory;
        void *endMemory = (char *)startMemory + memoryLength;
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