#include "stack.hpp"
#include "stack_list.hpp"
#include <iostream>

int main()
{
// ----------- Dynamic array (int) -------------
    try
    {
        Stack<int> stack(64);

        stack.push(100);
        std::cout << stack.top() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;

// ------------ List (int) ------------
    try
    {
        StackList<int> stacklist;

        stacklist.push(100);
        stacklist.push(200);
        stacklist.push(300);

        stacklist.pop();
        stacklist.pop();
        stacklist.pop();
        // stacklist.pop();
        // stacklist.top();

        for (int i = 1; i <= 100; ++i) {
            stacklist.push(i);
        }

        std::cout << stacklist.top() << std::endl;

        for (int i = 1; i <= 5; ++i) {
            stacklist.pop();
        }

        std::cout << stacklist.top() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }


// ------------ List (char) ------------
    try
    {
        StackList<char> stacklist;

        stacklist.push('A');
        stacklist.push('B');
        stacklist.push('C');

        stacklist.pop();
        stacklist.pop();
        stacklist.pop();
        // stacklist.pop();
        // stacklist.top();

        for (int i = 1; i <= 50; ++i) {
            stacklist.push('A');
        }
        for (int i = 1; i <= 40; ++i) {
            stacklist.push('B');
        }
        for (int i = 1; i <= 30; ++i) {
            stacklist.push('C');
        }
        
        std::cout << stacklist.top() << std::endl;

        for (int i = 1; i <= 29; ++i) {
            stacklist.pop();
        }

        while (stacklist.size() > 0) {
            std::cout << stacklist.top() << " ";
            stacklist.pop();
        }
        std::cout << std::endl;

        stacklist.push('F');
        std::cout << stacklist.top() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
