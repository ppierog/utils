#pragma once
#include <stdexcept>

namespace utils
{
    template <size_t Size>
    struct BitSet
    {
        static constexpr size_t setSize = Size;
        static constexpr size_t wordSize = Size / (8 * sizeof(int)) + bool(Size % (sizeof(int) * 8));

        BitSet() = default;
        BitSet(const BitSet &) = default;
        BitSet &operator=(const BitSet &) = default;
        BitSet &operator|=(const BitSet &other) {

        };
        BitSet &operator&=(const BitSet &other) {

        };
        BitSet &operator^=(const BitSet &other) {

        };

        BitSet &operator>>=(size_t pos) {

        };

        BitSet &operator<<=(size_t pos) {

        };

        size_t count() const
        {
            return 0;
        }

        size_t firstSet() const
        {
            return -1;
        }

        size_t firstUnset() const
        {
            return -1;
        }

        bool test(size_t pos) const
        {
            return false;
        }

        int set(size_t pos)
        {
            return -1;
        }

        int clear(size_t pos)
        {
            return -1;
        }

        int flip(size_t pos)
        {
            return -1;
        }

        bool all() const
        {
            return false;
        }
        void reset(bool value = false)
        {
        }

        int mask[wordSize];
    };
} // namespace utils