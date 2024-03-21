#pragma once

#include "dm/vector_internal.h"
#include "dm/elementwise.h"
#include "dm/math.h"

#include <cassert>
#include <cmath>

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <utility>


namespace dm {

namespace geom {

template <typename T>
inline constexpr size_t size_v = std::tuple_size_v<T>;

template <typename T>
[[nodiscard]] constexpr auto max_element(const T& elements) noexcept
{
    return internal::max_element(elements, std::make_index_sequence<size_v<T>>{});
}

template <typename T>
[[nodiscard]] constexpr auto element_sum(const T& elements) noexcept
{
    return internal::element_sum(elements, std::make_index_sequence<size_v<T>>{});
}

template <typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto dot_product(const Lhs& lhs, const Rhs& rhs) noexcept
{
    static_assert(size_v<Lhs> == size_v<Rhs>);
    return element_sum(dm::elementwise::make_product_expression(lhs, rhs));
}

template <typename T>
[[nodiscard]] constexpr auto magnitude_squared(const T& elements) noexcept
{
    return dot_product(elements, elements);
}

template <typename T>
[[nodiscard]] constexpr auto magnitude(const T& elements) noexcept
{
    return std::sqrt(magnitude_squared(elements));
}

template <typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto distance_squared(const Lhs& lhs, const Rhs& rhs) noexcept
{
    static_assert(size_v<Lhs> == size_v<Rhs>);
    return magnitude_squared(dm::elementwise::make_difference_expression(lhs, rhs));
}

template <typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto distance(const Lhs& lhs, const Rhs& rhs) noexcept
{
    static_assert(size_v<Lhs> == size_v<Rhs>);
    return magnitude(dm::elementwise::make_difference_expression(lhs, rhs));
}

template <typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto chebyshev_distance(const Lhs& lhs, const Rhs& rhs) noexcept
{
    static_assert(size_v<Lhs> == size_v<Rhs>);
    return max_element(dm::elementwise::make_absolute_difference_expression(lhs, rhs));
}

template <typename Lhs, typename Rhs>
[[nodiscard]] constexpr auto manhattan_distance(const Lhs& lhs, const Rhs& rhs) noexcept
{
    static_assert(size_v<Lhs> == size_v<Rhs>);
    return element_sum(dm::elementwise::make_absolute_difference_expression(lhs, rhs));
}

template <typename T>
constexpr T& normalize(T& elements) noexcept
{
    return elements /= magnitude(elements);
}

template <typename T>
struct is_vector : public std::false_type {};

template <typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template <typename Vector>
requires is_vector_v<Vector>
using dimension_type = typename Vector::dimension_type;

template <typename DimensionType, size_t N>
requires std::is_arithmetic_v<DimensionType>
class Vector
{
  private:
    using T = DimensionType;

  public:
    using vector_type = Vector<T, N>;
    using dimension_type = DimensionType;

    static constexpr size_t n_dimensions = N;
    enum Dimension { X, Y, Z };

    template <size_t Dimension>
    [[nodiscard]] static constexpr vector_type make_unit() noexcept
    {
        static_assert(Dimension < N);
        return make_unit_<Dimension>(std::make_index_sequence<N>{});
    }

    [[nodiscard]] static constexpr vector_type make_unit(const size_t dimension) noexcept
    {
        assert(dimension < N);
        return make_unit_(dimension, std::make_index_sequence<N>{});
    }

    [[nodiscard]] static constexpr vector_type unit_x() noexcept
    {
        return make_unit<X>();
    }

    [[nodiscard]] static constexpr vector_type unit_y() noexcept
    {
        return make_unit<Y>();
    }

    [[nodiscard]] static constexpr vector_type unit_z() noexcept
    {
        return make_unit<Z>();
    }

    [[nodiscard]] static constexpr vector_type make_repeated(const T value) noexcept
    {
        return make_repeated_(value, std::make_index_sequence<N>{});
    }

#ifdef VECTOR_USE_ELEMENTWISE_EXPRESSION_TEMPLATES
    template <typename Expression>
    requires elementwise::is_expression_v<Expression>
    [[nodiscard]] static constexpr vector_type from_elementwise_expression(const Expression& other) noexcept
    {
        return from_elementwise_expression_(value, std::make_index_sequence<N>{});
    }

    template <typename Expression>
    requires elementwise::is_expression_v<Expression>
    [[nodiscard]] vector_type& operator=(const Expression& other)
    {
        return operator_assign_(other, std::make_index_sequence<N>{});
    }

    template <typename Expression, size_t... I>
    requires elementwise::is_expression_v<Expression>
    [[nodiscard]] static constexpr vector_type from_elementwise_expression_(const Expression& other, std::index_sequence<I...>) noexcept
    {
        return {other[I]...};
    }

    template <typename Indexable, size_t... I>
    [[nodiscard]] vector_type& operator_assign_(const Indexable& other, std::index_sequence<I...>)
    {
        ((elements_[I] = other[I]), ...);
        return *this;
    }
#endif

    // Aggregate initialization
    std::array<T, N> elements_;

    [[nodiscard]] constexpr T operator[](const size_t i) const
    {
        return elements_[i];
    }

    [[nodiscard]] constexpr T& operator[](const size_t i)
    {
        return elements_[i];
    }

    [[nodiscard]] constexpr T x() const noexcept
    {
        static_assert(X < N);
        return elements_[X];
    }

    [[nodiscard]] constexpr T& x() noexcept
    {
        static_assert(X < N);
        return elements_[X];
    }

    [[nodiscard]] constexpr T y() const noexcept
    {
        static_assert(Y < N);
        return elements_[Y];
    }

    [[nodiscard]] constexpr T& y() noexcept
    {
        static_assert(Y < N);
        return elements_[Y];
    }

    [[nodiscard]] constexpr T z() const noexcept
    {
        static_assert(Z < N);
        return elements_[Z];
    }

    [[nodiscard]] constexpr T& z() noexcept
    {
        static_assert(Z < N);
        return elements_[Z];
    }

    [[nodiscard]] constexpr std::size_t size() noexcept
    {
        return N;
    }

    constexpr vector_type& normalize() noexcept
    {
        return *this /= magnitude(*this);
    }

    [[nodiscard]] friend constexpr bool operator==(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return lhs.elements_ == rhs.elements_; 
    }

    [[nodiscard]] friend constexpr bool operator!=(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    [[nodiscard]] friend constexpr bool operator<(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return lhs.elements_ < rhs.elements_;
    }

    [[nodiscard]] friend constexpr bool operator>(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return rhs < lhs;
    }

    [[nodiscard]] friend constexpr bool operator<=(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    [[nodiscard]] friend constexpr bool operator>=(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    [[nodiscard]] friend constexpr vector_type operator-(const vector_type& value) noexcept
    {
        return elementwise_negate_(value, std::make_index_sequence<N>{});
    }

    [[nodiscard]] friend constexpr vector_type operator-(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return elementwise_subtract_(lhs, rhs, std::make_index_sequence<N>{});
    }

    [[nodiscard]] friend constexpr vector_type operator+(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return elementwise_add_(lhs, rhs, std::make_index_sequence<N>{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr vector_type operator*(const vector_type& value, U n) noexcept
    {
        return elementwise_multiply_(value, n, std::make_index_sequence<N>{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr vector_type operator*(U n, const vector_type& value) noexcept
    {
        return value * n;
    }

    template <typename U>
    [[nodiscard]] friend constexpr vector_type operator/(const vector_type& value, U n) noexcept
    {
        return elementwise_divide_(value, n, std::make_index_sequence<N>{});
    }

    template <typename U>
    constexpr vector_type& operator-=(const U& other) noexcept
    {
        return *this = *this - other;
    }

    template <typename U>
    constexpr vector_type& operator+=(const U& other) noexcept
    {
        return *this = *this + other;
    }

    template <typename U>
    constexpr vector_type& operator*=(const U& other) noexcept
    {
        return *this = *this * other;
    }

    template <typename U>
    constexpr vector_type& operator/=(const U& other) noexcept
    {
        return *this = *this / other;
    }

    friend std::ostream& operator<<(std::ostream& os, const vector_type& value)
    {
        operator_output_(os, value, std::make_index_sequence<N>{});
        return os;
    }

  private:
    template <size_t... DummyI>
    [[nodiscard]] static constexpr vector_type make_repeated_(const T value, std::index_sequence<DummyI...>) noexcept
    {
        return vector_type{(DummyI, value)...};
    }

    template <size_t Dimension, size_t... I>
    [[nodiscard]] static constexpr vector_type make_unit_(std::index_sequence<I...>) noexcept
    {
        return vector_type{(I == Dimension ? 1 : 0)...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr vector_type make_unit_(const size_t dimension, std::index_sequence<I...>) noexcept
    {
        return vector_type{(I == dimension ? 1 : 0)...};
    }

#ifndef VECTOR_USE_ELEMENTWISE_EXPRESSION_TEMPLATES
    template <size_t... I>
    [[nodiscard]] static constexpr vector_type elementwise_negate_(const vector_type& value, std::index_sequence<I...>) noexcept
    {
        return {-value[I]...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr vector_type elementwise_subtract_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] - rhs[I])...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr vector_type elementwise_add_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] + rhs[I])...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr vector_type elementwise_multiply_(const vector_type& value, const U n, std::index_sequence<I...>) noexcept
    {
        return {(value[I] * n)...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr vector_type elementwise_divide_(const vector_type& value, const U n, std::index_sequence<I...>) noexcept
    {
        return {(value[I] / n)...};
    }
#endif

    template <size_t... I>
    static constexpr std::ostream& operator_output_(std::ostream& os, const vector_type& value, std::index_sequence<I...>)
    {
        os << '<';
        ((os << (I == 0 ? "" : ", ") << value[I]), ...);
        os << '>';
        return os;
    }
};

template <typename DimensionType, size_t N>
struct is_vector<Vector<DimensionType, N>> : public std::true_type {};

template <typename T>
using Vec2 = Vector<T, 2>;

template <typename T>
using Vec3 = Vector<T, 3>;

namespace internal {

template <std::forward_iterator It, size_t... I>
[[nodiscard]] std::optional<std::iter_value_t<It>>
min_extent(It begin, It end, std::index_sequence<I...>)
{
    using VectorT = std::iter_value_t<It>; 
    using VectorDimT = dimension_type<VectorT>;
    if (begin == end) {
        return {};
    }
    auto min = VectorT::make_repeated(std::numeric_limits<VectorDimT>::max());
    for (It vec_it = begin; vec_it != end; ++vec_it) {
        ((min[I] = std::min(min[I], (*vec_it)[I])), ...);
    }
    return min;
}

template <std::forward_iterator It, size_t... I>
[[nodiscard]] std::optional<std::iter_value_t<It>>
max_extent(It begin, It end, std::index_sequence<I...>)
{
    using VectorT = std::iter_value_t<It>; 
    using VectorDimT = dimension_type<VectorT>;
    if (begin == end) {
        return {};
    }
    auto max = VectorT::make_repeated(std::numeric_limits<VectorDimT>::min());
    for (It vec_it = begin; vec_it != end; ++vec_it) {
        ((max[I] = std::max(max[I], (*vec_it)[I])), ...);
    }
    return max;
}

template <std::forward_iterator It, size_t... I>
[[nodiscard]] std::optional<std::pair<std::iter_value_t<It>, std::iter_value_t<It>>>
extents(It begin, It end, std::index_sequence<I...>)
{
    using VectorT = std::iter_value_t<It>; 
    using VectorDimT = dimension_type<VectorT>;
    if (begin == end) {
        return {};
    }
    auto min = VectorT::make_repeated(std::numeric_limits<VectorDimT>::max());
    auto max = VectorT::make_repeated(std::numeric_limits<VectorDimT>::min());
    for (It vec_it = begin; vec_it != end; ++vec_it) {
        ((min[I] = std::min(min[I], (*vec_it)[I]),
          max[I] = std::max(max[I], (*vec_it)[I])), ...);
    }
    return std::pair{min, max};
}

}

template <size_t Dimension, typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
min(It begin, It end)
{
    using VectorT = std::iter_value_t<It>; 
    static_assert(Dimension < size_v<VectorT>);
    const auto min_element = std::min_element(begin, end,
        [](auto lhs, auto rhs) { return lhs[Dimension] < rhs[Dimension]; });
    return (min_element == end) ? std::nullopt :
        std::optional{(*min_element)[Dimension]};
}

template <size_t Dimension, typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
max(It begin, It end)
{
    using VectorT = std::iter_value_t<It>; 
    static_assert(Dimension < size_v<VectorT>);
    const auto max_element = std::max_element(begin, end,
        [](auto lhs, auto rhs) { return lhs[Dimension] < rhs[Dimension]; });
    return (max_element == end) ? std::nullopt :
        std::optional{(*max_element)[Dimension]};
}

template <typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
min_x(It begin, It end)
{
    return min<std::iter_value_t<It>::X>(begin, end);
}

template <typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
min_y(It begin, It end)
{
    return min<std::iter_value_t<It>::Y>(begin, end);
}

template <typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
min_z(It begin, It end)
{
    return min<std::iter_value_t<It>::Z>(begin, end);
}

template <typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
max_x(It begin, It end)
{
    return max<std::iter_value_t<It>::X>(begin, end);
}

template <typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
max_y(It begin, It end)
{
    return max<std::iter_value_t<It>::Y>(begin, end);
}

template <typename It>
[[nodiscard]] std::optional<typename std::iter_value_t<It>::dimension_type>
max_z(It begin, It end)
{
    return max<std::iter_value_t<It>::Z>(begin, end);
}

template <std::forward_iterator It>
[[nodiscard]] std::optional<std::iter_value_t<It>>
min_extent(It begin, It end)
{
    using VectorT = std::iter_value_t<It>; 
    return internal::min_extent(begin, end, std::make_index_sequence<size_v<VectorT>>{});
}

template <std::forward_iterator It>
[[nodiscard]] std::optional<std::iter_value_t<It>>
max_extent(It begin, It end)
{
    using VectorT = std::iter_value_t<It>; 
    return internal::max_extent(begin, end, std::make_index_sequence<size_v<VectorT>>{});
}

template <std::forward_iterator It>
[[nodiscard]] std::optional<std::pair<std::iter_value_t<It>, std::iter_value_t<It>>>
extents(It begin, It end)
{
    using VectorT = std::iter_value_t<It>; 
    return internal::extents(begin, end, std::make_index_sequence<size_v<VectorT>>{});
}

} // namespace geom

template <typename DimensionType, size_t N>
struct elementwise::is_expression<geom::Vector<DimensionType, N>> : public std::true_type {};

} // namespace dm

template <typename DimensionType, size_t N>
struct std::tuple_size<dm::geom::Vector<DimensionType, N>> : public std::integral_constant<size_t, N> {};
