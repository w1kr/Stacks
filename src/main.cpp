#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits>

#include "build_convex_hull.hpp"

const std::vector<point> data = 
{
    {2, 1},
    {4, 1},
    {5, 3},
    {3,2},
    {4,3},
    {3,5},
    {2,3},
    {1,3}
};


std::size_t build_multi_convex_hull(std::vector<point> p)
{
    std::size_t count = 0;
    std::vector<bool> used(p.size(), false);
    
    while (!p.empty())
    {
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


int main(void)
{
    std::size_t res = build_multi_convex_hull(data);

    std::cout << res << std::endl;

    // придумать как сгенерить тест для этой функции (через треугольники или что-то еще)

    return 0;
}
