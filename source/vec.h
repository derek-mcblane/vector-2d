#pragma once

#include <cmath>

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <utility>

namespace dm {

namespace math {

template <typename T>
[[nodiscard]] constexpr T abs_difference(T lhs, T rhs) noexcept
{
    return (lhs < rhs) ? rhs - lhs : lhs - rhs;
}

} // namespace math

namespace geom {

template <typename DimensionType, size_t N>
class Vector
{
  private:
    using T = DimensionType;
    using index_sequence = std::make_index_sequence<N>;

  public:
    using vector_type = Vector<T, N>;
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


    [[nodiscard]] static constexpr vector_type make_repeated(const T value) noexcept
    {
        vector_type vector;
        for (size_t i = 0; i < N; ++i) {
            vector[i] = value;
        }
        return vector;
    }

    [[nodiscard]] static constexpr T operator_dot_product(const vector_type& lhs, const vector_type& rhs)
    {
        return operator_dot_product_(lhs, rhs, index_sequence{});
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
    template <size_t... I>
    [[nodiscard]] static constexpr vector_type abs_differences_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {math::abs_difference(lhs[I], rhs[I])...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr vector_type operator_elementwise_multiply_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] * rhs[I])...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr T operator_dot_product_(const vector_type& lhs, const vector_type& rhs, std::index_sequence<I...>) noexcept
    {
        return ((lhs[I] * rhs[I]) + ...);
    }

    template <size_t... I>
    [[nodiscard]] constexpr T magnitude_squared_(std::index_sequence<I...>) const noexcept
    {
        return operator_dot_product(*this, *this);
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
};

template <typename T>
using Vec2 = Vector<T, 2>;

template <typename T>
using Vec3 = Vector<T, 3>;

template <typename Vector>
using dimension_type = typename Vector::dimension_type;

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
    static_assert(Dimension < VectorT::n_dimensions);
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
    static_assert(Dimension < VectorT::n_dimensions);
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
    return internal::min_extent(begin, end, std::make_index_sequence<VectorT::n_dimensions>{});
}

template <std::forward_iterator It>
[[nodiscard]] std::optional<std::iter_value_t<It>>
max_extent(It begin, It end)
{
    using VectorT = std::iter_value_t<It>; 
    return internal::max_extent(begin, end, std::make_index_sequence<VectorT::n_dimensions>{});
}

template <std::forward_iterator It>
[[nodiscard]] std::optional<std::pair<std::iter_value_t<It>, std::iter_value_t<It>>>
extents(It begin, It end)
{
    using VectorT = std::iter_value_t<It>; 
    return internal::extents(begin, end, std::make_index_sequence<VectorT::n_dimensions>{});
}

} // namespace geom
} // namespace dm
