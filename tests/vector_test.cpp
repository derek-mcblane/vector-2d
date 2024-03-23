#include "gtest/gtest.h"

#include <sstream>

#include "dm/vector.h"

using namespace dm::geom;
using namespace dm::elementwise::operators;

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

TEST(Vec2Test, X)
{
    EXPECT_EQ((Vec2i{1, 2}.x()), 1);
}

TEST(Vec2Test, Y)
{
    EXPECT_EQ((Vec2i{1, 2}.y()), 2);
}

TEST(Vec2Test, UnitX)
{
    EXPECT_EQ(Vec2i::unit_x(), (Vec2i{1, 0}));
}

TEST(Vec2Test, UnitY)
{
    EXPECT_EQ(Vec2i::unit_y(), (Vec2i{0, 1}));
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
    EXPECT_EQ((Vec2i{-3, 2} * 5), (Vec2i{-15, 10}));
}

TEST(Vec2Test, MultiplyAssign)
{
    Vec2i a{-3, 2};
    a *= 5;
    EXPECT_EQ(a, (Vec2i{-15, 10}));
}

TEST(Vec2Test, Divide)
{
    EXPECT_EQ((Vec2i{-15, 10} / 5), (Vec2i{-3, 2}));
}

TEST(Vec2Test, DivideAssign)
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

TEST(Vec3Test, GreaterThan)
{
    EXPECT_GT((Vec3i{2, 1, 1}), (Vec3i{1, 2, 2}));
}

TEST(Vec3Test, X)
{
    EXPECT_EQ((Vec3i{1, 2, 3}.x()), 1);
}

TEST(Vec3Test, Y)
{
    EXPECT_EQ((Vec3i{1, 2, 3}.y()), 2);
}

TEST(Vec3Test, Z)
{
    EXPECT_EQ((Vec3i{1, 2, 3}.z()), 3);
}

TEST(Vec3Test, UnitX)
{
    EXPECT_EQ(Vec3i::unit_x(), (Vec3i{1, 0, 0}));
}

TEST(Vec3Test, UnitY)
{
    EXPECT_EQ(Vec3i::unit_y(), (Vec3i{0, 1, 0}));
}

TEST(Vec3Test, UnitZ)
{
    EXPECT_EQ(Vec3i::unit_z(), (Vec3i{0, 0, 1}));
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

TEST(Vec3Test, Output)
{
    std::stringstream ss;
    const auto a = Vec3i{-3, 2, 1};
    ss << a;
    EXPECT_EQ(ss.str(), "<-3, 2, 1>");
}

TEST(Vec3Test, Distance)
{
    EXPECT_DOUBLE_EQ(distance(Vec3i{-3, -4, -5}, Vec3i{3, 4, 5}), std::sqrt(6*6 + 8*8 + 10*10));
}

TEST(Vec3Test, ChebyshevDistance)
{
    EXPECT_EQ(chebyshev_distance(Vec3i{11, -7, 1}, Vec3i{4, 10, 2}), 17);
}

TEST(Vec3Test, ManhattanDistance)
{
    EXPECT_EQ(manhattan_distance(Vec3i{-7, 11, 1}, Vec3i{10, 4, 2}), 25);
}

TEST(Vec3Test, MinX)
{
    std::vector<Vec3i> vecs {
        {1, 2, 3},
        {4, 5, 6}
    };
    EXPECT_EQ(min_x(std::begin(vecs), std::end(vecs)), 1);
}

TEST(Vec3Test, MinY)
{
    std::vector<Vec3i> vecs {
        {1, 2, 3},
        {4, 5, 6}
    };
    EXPECT_EQ(min_y(std::begin(vecs), std::end(vecs)), 2);
}

TEST(Vec3Test, MinZ)
{
    std::vector<Vec3i> vecs {
        {1, 2, 3},
        {4, 5, 6}
    };
    EXPECT_EQ(min_z(std::begin(vecs), std::end(vecs)), 3);
}

TEST(Vec3Test, MaxX)
{
    std::vector<Vec3i> vecs {
        {1, 2, 3},
        {4, 5, 6}
    };
    EXPECT_EQ(max_x(std::begin(vecs), std::end(vecs)), 4);
}

TEST(Vec3Test, MaxY)
{
    std::vector<Vec3i> vecs {
        {1, 2, 3},
        {4, 5, 6}
    };
    EXPECT_EQ(max_y(std::begin(vecs), std::end(vecs)), 5);
}

TEST(Vec3Test, MaxZ)
{
    std::vector<Vec3i> vecs {
        {1, 2, 3},
        {4, 5, 6}
    };
    EXPECT_EQ(max_z(std::begin(vecs), std::end(vecs)), 6);
}

TEST(Vec3Test, MinExtents)
{
    const std::vector<Vec3i> vecs {
        {1, 5, 3},
        {4, 2, 6}
    };
    EXPECT_EQ(min_extent(std::begin(vecs), std::end(vecs)).value(), (Vec3i{1, 2, 3}));
}

TEST(Vec3Test, MaxExtents)
{
    const std::vector<Vec3i> vecs {
        {1, 5, 3},
        {4, 2, 6}
    };
    EXPECT_EQ(max_extent(std::begin(vecs), std::end(vecs)).value(), (Vec3i{4, 5, 6}));
}

TEST(Vec3Test, Extents)
{
    std::vector<Vec3i> vecs {
        {1, 5, 3},
        {4, 2, 6}
    };
    EXPECT_EQ(extents(std::begin(vecs), std::end(vecs)).value(), (std::pair(Vec3i{1, 2, 3}, Vec3i{4, 5, 6})));
}
