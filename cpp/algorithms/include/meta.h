#pragma once
#include <stddef.h>
#include <stdexcept>
#include <bitset>
#include <cstring>

namespace utils
{
    template <size_t Size>
    size_t power()
    {
        return Size * power<Size - 1>();
    }
    template <>
    size_t power<0>()
    {
        return 1;
    }

    template <size_t Size>
    size_t fibo()
    {
        return fibo<Size - 1>() + fibo<Size - 2>();
    }
    template <>
    size_t fibo<0>()
    {
        return 0;
    }
    template <>
    size_t fibo<1>()
    {
        return 1;
    }
    template <>
    size_t fibo<2>()
    {
        return 1;
    }

    template <size_t Size>
    struct BitMaskWindow
    {
        static_assert(Size > 0, "Size must be grather than 0");
        static_assert(Size <= sizeof(unsigned int) * 8, "Size must be less than or equal to sizeof(unsigned int)*8");
        static constexpr const int &mask = (0x1 << (Size - 1)) | BitMaskWindow<Size - 1>::mask;
    };
    template <>
    struct BitMaskWindow<1>
    {
        static constexpr const int &mask = 0x01;
    };

} // namespace utils