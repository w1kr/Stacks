#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <numbers>

#include "build_convex_hull.hpp"


std::vector<point> make_nested_triangles(
    std::size_t layers, 
    double step = 10.0)
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


#include <random>
#include <cmath>

std::vector<point> make_nested_rings(
    std::size_t layers, 
    std::size_t min_points = 10, 
    std::size_t max_points = 20)
{
    if (min_points < 5 || max_points < min_points)
    {
        throw std::invalid_argument("Invalid points count");
    }

    std::vector<point> points;
    std::mt19937 rng(12345);

    const double outer_radius = 1000000.0;
    constexpr double PI = 3.14159265358979323846;

    std::uniform_int_distribution<std::size_t> count_dist(min_points, max_points); // random number of points
    std::uniform_real_distribution<double> phase_dist(0.0, 2.0 * PI);              // random ring rotation
    std::uniform_real_distribution<double> jitter_dist(-0.2, 0.2);                 // random displacement of each point

    for (std::size_t ring = 0; ring < layers; ++ring)
    {
        const std::size_t n = count_dist(rng);
        const double radius = outer_radius * std::pow(0.85, static_cast<double>(ring));
        const double angle_step = 2.0 * PI / n;
        const double phase = phase_dist(rng);

        for (std::size_t i = 0; i < n; ++i)
        {
            const double angle = phase + i * angle_step + jitter_dist(rng) * angle_step;
            const double x = radius * std::cos(angle);
            const double y = radius * std::sin(angle);
            points.push_back({x, y});
        }
    }

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
        std::cout << "all triangle's tests passed\n";
    }


    f = true;

    for (std::size_t layers = 1; layers <= 100; ++layers)
    {
        auto test = make_nested_rings(layers);
        std::size_t result = build_multi_convex_hull(test);

        if (result != layers)
        {
            std::cout << "test failed: " << result << " != " << layers << "\n";
            f = false;
        }
    }

    if (f)
    {    
        std::cout << "all ring's tests passed\n";
    }

    return 0;
}
