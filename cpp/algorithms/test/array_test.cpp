#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "array.h"

using namespace utils;

TEST(ArrayTest, InitializationTest)
{
    utils::Array<int, 16> array1;
    utils::Array<int, 16> array2;
    auto result = utils::MergeArray(array1, array2);

    EXPECT_EQ(array1.arraySize, 16);

    EXPECT_EQ(result.arraySize, 32);
}
