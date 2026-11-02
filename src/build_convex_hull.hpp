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
