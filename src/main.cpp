#include "stack.hpp"
#include "stack_list.hpp"
#include <iostream>

#include <string>

int main()
{
// ----------- Dynamic array -------------
    try
    {
        Stack<int> stack(64);

        stack.push(100);
        stack.push(200);
        stack.push(300);
        stack.push(400);
        stack.push(500);

        Stack<int> stack2 = std::move(stack);

        while (stack2.size() > 0) { std::cout << stack2.top() << " "; stack2.pop(); }
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;

// ------------ List ------------
    try
    {
        StackList<int> stacklist;
        stacklist.push(1);
        stacklist.push(2);
        stacklist.push(3);
        stacklist.push(4);

        int a = 100;
        stacklist.push(a);

        std::cout << stacklist.top() << std::endl;
        stacklist.pop();
        std::cout << stacklist.top() << std::endl;

        StackList<int> stacklist2 = stacklist;
        stacklist2.push(100);
        stacklist2.push(200);
        
        stacklist2.top();
        stacklist2.pop();
        stacklist2.top();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
