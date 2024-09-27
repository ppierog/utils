#pragma once
#include <stddef.h>
#include <stdexcept>
#include <bitset>
#include <cstring>

namespace utils
{
    template <typename T, size_t Size>
    struct Array
    {
        static constexpr size_t const &arraySize = Size;

        Array() = default;
        Array(const Array &) = default;
        Array &operator=(const Array &) = default;

        T &operator[](size_t pos)
        {
            return array[pos];
        }
        T array[Size];
    };
#if 0
    template <typename T, size_t Size1, size_t Size2>
    static Array<T, Size1 + Size2> MergeArray(const Array<T, Size1> &first, const Array<T, Size2> &second)
    {
        Array<T, Size1 + Size2> result;
        return result;
    }
#endif
    template <typename T, size_t... Sizes>
    static Array<T, Size1 + Size2> MergeArray(const Array<T, Size1> &first, const Array<T, Size2> &second)
    {
        Array<T, Size1 + Size2> result;
        return result;
    }

} // namespace utils