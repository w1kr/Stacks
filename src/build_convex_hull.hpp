#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>


struct point
{
    double x, y;

    point operator+(const point& p) const
    {
        return {this->x + p.x, this->y + p.y};
    }

    point operator-(const point &p) const
    {
        return {this->x - p.x, this->y - p.y};
    }

    bool operator<(const point& p) const
    {
        long double cross = static_cast<long double>(x) * p.y - static_cast<long double>(y) * p.x;

        if (cross != 0)
        {
            return cross > 0;
        }

        long double d1 = static_cast<long double>(x) * x + static_cast<long double>(y) * y;
        long double d2 = static_cast<long double>(p.x) * p.x + static_cast<long double>(p.y) * p.y;

        return d1 < d2;
    }
};


std::vector<std::size_t> build_convex_hull(const std::vector<point> &p)
{
    if (p.empty())
    {
        return {};
    }
    else if (p.size() == 1)
    {
        return {0};
    }
    else if (p.size() == 2)
    {
        return {0, 1};
    }

    // delete repeating points
    std::vector<std::size_t> vec(p.size());

    for (std::size_t i = 0; i < p.size(); ++i)
    {
        vec[i] = i;
    }

    std::sort(vec.begin(), vec.end(), [&](std::size_t a, std::size_t b)
    {
        if (p[a].x != p[b].x)
        {
            return p[a].x < p[b].x;
        }

        if (p[a].y != p[b].y)
        {
            return p[a].y < p[b].y;
        }

        return a < b;
    });

    std::vector<std::size_t> unique;

    for (std::size_t i : vec)
    {
        if (unique.empty() || 
            p[i].x != p[unique.back()].x || 
            p[i].y != p[unique.back()].y)
        {
            unique.push_back(i);
        }
    }

    vec = std::move(unique);

    // 1
    std::size_t min_pos = 0;

    for (std::size_t i = 1; i < vec.size(); ++i)
    {
        if (p[vec[i]].y < p[vec[min_pos]].y || 
            (p[vec[i]].y == p[vec[min_pos]].y && 
            p[vec[i]].x < p[vec[min_pos]].x))
        {
            min_pos = i;
        }
    }

    // 2
    const point base = p[vec[0]];

    std::sort(vec.begin() + 1, vec.end(), [&](std::size_t a, std::size_t b)
    {
        return (p[a] - base) < (p[b] - base);
    });

    // 3
    std::vector<std::size_t> S;

    S.push_back(vec[0]);
    S.push_back(vec[1]);

    for (std::size_t i = 2; i < vec.size(); ++i)
    {
        std::size_t c = vec[i];

        while (S.size() >= 2)
        {
            std::size_t a = S[S.size() - 2];
            std::size_t b = S[S.size() - 1];

            point ab = p[b] - p[a];
            point bc = p[c] - p[b];

            long double cross = ab.x * bc.y - ab.y * bc.x;

            if (cross > 0)
            {
                break;
            }
            S.pop_back();
        }
        S.push_back(c);
    }

    // 4
    std::reverse(S.begin() + 1, S.end());
    std::reverse(begin(S), end(S));

    return S;
}


std::size_t build_multi_convex_hull(std::vector<point> p)
{
    std::size_t count = 0;
    
    while (!p.empty())
    {
        std::vector<bool> used(p.size(), false);

        auto res = build_convex_hull(p);
        for (std::size_t idx : res)
        {
            used[idx] = true;
        }

        std::size_t passed = 0;
        for (std::size_t i = 0; i < p.size(); ++i)
        {
            if (used[i])
            {
                passed++;
            }
            else
            {
                p[i - passed] = p[i];
            }
        }

        p.resize(p.size() - passed);
        count += 1;
    }

    return count;
}


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
