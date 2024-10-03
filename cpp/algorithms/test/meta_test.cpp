#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "meta.h"

using namespace utils;

TEST(MetaTest, InitializationTest)
{
    EXPECT_EQ(power<6>(), 720);
    EXPECT_EQ(power<5>(), 120);
    EXPECT_EQ(power<4>(), 24);
    EXPECT_EQ(power<3>(), 6);
    EXPECT_EQ(fibo<5>(), 5);
    EXPECT_EQ(fibo<6>(), 8);
    EXPECT_EQ(fibo<7>(), 13);
}
