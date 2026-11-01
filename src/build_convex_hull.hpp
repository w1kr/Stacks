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

    // input check
    for (const auto& pt : p)
    {
        if (!std::isfinite(pt.x) || !std::isfinite(pt.y))
        {
            throw std::invalid_argument("Invalid coordinates");
        }
    }

    // delete repeating points
    std::vector<std::size_t> order(p.size());

    for (std::size_t i = 0; i < p.size(); ++i)
    {
        order[i] = i;
    }

    std::sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b)
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

    for (std::size_t i : order)
    {
        if (unique.empty() || 
            p[i].x != p[unique.back()].x || 
            p[i].y != p[unique.back()].y)
        {
            unique.push_back(i);
        }
    }

    order = std::move(unique);

    if (order.size() == 1)
    {
        return order;
    }

    // 1
    std::size_t min_pos = 0;

    for (std::size_t i = 1; i < order.size(); ++i)
    {
        if (p[order[i]].y < p[order[min_pos]].y || 
            (p[order[i]].y == p[order[min_pos]].y && 
            p[order[i]].x < p[order[min_pos]].x))
        {
            min_pos = i;
        }
    }

    std::swap(order[0], order[min_pos]);

    if (order.size() == 2)
    {
        return order;
    }

    // 2
    const point base = p[order[0]];

    std::sort(order.begin() + 1, order.end(), [&](std::size_t a, std::size_t b)
    {
        return (p[a] - base) < (p[b] - base);
    });

    // 3
    std::vector<std::size_t> S;

    S.push_back(order[0]);
    S.push_back(order[1]);

    for (std::size_t i = 2; i < order.size(); ++i)
    {
        std::size_t c = order[i];

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

    return S;
}
