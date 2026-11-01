#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits>

#include "build_convex_hull.hpp"


void test(
    const std::string& name,
    const std::vector<point>& p,
    const std::vector<std::size_t>& expected)
{
    auto result = build_convex_hull(p);

    if (result != expected)
    {
        std::cout << "[FAIL] " << name << "\n";

        std::cout << "Expected: ";
        for (auto i : expected)
        {
            std::cout << i << " ";
        }

        std::cout << "\nActual: ";
        for (auto i : result)
        {
            std::cout << i << " ";
        }

        std::cout << "\n\n";
        
        return;
    }

    std::cout << "[OK] " << name << "\n";
}


int main(void)
{
    test("Empty", {}, {});

    test("One point",
        {{5, 5}},
        {0});

    test("Two points",
        {{0, 0}, {5, 5}},
        {0, 1});

    test("Triangle",
        {
            {0, 0},
            {4, 0},
            {2, 4},
            {2, 1}
        },
        {0, 2, 1});

    test("Square",
        {
            {0, 0},
            {4, 0},
            {4, 4},
            {0, 4}
        },
        {0, 3, 2, 1});

    test("Square with inner points",
        {
            {0, 0},
            {4, 0},
            {4, 4},
            {0, 4},
            {1, 1},
            {2, 2},
            {3, 3}
        },
        {0, 3, 2, 1});

    test("Points on edges",
        {
            {0, 0},
            {4, 0},
            {4, 4},
            {0, 4},
            {2, 0},
            {4, 2},
            {2, 4},
            {0, 2}
        },
        {0, 3, 2, 1});

    test("Horizontal line",
        {
            {2, 0},
            {0, 0},
            {1, 0},
            {3, 0}
        },
        {1, 3});

    test("Diagonal line",
        {
            {2, 2},
            {0, 0},
            {1, 1},
            {3, 3}
        },
        {1, 3});

    test("Duplicate points",
        {
            {0, 0},
            {0, 0},
            {4, 0},
            {4, 4},
            {0, 4},
            {2, 2}
        },
        {0, 4, 3, 2});

    test("Negative coordinates",
        {
            {-2, -1},
            {-1, -1},
            {-1,  0},
            {-2,  0},
            {-1.5, -0.5}
        },
        {0, 3, 2, 1});

    test("Two points: lowest first",
        {
            {0, 5},
            {10, 0}
        },
        {1, 0});

    test("Same polar angle",
        {
            {0, 0},
            {-1, 1},
            {-2, 2},
            {-3, 3},
            {-100, 1}
        },
        {0, 4, 3});

        return 0;
}
