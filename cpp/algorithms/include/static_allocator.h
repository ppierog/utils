#pragma once

#include <stddef.h>
#include <stdexcept>
#include <bitset>
#include <cstring>
#include <cassert>

#ifdef ENABLE_UTILS_RUNTIME_CHECKS
#ifndef REDZONE_SIZE
#define REDZONE_SIZE 8
#endif
static_assert(REDZONE_SIZE == sizeof(int) * 2);
static_assert(REDZONE_SIZE % (sizeof(int) * 2) == 0);
#else
#define REDZONE_SIZE 0
#endif

namespace utils
{
    template <size_t ObjectSize, size_t NumObjects>
    struct StaticMemoryProvideder
    {
        static constexpr size_t const &objectSize = ObjectSize;
        static constexpr size_t const &elementSize = (ObjectSize / sizeof(int)) * sizeof(int) + sizeof(int) * (bool)(ObjectSize % sizeof(int));
        static constexpr size_t const &numObjects = NumObjects;
        static constexpr size_t const &memoryLength = elementSize * numObjects;

        static_assert(numObjects > 0);
        static_assert(objectSize >= sizeof(int));
        static_assert(elementSize >= sizeof(int));
        static_assert(0 == (elementSize % sizeof(int)));

        constexpr StaticMemoryProvideder() noexcept = default;
        constexpr StaticMemoryProvideder(const StaticMemoryProvideder &) = delete;
        constexpr StaticMemoryProvideder &operator=(const StaticMemoryProvideder &) = delete;

        char *getMemory() const noexcept
        {
            return (char *)memory;
        }

    private:
        int memory[(elementSize / sizeof(int)) * numObjects];
    };

    template <size_t ObjectSize, size_t NumObjects, typename MemoryProvider>
    struct BitsetAllocator
    {
        static constexpr size_t const &objectSize = ObjectSize;
        static constexpr size_t const &redZoneSize = REDZONE_SIZE;
        static constexpr size_t const &numObjects = NumObjects;
        static constexpr size_t const &elementSize = MemoryProvider::elementSize;
        static constexpr size_t const &memoryLength = MemoryProvider::memoryLength;

        constexpr BitsetAllocator() = default;
        constexpr BitsetAllocator(const BitsetAllocator &) = delete;
        constexpr BitsetAllocator &operator=(const BitsetAllocator &) = delete;

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
#ifdef ENABLE_UTILS_RUNTIME_CHECKS
            std::memset(memory, 0xdeadbeef, sizeof(int));
            std::memset(memory + elementSize - sizeof(int), 0xdeadbeef, sizeof(int));
#endif
            return memory + redZoneSize / 2;
        }

        int deallocate(void *ptr)
        {
            char *allocatedPtr = (char *)ptr - redZoneSize / 2;
            if (allocatedPtr < startMemory || allocatedPtr >= endMemory ||
                ((size_t)allocatedPtr % elementSize != 0))
            {
#ifdef ENABLE_UTILS_EXCEPTIONS
                throw std::invalid_argument("received ptr outside allocator");
#endif
                return -1;
            }

            size_t slot = ((size_t)allocatedPtr - (size_t)startMemory) / elementSize;

            if (!memoryUsage.test(slot))
            {
#ifdef ENABLE_UTILS_EXCEPTIONS
                throw std::invalid_argument("received ptr allready freed");
#endif
                return -1;
            }

#ifdef ENABLE_UTILS_RUNTIME_CHECKS
#warning "Fix This for redefined RED_ZONE"

            if (*(int *)allocatedPtr != 0xdeadbeef ||
                *(int *)(allocatedPtr + elemeentSize - sizeof(int)) != 0xdeadbeef)
            {
                assert(nullptr == "Memory Corruption detected in Static Allocator");
            }
#endif

            numDeallocs++;
            memoryUsage.reset(slot);

            return 0;
        }

        size_t allocatedSlots() const noexcept
        {
            return memoryUsage.count();
        }

        size_t freeSlots() const noexcept
        {
            return memoryUsage.size() - memoryUsage.count();
        }

        size_t getNumAllocs() const noexcept
        {
            return numAllocs;
        }

        size_t getNumDeallocs() const noexcept
        {
            return numDeallocs;
        }

        constexpr const void *const getStartMemory() const noexcept
        {
            return (const void *const)startMemory;
        }

        constexpr const void *const getEndMemory() const noexcept
        {
            return (const void *const)endMemory;
        }

    private:
        size_t numAllocs{0};
        size_t numDeallocs{0};

        MemoryProvider memoryProvider;

        void *startMemory = memoryProvider.getMemory();
        void *endMemory = (char *)startMemory + memoryProvider.memoryLength;
        std::bitset<numObjects> memoryUsage;
    };

    template <size_t ObjectSize, size_t NumObjects>
    using StaticAllocator = BitsetAllocator<ObjectSize, NumObjects,
                                            StaticMemoryProvideder<ObjectSize + REDZONE_SIZE, NumObjects>>;

    template <typename T, size_t NumObjects, typename Allocator>
    struct AllocatorPool
    {
        constexpr AllocatorPool() = default;
        constexpr AllocatorPool(const AllocatorPool &) = delete;
        constexpr AllocatorPool &operator=(const AllocatorPool &) = delete;

        T *allocate()
        {
            return static_cast<T *>(pool.allocate());
        }

        template <typename... Types>
        T *create(Types... args)
        {
            T *ret = static_cast<T *>(pool.allocate());
            new (ret) T(&args...);
            return ret;
        }

        T *create(const T &obj)
        {
            T *ret = static_cast<T *>(pool.allocate());
            new (ret) T(obj);
            return ret;
        }

        T *create(const T &&obj)
        {
            T *ret = static_cast<T *>(pool.allocate());
            new (ret) T(std::move(obj));
            return ret;
        }

        void deallocate(T *ptr)
        {
            pool.deallocate(ptr);
        }
        const Allocator &getAllocator() const
        {
            return pool;
        }

    private:
        Allocator pool;
    };

    template <typename T, size_t NumObjects>
    using StaticAllocatorPool = AllocatorPool<T, NumObjects, StaticAllocator<sizeof(T), NumObjects>>;

} // namespace utils