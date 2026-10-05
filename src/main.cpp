#include <iostream>
#include "stack_combined.hpp"

#include <algorithm>


template <typename Iter>
Iter find_majorizing_element(Iter first, Iter last)
{
    StackCombined<Iter> stack(GrowthPolicy::Exponential);

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
Iter find_majorizing_equal_element(Iter first, Iter last)
{
    // если нечетное кол-во, то просто считаем по алгоритму
    // если четное кол-во, то запоминаем первый элемент (просто например) и считаем по алгоритму оставшиеся нечетные
    //      если ничего не вернуло, то считаем число того первого элемента

    if (std::distance(first, last) % 2)
    {
        return find_majorizing_element(first, last);
    }

    if (first == last)
    {
        return last;
    }

    auto it = find_majorizing_element(first, last - 1);

    if (it != last - 1)
    {
        return it;
    }

    return std::count(first, last, *(last - 1)) * 2 >= std::distance(first, last) ? *(last - 1) : last;
}


int main(void)
{   
    std::string data = "111223";

    if (auto res = find_majorizing_element(begin(data), end(data));
        res != end(data)) 
    {
        std::cout << *res << std::endl;
    }
    else 
    {
        std::cout << "no major element" << std::endl;
    }

    auto res = find_majorizing_equal_element(begin(data), end(data));




    return 0;
}
