#pragma once

#include "stack_combined.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>


template <typename Iter>
Iter find_major_element(Iter first, Iter last)
{
    StackCombined<Iter> stack;

    auto _first = first;

    for (; first != last; ++first)
    {
        if (!stack.size())
        {
            stack.push(first);
        }
        else
        {
            if (*first == *stack.top())
            {
                stack.push(first);
            }
            else 
            {
                stack.pop();
            }
        }
    }

    if (!stack.size())
    {
        return last;
    }

    return std::count(_first, last, *stack.top()) * 2 >= std::distance(_first, last) ? stack.top() : last;
}


template <typename Iter>
Iter find_major_equal_element(Iter first, Iter last)
{
    if (std::distance(first, last) % 2)
    {
        return find_major_element(first, last);
    }

    if (first == last)
    {
        return last;
    }

    auto it = find_major_element(first, last - 1);

    if (it != last - 1)
    {
        return it;
    }

    return std::count(first, last, *(last - 1)) * 2 >= std::distance(first, last) ? *(last - 1) : last;
}


template <typename Iter>
Iter find_frequent_element_a(Iter first, Iter last, std::size_t k)
{
    if (k < 1 || first == last)
    {
        return last;
    }

    std::vector<StackCombined<Iter>> stacks;
    stacks.reserve(k - 1);

    for (auto it = first; it != last; ++it)
    {
        bool found = false;

        // if the element is already a candidate => increment counter
        for (auto& stack : stacks)
        {
            if (stack.size() && *stack.top() == *it)
            {
                stack.push(it);
                found = true;
                break;
            }
        }

        if (found)
        {
            continue;
        }

        // if there is room, introduce a new candidate
        if (stacks.size() < k - 1)
        {
            // GrowthPolicy::Exponential
            stacks.emplace_back();
            stacks.back().push(it);
            continue;
        }

        // decrement all candidate counters
        for (auto& stack : stacks)
        {
            stack.pop();
        }

        // remove candidates whose counters reached zero
        stacks.erase(std::remove_if
            (
                stacks.begin(), 
                stacks.end(), 
                [](auto& stack)
                    {
                        return stack.size() == 0;
                    }), 
                stacks.end()
            );
    }

    // verify candidates against their actual frequencies
    const auto n = std::distance(first, last);

    for (const auto& stack : stacks)
    {
        Iter candidate = stack.top();

        if (std::count(first, last, *candidate) > n / k)
        {
            return candidate;
        }
    }

    return last;
}


template <typename Iter>
Iter find_frequent_element_b(Iter first, Iter last)
{
    constexpr std::size_t k = 4;

    if (first == last)
        return last;

    std::vector<StackCombined<Iter>> stacks;
    stacks.reserve(k - 1);

    for (auto it = first; it != last; ++it)
    {
        bool found = false;

        for (auto& stack : stacks)
        {
            if (stack.size() && *stack.top() == *it)
            {
                stack.push(it);
                found = true;
                break;
            }
        }

        if (found)
            continue;

        if (stacks.size() < k - 1)
        {
            stacks.emplace_back(GrowthPolicy::Exponential);
            stacks.back().push(it);
            continue;
        }

        for (auto& stack : stacks)
            stack.pop();

        stacks.erase(std::remove_if
            (
                stacks.begin(), 
                stacks.end(), 
                [](auto& stack)
                    {
                        return stack.size() == 0;
                    }), 
                stacks.end()
            );
    }

    const auto n = std::distance(first, last);

    for (const auto& stack : stacks)
    {
        Iter candidate = stack.top();
        const auto count = std::count(first, last, *candidate);

        if (static_cast<double>(count) > 0.31 * n)
        {
            return candidate;
        }
    }

    return last;
}
