#pragma once

#include <cmath>

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <utility>

namespace dm {

template <typename DimensionType, size_t N>
class Vec
{
  private:
    using T = DimensionType;
  public:
    using dimension_type = DimensionType;
    static constexpr size_t n_elements = N;

  public:
    enum Dimension {
        X,
        Y,
        Z,
        W
    };

    [[nodiscard]] static constexpr Vec<T, N> unit_x() noexcept
    {
        Vec<T, N> v;
        v.x() = 1;
        return v;
    }

    [[nodiscard]] static constexpr Vec<T, N> unit_y() noexcept
    {
        Vec v;
        v.y() = 1;
        return v;
    }

    [[nodiscard]] static constexpr Vec<T, N> unit_z() noexcept
    {
        Vec v;
        v.z() = 1;
        return v;
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
        return elements_[X];
    }

    [[nodiscard]] constexpr T& x() noexcept
    {
        return elements_[X];
    }

    [[nodiscard]] constexpr T y() const noexcept
    {
        return elements_[Y];
    }

    [[nodiscard]] constexpr T& y() noexcept
    {
        return elements_[Y];
    }

    [[nodiscard]] constexpr T z() const noexcept
    {
        return elements_[Z];
    }

    [[nodiscard]] constexpr T& z() noexcept
    {
        return elements_[Z];
    }

  private:
    template <size_t... I>
    [[nodiscard]] static constexpr Vec<T, N> operator_negate(const Vec<T, N>& value, std::index_sequence<I...>) noexcept
    {
        return {-value[I]...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr Vec<T, N> operator_minus(const Vec<T, N>& lhs, const Vec<T, N>& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] - rhs[I])...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr Vec<T, N> operator_plus(const Vec<T, N>& lhs, const Vec<T, N>& rhs, std::index_sequence<I...>) noexcept
    {
        return {(lhs[I] + rhs[I])...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr Vec<T, N> operator_multiply(const Vec<T, N>& value, const U n, std::index_sequence<I...>) noexcept
    {
        return {(value[I] * n)...};
    }

    template <typename U, size_t... I>
    [[nodiscard]] static constexpr Vec<T, N> operator_divide(const Vec<T, N>& value, const U n, std::index_sequence<I...>) noexcept
    {
        return {(value[I] / n)...};
    }

    template <size_t... I>
    [[nodiscard]] static constexpr T magnitude_squared_(const Vec<T, N>& value, std::index_sequence<I...>) const noexcept
    {
        return (value[I] * value[I]) + ...;
    }

    template <size_t... I>
    [[nodiscard]] static constexpr std::ostream& operator_output(std::ostream& output, const Vec<T, N>& value, std::index_sequence<I...>)
    {
        return output << ... << value[i];
    }

  public:
    [[nodiscard]] friend constexpr Vec<T, N> operator-(const Vec<T, N>& value) noexcept
    {
        return Vec<T, N>::operator_negate(value, std::make_index_sequence<N>{});
    }

    [[nodiscard]] friend constexpr Vec<T, N> operator-(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        using Indices = std::make_index_sequence<N>;
        return Vec<T, N>::operator_minus(lhs, rhs, std::make_index_sequence<N>{});
    }

    [[nodiscard]] friend constexpr Vec<T, N> operator+(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return Vec<T, N>::operator_plus(lhs, rhs, std::make_index_sequence<N>{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr Vec<T, N> operator*(const Vec<T, N>& value, U n) noexcept
    {
        return Vec<T, N>::operator_multiply(value, n, std::make_index_sequence<N>{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr Vec<T, N> operator*(U n, const Vec<T, N>& value) noexcept
    {
        return Vec<T, N>::operator_multiply<U, Indices>(value, n, std::make_index_sequence<N>{});
    }

    template <typename U>
    [[nodiscard]] friend constexpr Vec<T, N> operator/(const Vec<T, N>& value, U n) noexcept
    {
        return Vec<T, N>::operator_divide<U, Indices>(value, n, std::make_index_sequence<N>{});
    }

    constexpr Vec<T, N>& operator-=(const Vec<T, N>& other) noexcept
    {
        return *this = *this - other;
    }

    constexpr Vec<T, N>& operator+=(const Vec<T, N>& other) noexcept
    {
        return *this = *this + other;
    }

    constexpr Vec<T, N>& operator*=(T n) noexcept
    {
        return *this = *this * n;
    }

    constexpr Vec<T, N>& operator/=(T n) noexcept
    {
        return *this = *this / n;
    }

    [[nodiscard]] constexpr T magnitude_squared() const noexcept
    {
        return magnitude_squared_(*this, ;
    }

    [[nodiscard]] constexpr double magnitude() const noexcept
    {
        return std::sqrt(magnitude_squared());
    }

    constexpr Vec<T, N>& normalize() noexcept
    {
        return *this /= magnitude();
    }

    [[nodiscard]] friend constexpr bool operator==(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    [[nodiscard]] friend constexpr bool operator!=(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    [[nodiscard]] friend constexpr bool operator<(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return (lhs.x() != rhs.x()) ? lhs.x() < rhs.x() : lhs.y() < rhs.y();
    }

    [[nodiscard]] friend constexpr bool operator>(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return rhs < lhs;
    }

    [[nodiscard]] friend constexpr bool operator<=(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    [[nodiscard]] friend constexpr bool operator>=(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    [[nodiscard]] static constexpr T distance_squared(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        Vec<T, N> delta = abs_difference(lhs, rhs);
        return delta.x() * delta.x() + delta.y() * delta.y();
    }

    [[nodiscard]] static constexpr T distance(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return std::sqrt(distance_squared(lhs, rhs));
    }

    [[nodiscard]] static constexpr T chebyshev_distance(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        Vec<T, N> delta = abs_difference(lhs, rhs);
        return std::max(delta.x(), delta.y());
    }

    [[nodiscard]] static constexpr T manhattan_distance(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        Vec<T, N> delta = abs_difference(lhs, rhs);
        return delta.x() + delta.y();
    }

    friend std::ostream& operator<<(std::ostream& output, const Vec<T, N>& value)
    {
        return output << "{" << operator_output(value, std::make_index_sequence<N>{}) << "}";
    }

  private:
    [[nodiscard]] static constexpr T abs_difference(T lhs, T rhs) noexcept
    {
        return std::max(lhs, rhs) - std::min(lhs, rhs);
    }

    [[nodiscard]] static constexpr T abs_difference_x(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return abs_difference(lhs.x(), rhs.x());
    }

    [[nodiscard]] static constexpr T abs_difference_y(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return abs_difference(lhs.y(), rhs.y());
    }

    [[nodiscard]] static constexpr Vec<T, N> abs_difference(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept
    {
        return {abs_difference(lhs.x(), rhs.x()), abs_difference(lhs.y(), rhs.y())};
    }
};

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> min(const VecContainer& vectors)
{
    static_assert(Dimension < VecContainer::n_dimensions);
    if (vectors.size() == 0) {
        return {};
    }
    const auto min_element = std::min_element(vectors.cbegin(), vectors.cend(),
        [](auto lhs, auto rhs) { return lhs.elements()[Dimension] < rhs.elements()[Dimension]; });
    return min_element->elements()[Dimension];
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> min(const VecContainer& vectors, const size_t dimension)
{
    if (vectors.size() == 0) {
        return {};
    }
    const auto min_element = std::min_element(vectors.cbegin(), vectors.cend(),
        [dimension](auto lhs, auto rhs) { return lhs.elements()[dimension] < rhs.elements()[dimension]; });
    return min_element->elements()[dimension];
}

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> max(const VecContainer& vectors)
{
    static_assert(Dimension < VecContainer::n_dimensions);
    if (vectors.size() == 0) {
        return {};
    }
    const auto max_element = std::max_element(vectors.cbegin(), vectors.cend(),
        [](auto lhs, auto rhs) { return lhs.elements()[Dimension] < rhs.elements()[Dimension]; });
    return max_element->elements()[Dimension];
}

template <typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> max(const VecContainer& vectors, const size_t dimension)
{
    if (vectors.size() == 0) {
        return {};
    }
    const auto max_element = std::max_element(vectors.cbegin(), vectors.cend(),
        [dimension](auto lhs, auto rhs) { return lhs.elements()[dimension] < rhs.elements()[dimension]; });
    return max_element->elements()[dimension];
}

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> min_x(const VecContainer& vectors)
{
    return min<VecContainer::X>(vectors);
}

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> min_y(const VecContainer& vectors)
{
    return min<VecContainer::Y>(vectors);
}

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> min_z(const VecContainer& vectors)
{
    return min<VecContainer::Z>(vectors);
}


template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> max_x(const VecContainer& vectors)
{
    return max<VecContainer::X>(vectors);
}

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> max_y(const VecContainer& vectors)
{
    return max<VecContainer::Y>(vectors);
}

template <size_t Dimension, typename VecContainer>
[[nodiscard]] std::optional<typename VecContainer::value_type::dimension_type> max_z(const VecContainer& vectors)
{
    return max<VecContainer::Z>(vectors);
}

namespace internal {

template <typename VecContainer, size_t... Indices>
[[nodiscard]] VecContainer min_extent(const VecContainer& vectors)
{
    return {min<Indices>(vectors)...};
}

template <typename VecContainer, size_t... Indices>
[[nodiscard]] VecContainer max_extent(const VecContainer& vectors)
{
    return {max<Indices>(vectors)...};
}

}

template <typename VecContainer>
[[nodiscard]] VecContainer min_extent(const VecContainer& vectors)
{
    if (vectors.size() == 0) {
        return {};
    }
    using Indices = std::make_index_sequence<VecContainer::n_elements>;
    return internal::min_extent<VecContainer, Indices>(vectors);
}

template <typename VecContainer>
[[nodiscard]] VecContainer max_extent(const VecContainer& vectors)
{
    if (vectors.size() == 0) {
        return {};
    }
    using Indices = std::make_index_sequence<VecContainer::n_elements>;
    return internal::max_extent<VecContainer, Indices>(vectors);
}

template <typename VecContainer>
[[nodiscard]] std::optional<std::pair<VecContainer, VecContainer>>
extents(const VecContainer& vectors)
{
    if (vectors.size() == 0) {
        return {};
    }
    return {min_extent(vectors), max_extent(vectors)};
}

} // namespace dm

#if (__cpp_lib_format >= 201907L)
#include <format>
template <typename T>
struct std::formatter<dm::Vec<T, N>> : std::formatter<std::string>
{
    auto format(dm::Vec<T, N> value, format_context& ctx) -> decltype(ctx.out())
    {
        return std::format_to(ctx.out(), "<{}, {}>]", value.x(), value.y());
    }
};
#endif
