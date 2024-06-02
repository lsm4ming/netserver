#include <gtest/gtest.h>
#include "demo.cpp"

// 测试 Add 函数
TEST(AddTest, PositiveNumbers)
{
    EXPECT_EQ(3, Add(1, 2));
    EXPECT_EQ(5, Add(2, 3));
}

// 测试 Subtract 函数
TEST(SubtractTest, PositiveNumbers)
{
    EXPECT_EQ(1, Subtract(3, 2));
    EXPECT_EQ(-1, Subtract(2, 3));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
