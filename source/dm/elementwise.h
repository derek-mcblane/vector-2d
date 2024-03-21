#include "dm/math.h"

#include <cstddef>

#include <tuple>
#include <type_traits>

namespace dm::elementwise {

template <typename T>
concept Indexable = requires (T t)
{
    {t[0]};
};

template <Indexable T>
[[nodiscard]] constexpr auto element(const T& object, const std::size_t i)
{
    return object[i];
}

template <typename T>
[[nodiscard]] constexpr T element(const T& object, const std::size_t i)
{
    return object;
}

template <typename T>
struct is_expression : public std::false_type {};

template <typename T>
inline constexpr bool is_expression_v = is_expression<T>::value;

template <typename Operator, typename... Operands>
class OperandExpression
{
public:
    OperandExpression(Operator&& oper, const Operands& ...operands)
        : operator_(std::move(oper)), operands_(operands...)
    {
    }

    [[nodiscard]] constexpr auto operator[](const std::size_t i) const
    {
        return std::apply(
            [this, i](const Operands&... operands)
            {
                return operator_(element(operands, i)...);
            },
            operands_
        );
    }

private:
    Operator operator_;
    std::tuple<Operands const&...> operands_;
};

template <typename Derived, typename... Operands>
struct is_expression<OperandExpression<Derived, Operands...>> : public std::true_type {};

template <typename T>
constexpr auto make_negate_expression(T&& value)
{
    const auto negate = [](const auto el)
        { return -el; };
    return OperandExpression{std::move(negate), std::forward<T>(value)};
}

template <typename T, typename U>
constexpr auto make_sum_expression(T&& lhs, U&& rhs)
{
    const auto sum = [](const auto lhs_el, const auto rhs_el)
        { return lhs_el + rhs_el; };
    return OperandExpression{std::move(sum), std::forward<T>(lhs), std::forward<U>(rhs)};
}

template <typename T, typename U>
constexpr auto make_difference_expression(T&& lhs, U&& rhs)
{
    const auto difference = [](const auto lhs_el, const auto rhs_el)
        { return lhs_el - rhs_el; };
    return OperandExpression{std::move(difference), std::forward<T>(lhs), std::forward<U>(rhs)};
}

template <typename T, typename U>
constexpr auto make_absolute_difference_expression(T&& lhs, U&& rhs)
{
    const auto difference = [](const auto lhs_el, const auto rhs_el)
        { return math::absolute_difference(lhs_el, rhs_el); };
    return OperandExpression{std::move(difference), std::forward<T>(lhs), std::forward<U>(rhs)};
}

template <typename T, typename U>
constexpr auto make_product_expression(T&& lhs, U&& rhs)
{
    const auto product = [](const auto lhs_el, const auto rhs_el)
        { return lhs_el * rhs_el; };
    return OperandExpression{std::move(product), std::forward<T>(lhs), std::forward<U>(rhs)};
}

template <typename T, typename U>
constexpr auto make_quotient_expression(T&& lhs, U&& rhs)
{
    const auto quotient = [](const auto lhs_el, const auto rhs_el)
        { return lhs_el / rhs_el; };
    return OperandExpression{std::move(quotient), std::forward<T>(lhs), std::forward<U>(rhs)};
}

} // namespace dm::elementwise

template <typename Derived, typename Operand, typename... Operands>
struct std::tuple_size<dm::elementwise::OperandExpression<Derived, Operand, Operands...>>
    : public std::integral_constant<size_t, std::tuple_size_v<Operand>>
{};
