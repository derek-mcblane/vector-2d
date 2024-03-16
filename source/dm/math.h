#pragma once

namespace dm {

namespace math {

template <typename T>
[[nodiscard]] inline constexpr T abs_difference(T lhs, T rhs) noexcept
{
    return (lhs < rhs) ? rhs - lhs : lhs - rhs;
}

} // namespace math

} // namespace dm
