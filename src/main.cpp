#include "max_correct_subbrackets.hpp"
#include <iostream>
#include <vector>
#include <deque>


int main(void)
{    
// ------------ task 2 ------------
/*     std::string data = "((())*";
    bool res = is_brackets_restorable(begin(data), end(data));
    if (res) { std::cout << " true" << std::endl; }
    else { std::cout << " false" << std::endl; } */


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
        std::string res = max_correct_subbrackets(input);
        std::cout << input << " - " << res << std::endl;
    }

    return 0;
}
