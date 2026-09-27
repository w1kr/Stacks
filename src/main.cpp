#include "stack_combined.hpp"
#include <iostream>

template <typename Iter>
Iter check_correct_brackets(Iter first, Iter last)
{
    StackCombined<std::pair<Iter, int>> stack(GrowthPolicy::Exponential);

    std::string open_brackets  = "([{<";
    std::string close_brackets = ")]}>";

    for (; first != last; ++first) {

        if (long long idx = open_brackets.find(*first); idx >= 0) {
            stack.push({ first, idx });
        }
        else {
            if (!stack.size() || close_brackets[stack.top().second] != *first) { return first; }
            stack.pop();
        }
    }

    return stack.size() ? stack.top().first : last;
}


/* std::string max_correct_subbrackets(const std::string& string) 
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
} */


template <typename Iter>
std::string max_correct_subbrackets(Iter first, Iter last)
{
    StackCombined<std::pair<Iter, int>> stack(GrowthPolicy::Exponential);

    std::string open_brackets  = "([{<";
    std::string close_brackets = ")]}>";

    Iter max_start = last;
    Iter max_end = last;

    stack.push({ first, -1 });

    for (Iter current = first; current != last; ++current) {

        auto open_idx = open_brackets.find(*current);

        if (open_idx != std::string::npos) {
            stack.push({ current, static_cast<int>(open_idx) });
        }
        else {
            auto close_idx = close_brackets.find(*current);

            if (stack.size() == 1) {
                stack.pop();
                stack.push({ current, -1 });
            }
            else if (stack.top().second != static_cast<int>(close_idx)) {
                while (stack.size() > 1) {
                    stack.pop();
                }
                stack.pop();
                stack.push({ current, -1 });
            }
            else {
                stack.pop();
                Iter start = stack.top().first;

                if (stack.top().second != -1) {
                    ++start;
                }

                std::size_t len = std::distance(start, current);
                std::size_t max_len = (max_start == last) ? 0 : std::distance(max_start, max_end);

                if (len > max_len) {
                    max_start = start;
                    max_end = current;
                    ++max_end;
                }
            }
        }
    }

    return std::string(max_start, max_end);
}


int main()
{
// ------------ task 1 ------------
    std::string data = "({[(())]}[{}]({[]}))";

    auto iter = check_correct_brackets(begin(data), end(data));

    if (iter == end(data)) { std::cout << " correct brackets" << std::endl; }
    else { std::cout <<  " incorrect bracket: " << *iter << std::endl; }
    std::cout << std::endl;
    
// ------------ task 3 ------------

    std::string test;
    std::string result;

    test = "())(()()";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << " ->\t" << result << std::endl;

    test = "((()))";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "()()()";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "((()";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "())";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "([{}])";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "([)]";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "{[()]}";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "{[(])}";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    test = "(()[{}])";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << " ->\t" << result << std::endl;

    test = "";
    result = max_correct_subbrackets(test.begin(), test.end());
    std::cout << test << "\t->\t" << result << std::endl;

    return 0;
}
