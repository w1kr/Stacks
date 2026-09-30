#include "stack_combined.hpp"


std::string max_correct_subbrackets(const std::string& string) 
{
    StackCombined<int> stack(GrowthPolicy::Exponential);
    std::size_t max_len = 0;
    std::size_t max_start = 0;

    stack.push(-1);

    for (size_t i = 0; i < string.size(); ++i) {
        if (string[i] == '(') {
            stack.push(i);
        }
        else {
            stack.pop();

            if (stack.size() == 0) {
                stack.push(i);
            }
            else {
                size_t len = i - stack.top();

                if (len > max_len) {
                    max_len = len;
                    max_start = stack.top() + 1;
                }
            }
        }
    }

    return string.substr(max_start, max_len);
}


template<typename Iter>
std::string max_correct_subbrackets(Iter first, Iter last)
{
    StackCombined<std::pair<Iter, int>> stack(GrowthPolicy::Exponential);
    std::string open_brackets  = "([{<";
    std::string close_brackets = ")]}>";
    Iter max_start = first;
    std::size_t max_len = 0;
    
    for ( ; first != last; ++first)
    {
        if (std::size_t idx = open_brackets.find(*first); 
            idx != std::string_view::npos)
        {
            stack.push( {first, idx} );
            continue;
        }

        if (std::size_t idx = close_brackets.find(*first);
            idx != std::string_view::npos && 
            !stack.size() && 
            stack.top().second == idx)
        {
            stack.pop();
            const Iter start = stack.size() == 0 ? first : std::next(stack.top().first);
            const std::size_t len = static_cast<std::size_t>(std::distance(start, std::next(first)))
            
            if (len > max_len)
            {
                max_len = len;
                max_start = start;
            }
        }

        else
        {
            stack.push( {first, std::string_view::npos} )
        }
    }

    return std::string(max_start, std::next(max_start, max_len));
}
