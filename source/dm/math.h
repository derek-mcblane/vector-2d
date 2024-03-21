#pragma once

namespace dm::math {

template <typename T>
[[nodiscard]] inline constexpr T absolute_difference(T lhs, T rhs) noexcept
{
    return (lhs < rhs) ? rhs - lhs : lhs - rhs;
}

} // namespace dm::math
