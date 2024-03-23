#include "gtest/gtest.h"

#include "dm/elementwise.h"

#include <array>

using namespace dm::elementwise;
using namespace dm::elementwise::operators;

TEST(Elementwise, Add)
{
    std::array<int, 3> elements{0, 1, 2};
    const auto expr = make_sum_expression(elements, elements);
    EXPECT_EQ(expr[0], 0);
    EXPECT_EQ(expr[1], 2);
    EXPECT_EQ(expr[2], 4);
}

TEST(Elementwise, AddTwoTypes)
{
    std::array<int, 3> a{0, 1, 2};
    int b[3] = {0, 1, 2};
    const auto expr = make_sum_expression(a, b);
    EXPECT_EQ(expr[0], 0);
    EXPECT_EQ(expr[1], 2);
    EXPECT_EQ(expr[2], 4);
}

TEST(Elementwise, AddScalar)
{
    std::array<int, 3> a{0, 1, 2};
    const auto expr = make_sum_expression(a, 5);
    EXPECT_EQ(expr[0], 5);
    EXPECT_EQ(expr[1], 6);
    EXPECT_EQ(expr[2], 7);
}

TEST(Elementwise, Subtract)
{
    std::array<int, 3> elements{0, 1, 2};
    const auto expr = make_difference_expression(elements, elements);
    EXPECT_EQ(expr[0], 0);
    EXPECT_EQ(expr[1], 0);
    EXPECT_EQ(expr[2], 0);
}

