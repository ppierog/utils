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

} // namespace utils