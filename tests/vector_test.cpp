#include "gtest/gtest.h"

#include <sstream>

#include "vec2.h"
using namespace dm;

using Vec2i = Vec2<int>;

TEST(Vec2Test, Equal)
{
    EXPECT_EQ((Vec2i{1, 2}), (Vec2i{1, 2}));
}

TEST(Vec2Test, NotEqual)
{
    EXPECT_NE((Vec2i{1, 2}), (Vec2i{2, 1}));
}

TEST(Vec2Test, LessThan)
{
    EXPECT_LT((Vec2i{1, 2}), (Vec2i{2, 1}));
}

TEST(Vec2Test, x)
{
    EXPECT_EQ((Vec2i{1, 2}.x()), 1);
}

TEST(Vec2Test, y)
{
    EXPECT_EQ((Vec2i{1, 2}.y()), 2);
}

TEST(Vec2Test, Add)
{
    const auto result = Vec2i{-15, 10} + Vec2i{-15, 10};
    EXPECT_EQ(result, (Vec2i{-30, 20}));
}

TEST(Vec2Test, Subtract)
{
    const auto result = Vec2i{-15, 10} - Vec2i{-15, 10};
    EXPECT_EQ(result, (Vec2i{0, 0}));
}

TEST(Vec2Test, Multiply)
{
    Vec2i a{-3, 2};
    a *= 5;
    EXPECT_EQ(a, (Vec2i{-15, 10}));
}

TEST(Vec2Test, Divide)
{
    Vec2i a{-15, 10};
    a /= 5;
    EXPECT_EQ(a, (Vec2i{-3, 2}));
}

TEST(Vec2Test, Print)
{
    std::stringstream ss;
    const auto a = Vec2i{-3, 2};
    ss << a;
    EXPECT_EQ(ss.str(), "<-3, 2>");
}

using Vec3i = Vec3<int>;

TEST(Vec3Test, Equal)
{
    EXPECT_EQ((Vec3i{1, 2, 3}), (Vec3i{1, 2, 3}));
}

TEST(Vec3Test, NotEqual)
{
    EXPECT_NE((Vec3i{1, 2, 3}), (Vec3i{3, 2, 1}));
}

TEST(Vec3Test, LessThan)
{
    EXPECT_LT((Vec3i{1, 2, 1}), (Vec3i{1, 2, 2}));
}

TEST(Vec3Test, x)
{
    EXPECT_EQ((Vec3i{1, 2, 3}.x()), 1);
}

TEST(Vec3Test, y)
{
    EXPECT_EQ((Vec3i{1, 2, 3}.y()), 2);
}

TEST(Vec3Test, z)
{
    EXPECT_EQ((Vec3i{1, 2, 3}.z()), 3);
}

TEST(Vec3Test, Add)
{
    const auto result = Vec3i{-15, 10, 5} + Vec3i{-15, 10, 5};
    EXPECT_EQ(result, (Vec3i{-30, 20, 10}));
}

TEST(Vec3Test, Subtract)
{
    const auto result = Vec3i{-15, 10, 5} - Vec3i{-15, 10, 5};
    EXPECT_EQ(result, (Vec3i{0, 0, 0}));
}

TEST(Vec3Test, Multiply)
{
    Vec3i a{-3, 2, 1};
    a *= 5;
    EXPECT_EQ(a, (Vec3i{-15, 10, 5}));
}

TEST(Vec3Test, Divide)
{
    Vec3i a{-15, 10, 5};
    a /= 5;
    EXPECT_EQ(a, (Vec3i{-3, 2, 1}));
}

TEST(Vec3Test, Print)
{
    std::stringstream ss;
    const auto a = Vec3i{-3, 2, 1};
    ss << a;
    EXPECT_EQ(ss.str(), "<-3, 2, 1>");
}
