#pragma once

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

    private:
        T array[Size];
    };

} // namespace utils