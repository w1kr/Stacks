#include "max_correct_subbrackets.hpp"
#include <iostream>
#include <vector>
<<<<<<< HEAD
#include <deque>


int main(void)
{    
// ------------ task 2 ------------
/*     std::string data = "((())*";
    bool res = is_brackets_restorable(begin(data), end(data));
    if (res) { std::cout << " true" << std::endl; }
    else { std::cout << " false" << std::endl; } */


=======


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


template<typename Iter>
std::string max_correct_subbrackets(Iter first, Iter last)
{
    StackCombined<std::pair<Iter, int>> stack(GrowthPolicy::Exponential);
    std::string open_brackets  = "([{<";
    std::string close_brackets = ")]}>";
    Iter max_start = last;
    Iter max_end = last;

    stack.push({first, -1});

    for (; first != last; ++first) {
        int idx = open_brackets.find(*first);

        if (idx >= 0) {
            stack.push({ first, idx });
        }
        else {
            stack.pop();

            if (stack.size() == 0) {
                stack.push( {first, -1});
            }
            else {
                Iter start = stack.top();
                Iter end = first;
                ++start;
                ++end;

                if (std::distance(start, end) > std::distance(max_start, max_end)) {
                    max_start = start;
                    max_end = end;
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
    
>>>>>>> 904d8db0d651ebe774569a125c19b8928d1b6c75
// ------------ task 3 ------------
    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"(", ""},
        {")", ""},
        {"()", "()"},
        {"((", ""},
        {"))", ""},
        {"())", "()"},
        {"(()", "()"},
        {"((()))", "((()))"},
        {"()()", "()()"},
        {"()()()", "()()()"},
        {"(()())", "(()())"},
        {"((()))()", "((()))()"},
        {"()((()))", "()((()))"},
        {"())((()))", "((()))"},
        {"((())", "(())"},
        {"(()))", "(())"},
        {"())()", "()"},
    };

    for (const auto& [input, expected] : tests) {
<<<<<<< HEAD
        std::string res = max_correct_subbrackets(input);
        std::cout << input << " - " << res << std::endl;
=======
        std::string result = max_correct_subbrackets(begin(input), end(input));
        std::cout << input
                << " -> " << result
                << (result == expected ? "  OK" : "  ERROR")
                << std::endl;
>>>>>>> 904d8db0d651ebe774569a125c19b8928d1b6c75
    }

    return 0;
}
