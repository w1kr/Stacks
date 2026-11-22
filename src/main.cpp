#include <string>
#include <iostream>
#include <cmath>
#include <cassert>
#include <unordered_map>

#include "postfix.hpp"

int main()
{
    const auto vars = std::unordered_map<std::string, double>{{"a", 2}, {"b", 3}, {"c", 4}};
    auto close = [](double a, double b) { return std::abs(a-b) < 1e-9; };

    assert(close(calculate_expression("-2^2"), -4));
    assert(close(calculate_expression("2^-2"), 0.25));
    assert(close(calculate_expression("--2"), 2));
    assert(close(calculate_expression("!0"), 1));
    assert(close(calculate_expression("5!"), 120));
    assert(close(calculate_expression("2+3*4"), 14));
    assert(close(calculate_expression("max(1, 5, min(7, 3), 4)"), 5));
    assert(close(calculate_expression("pow(2, 3) + 10"), 18));
    assert(close(calculate_expression("1 < 2 ? 10 : 20"), 10));
    assert(close(calculate_expression("0 ? 1 : 1 ? 2 : 3"), 2));
    assert(close(calculate_expression("1 ? 0 ? 4 : 5 : 6"), 5));
    assert(close(calculate_expression("(2 + 3) > 4 ? sin(0) : cos(0)"), 0));

    custom_func f("x * sin(x / 2)");
    assert(close(f(2), 2 * std::sin(1.0)));
    std::cout << "All tests passed\n";

    return 0;
}
