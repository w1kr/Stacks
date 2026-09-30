#include "stack_combined.hpp"

template <typename Iter>
Iter check_correct_brackets(Iter first, Iter last)
{
    StackCombined<std::pair<Iter, int>> stack(GrowthPolicy::Exponential);
    std::string open_brackets  = "([{<";
    std::string close_brackets = ")]}>";

    for (; first != last; ++first) {

        if (int idx = open_brackets.find(*first); idx >= 0) {
            stack.push({ first, idx });
        }
        else {
            if (!stack.size() || close_brackets[stack.top().second] != *first) { return first; }
            stack.pop();
        }
    }

    return stack.size() ? stack.top().first : last;
}
