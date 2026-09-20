#include "stack_combined.hpp"
#include <iostream>


long long correct_brackets(const std::string& data)
{
    StackCombined<char> stack = StackCombined<char>(GrowthPolicy::Exponential);

    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == '(' || data[i] == '[' || data[i] == '{' || data[i] == '<') {
            stack.push(data[i]);
        }
        else {
            if (stack.size() == 0) { return i; }
            else {
                switch (data[i])
                {
                case ')': 
                    if (stack.top() != '(') { return i; }
                    break;
                case ']': 
                    if (stack.top() != '[') { return i; }
                    break;
                case '}': 
                    if (stack.top() != '{') { return i; }
                    break;
                case '>': 
                    if (stack.top() != '<') { return i; }
                    break;
                default:
                    break;
                }
                stack.pop();
            }
        }
    }

    if (stack.size() != 0) { return data.size() - 1; }

    return -1;
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
    std::string data;
    const size_t N = 10000000;
    data.append(N / 2, '(');
    data.append(N / 2, ')');

    long long index = correct_brackets(data);

    if (index == -1) { std::cout << " correct brackets" << std::endl; }
    else { std::cout <<  " incorrect bracket: " << index << std::endl; }

    std::cout << std::endl;
    
// ------------ task 3 ------------
    std::string data2 = "())(()()";
    std::cout << data2 << std::endl;
    std::string res = max_correct_subbrackets(data2);
    std::cout << res << std::endl;

    return 0;
}
