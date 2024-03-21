#include <cstddef>
#include <utility>

namespace dm {
namespace geom {
namespace internal {

template <typename T>
[[nodiscard]] constexpr auto max(const T& lhs, const T& rhs) noexcept
{
    return (lhs < rhs) ? rhs : lhs;
}

template <typename T, typename... Ts>
[[nodiscard]] constexpr auto max(const T& element, const Ts&... elements) noexcept
{
    return max(element, max(elements...));
}

template <typename U, size_t... I>
[[nodiscard]] constexpr auto max_element(const U& object, std::index_sequence<I...>) noexcept
{
    return max(object[I]...);
}

template <typename U, size_t... I>
[[nodiscard]] constexpr auto element_sum(const U& object, std::index_sequence<I...>) noexcept
{
    return (object[I] + ...);
}

} // namespace internal
} // namespace geom
} // namespace dm
