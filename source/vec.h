#pragma once

#include <cmath>

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <type_traits>
#include <utility>

namespace dm {
namespace geom {

template <typename DimensionType, size_t N>
class Vec
{
  private:
    using T = DimensionType;
    using index_sequence = std::make_index_sequence<N>;

  public:
    using vector_type = Vec<T, N>;
    using dimension_type = DimensionType;

    static constexpr size_t n_dimensions = N;
    enum Dimension { X, Y, Z };

    [[nodiscard]] static constexpr vector_type unit_x() noexcept
    {
        static_assert(X < N);
        return {1};
    }

    [[nodiscard]] static constexpr vector_type unit_y() noexcept
    {
        static_assert(Y < N);
        return {0, 1};
    }

    [[nodiscard]] static constexpr vector_type unit_z() noexcept
    {
        static_assert(Z < N);
        return {0, 0, 1};
    }


    [[nodiscard]] vector_type make_repeated(const T value) noexcept
    {
        vector_type vector;
        for (size_t i = 0; i < N; ++i) {
            vector[i] = value;
        }
        return vector;
    }

    [[nodiscard]] static constexpr T distance_squared(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        const auto delta = lhs - rhs;
        return delta.magnitude_squared();
    }

    [[nodiscard]] static constexpr double distance(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        const auto delta = lhs - rhs;
        return delta.magnitude();
    }

    [[nodiscard]] static constexpr T chebyshev_distance(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        const auto differences = abs_differences_(lhs, rhs, index_sequence{});
        return *std::max_element(differences.elements_.begin(), differences.elements_.end());
    }

    [[nodiscard]] static constexpr T manhattan_distance(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        const auto differences = abs_differences_(lhs, rhs, index_sequence{});
        return std::accumulate(differences.elements_.begin(), differences.elements_.end(), 0);
    }

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

    [[nodiscard]] constexpr T magnitude_squared() const noexcept
    {
        return magnitude_squared_(index_sequence{});
    }

    [[nodiscard]] constexpr double magnitude() const noexcept
    {
        return std::sqrt(magnitude_squared());
    }

    constexpr vector_type& normalize() noexcept
    {
        return *this /= magnitude();
    }

    [[nodiscard]] friend constexpr bool operator==(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return operator_equal_(lhs, rhs, index_sequence{});
    }

    [[nodiscard]] friend constexpr bool operator!=(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    [[nodiscard]] friend constexpr bool operator<(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return operator_less_than_(lhs, rhs, std::make_index_sequence<N-1>{});
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
        return operator_negate_(value, index_sequence{});
    }

    [[nodiscard]] friend constexpr vector_type operator-(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return operator_minus_(lhs, rhs, index_sequence{});
    }

    [[nodiscard]] friend constexpr vector_type operator+(const vector_type& lhs, const vector_type& rhs) noexcept
    {
        return operator_plus_(lhs, rhs, index_sequence{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr vector_type operator*(const vector_type& value, U n) noexcept
    {
        return operator_multiply_(value, n, index_sequence{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr vector_type operator*(U n, const vector_type& value) noexcept
    {
        return operator_multiply_(value, n, index_sequence{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr vector_type operator/(const vector_type& value, U n) noexcept
    {
        return operator_divide_(value, n, index_sequence{});
    }

    constexpr vector_type& operator-=(const vector_type& other) noexcept
    {
        return *this = *this - other;
    }

    constexpr vector_type& operator+=(const vector_type& other) noexcept
    {
        return *this = *this + other;
    }

    constexpr vector_type& operator*=(T n) noexcept
    {
        return *this = *this * n;
    }

    constexpr vector_type& operator/=(T n) noexcept
    {
        return *this = *this / n;
    }

    friend std::ostream& operator<<(std::ostream& os, const vector_type& value)
    {
        operator_output_(os, value, index_sequence{});
        return os;
    }

  private:
    [[nodiscard]] static constexpr T abs_difference_(T lhs, T rhs) noexcept
    {
        return (lhs < rhs) ? rhs - lhs : lhs - rhs;
    }

    template <size_t... I>
    [[nodiscard]] static constexpr vector_type abs_differences_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {abs_difference_(lhs[I], rhs[I])...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr vector_type operator_elementwise_multiply_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] * rhs[I])...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr T operator_dot_product_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return ((lhs[I] * rhs[I]) + ...);
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr T operator_outer_product_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {

        return ((lhs[I] * rhs[I]) + ...);
    }

    template <size_t... I>
    [[nodiscard]] constexpr T magnitude_squared_(std::index_sequence<I...>) const noexcept
    {
        return operator_dot_product_(*this, *this);
    }

    template <size_t... I>
    [[nodiscard]] static constexpr bool operator_equal_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return ((lhs[I] == rhs[I]) && ...);
    }

    template <size_t I>
    [[nodiscard]] static constexpr bool operator_less_than_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I>) noexcept
    {
        return lhs[I] < rhs[I];
    }

    template <size_t... I>
    [[nodiscard]] static constexpr bool operator_less_than_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return (((lhs[I] != rhs[I] && lhs[I] < rhs[I]) || lhs[I+1] < rhs[I+1]) || ...);
    }

    template <size_t... I>
    [[nodiscard]] static constexpr vector_type operator_negate_(const vector_type& value, std::index_sequence<I...>) noexcept
    {
        return {-value[I]...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr vector_type operator_minus_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] - rhs[I])...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr vector_type operator_plus_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] + rhs[I])...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr vector_type operator_multiply_(const vector_type& value, const U n, std::index_sequence<I...>) noexcept
    {
        return {(value[I] * n)...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr vector_type operator_divide_(const vector_type& value, const U n, std::index_sequence<I...>) noexcept
    {
        return {(value[I] / n)...};
    }

    template <size_t... I>
    static constexpr std::ostream& operator_output_(std::ostream& os, const vector_type& value, std::index_sequence<I...>)
    {
        os << '<';
        ((os << (I == 0 ? "" : ", ") << value[I]), ...);
        os << '>';
        return os;
    }

    [[nodiscard]] friend constexpr T operator_dot_product(const vector_type& lhs, const vector_type& rhs)
    {
        return operator_dot_product_(lhs, rhs, index_sequence{});
    }

    [[nodiscard]] friend constexpr T operator_outer_product(const vector_type& lhs, const vector_type& rhs)
    {
        return vector_type::operator_outer_product_(lhs, rhs, index_sequence{});
    }
};

template <typename T>
using Vec2 = Vec<T, 2>;

template <typename T>
using Vec3 = Vec<T, 3>;

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
min(const VecContainer& vectors)
{
    static_assert(Dimension < VecContainer::value_type::n_dimensions);
    if (vectors.size() == 0) {
        return {};
    }
    const auto min_element = *std::min_element(vectors.cbegin(), vectors.cend(),
        [](auto lhs, auto rhs) { return lhs[Dimension] < rhs[Dimension]; });
    return min_element[Dimension];
}

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
max(const VecContainer& vectors)
{
    static_assert(Dimension < VecContainer::value_type::n_dimensions);
    if (vectors.size() == 0) {
        return {};
    }
    const auto max_element = *std::max_element(vectors.cbegin(), vectors.cend(),
        [](auto lhs, auto rhs) { return lhs[Dimension] < rhs[Dimension]; });
    return max_element[Dimension];
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
min_x(const VecContainer& vectors)
{
    return min<VecContainer::value_type::X>(vectors);
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
min_y(const VecContainer& vectors)
{
    return min<VecContainer::value_type::Y>(vectors);
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
min_z(const VecContainer& vectors)
{
    return min<VecContainer::value_type::Z>(vectors);
}


template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
max_x(const VecContainer& vectors)
{
    return max<VecContainer::value_type::X>(vectors);
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
max_y(const VecContainer& vectors)
{
    return max<VecContainer::value_type::Y>(vectors);
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type>
max_z(const VecContainer& vectors)
{
    return max<VecContainer::value_type::Z>(vectors);
}

namespace internal {

template <typename VecContainer, size_t... I>
[[nodiscard]] std::optional<typename VecContainer::value_type>
min_extent(const VecContainer& vectors, std::index_sequence<I...>)
{
    if (vectors.size() == 0) {
        return {};
    }
    using Vec = typename VecContainer::value_type; 
    const auto dimension_min = std::numeric_limits<typename Vec::dimension_type>::min();

    auto min = Vec::make_repeated(dimension_min);
    for (const auto& vector : vectors) {
        ((min[I] = std::min(min[I], vector[I])), ...);
    }
    return min;
}

template <typename VecContainer, size_t... I>
[[nodiscard]] std::optional<typename VecContainer::value_type>
max_extent(const VecContainer& vectors, std::index_sequence<I...>)
{
    if (vectors.size() == 0) {
        return {};
    }
    using Vec = typename VecContainer::value_type; 
    const auto dimension_min = std::numeric_limits<typename Vec::dimension_type>::min();

    auto max = Vec::make_repeated(dimension_min);
    for (const auto& vector : vectors) {
        ((max[I] = std::max(max[I], vector[I])), ...);
    }
    return max;
}

}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type>
min_extent(const VecContainer& vectors)
{
    if (vectors.size() == 0) {
        return {};
    }
    using index_sequence = std::make_index_sequence<VecContainer::value_type::n_dimensions>;
    return internal::min_extent(vectors, index_sequence{});
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type>
max_extent(const VecContainer& vectors)
{
    if (vectors.size() == 0) {
        return {};
    }
    using index_sequence = std::make_index_sequence<VecContainer::value_type::n_dimensions>;
    return internal::max_extent(vectors, index_sequence{});
}

template <typename VecContainer>
[[nodiscard]] std::optional<std::pair<typename VecContainer::value_type, typename VecContainer::value_type>>
extents(const VecContainer& vectors)
{
    if (vectors.size() == 0) {
        return {};
    }
    return std::pair{min_extent(vectors).value(), max_extent(vectors).value()};
}

} // namespace geom
} // namespace dm
