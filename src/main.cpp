#include "stack_combined.hpp"
#include <iostream>

template <typename Iter>
Iter check_correct_brackets(Iter first, Iter last)
{
    StackCombined<std::pair<Iter, int>> stack = StackCombined<size_t>(GrowthPolicy::Exponential);

    constexpr std::string open_brackets  = "([{<";
    constexpr std::string close_brackets = ")]}>";

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


std::string max_correct_subbrackets(const std::string& string) 
{
    StackCombined<int> stack(GrowthPolicy::Exponential);
    size_t max_len = 0;
    size_t max_start = 0;

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


int main()
{
// ------------ task 1 ------------
    std::string data = "([][[{}])";

    auto iter = check_correct_brackets(begin(data), end(data));

    if (iter == end(data)) { std::cout << " correct brackets" << std::endl; }
    else { std::cout <<  " incorrect bracket: " << *iter << std::endl; }

    std::cout << std::endl;
    
// ------------ task 3 ------------
    std::string data2 = "())(()()";
    std::cout << data2 << std::endl;
    std::string res = max_correct_subbrackets(data2);
    std::cout << res << std::endl;

    return 0;
}
