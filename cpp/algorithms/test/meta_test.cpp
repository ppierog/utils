#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "meta.h"

using namespace utils;

TEST(MetaTest, PowerTest)
{
    EXPECT_EQ(power<0>(), 1);
    EXPECT_EQ(power<1>(), 1);
    EXPECT_EQ(power<2>(), 2);
    EXPECT_EQ(power<3>(), 6);
    EXPECT_EQ(power<4>(), 24);
    EXPECT_EQ(power<5>(), 120);
    EXPECT_EQ(power<6>(), 720);
    EXPECT_EQ(power<7>(), 5040);
}

TEST(MetaTest, FiboTest)
{
    EXPECT_EQ(fibo<5>(), 5);
    EXPECT_EQ(fibo<6>(), 8);
    EXPECT_EQ(fibo<7>(), 13);
}

TEST(MetaTest, BitMaskWindowTest)
{
    EXPECT_EQ(BitMaskWindow<1>::mask, 0x1);
    EXPECT_EQ(BitMaskWindow<2>::mask, 0x3);
    EXPECT_EQ(BitMaskWindow<3>::mask, 0x7);
    EXPECT_EQ(BitMaskWindow<4>::mask, 0xF);
    EXPECT_EQ(BitMaskWindow<5>::mask, 0x1F);
    EXPECT_EQ(BitMaskWindow<6>::mask, 0x3F);
    EXPECT_EQ(BitMaskWindow<7>::mask, 0x7F);
    EXPECT_EQ(BitMaskWindow<8>::mask, 0xFF);
}
