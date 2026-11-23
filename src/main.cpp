#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cmath>


struct rectangle
{
    long double width;
    long double height;
};


long double calc_flooded_cobbles(const std::vector<rectangle>& src)
{
    std::vector<long double> dist;
    long double sum = 0.0;

    dist.push_back(0.0);

    for (std::size_t idx = 1; idx < src.size(); ++idx)
    {
        long double right = sum += src[idx].width;
        dist.push_back(right);
        sum += src[idx].width;
    }

    std::stack<std::size_t> stack;
    long double result = 0;

    for (std::size_t idx = 0; idx < src.size(); ++idx)
    {
        while (!stack.empty() && src[stack.top()].height < src[idx].height)
        {
            long double htop = src[stack.top()].height;
            stack.pop();

            if (!stack.empty())
            {
                result += (std::min(htop, src[idx].height) - htop) * (dist[idx] - dist[stack.top()]);
            }
        }

        

        stack.push(idx);
    }

    return result;
}


// доделать этот + задача о контрабанде


int main(void)
{
    std::vector<rectangle> data = {
        {2, 4},
        {2, 3},
        {2, 4}
    };

    long double res = calc_flooded_cobbles(data);
    std::cout << res << '\n';

    return 0;
}
