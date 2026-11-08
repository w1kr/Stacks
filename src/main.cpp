#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits>
#include <random>

#include "build_convex_hull.hpp"


std::vector<point> make_nested_triangles(std::size_t layers, double step = 10.0)
{
    std::vector<point> points;
    points.reserve(layers * 3);

    const double center = 2.0 * (static_cast<double>(layers) + 1.0) * step;

    for (std::size_t i = 0; i < layers; ++i)
    {
        const double r = static_cast<double>(layers - i) * step;
        points.push_back({center, center + 2.0 * r});
        points.push_back({center - r, center - r});
        points.push_back({center + r, center - r});
    }

    std::mt19937 rng(12345);
    std::shuffle(points.begin(), points.end(), rng);

    return points;
}


int main(void)
{
    bool f = true;

    for (std::size_t layers = 1; layers <= 100; ++layers)
    {
        auto test = make_nested_triangles(layers);
        std::size_t result = build_multi_convex_hull(test);

        if (result != layers)
        {
            std::cout << "test failed: " << result << " != " << layers << "\n";
            f = false;
            break;
        }
    }

    if (f)
    {    
        std::cout << "all tests passed\n";
    }

    return 0;
}
