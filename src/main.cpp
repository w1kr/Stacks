#include "stack.hpp"
#include "stack_list.hpp"
#include <iostream>

const int CAPACITY = 1024;

int main()
{
// ----------- Dynamic array (int) -------------
    try
    {        
        Stack<int> stack(CAPACITY);
        for (int i = 1; i <= 1000; ++i) { stack.push(i); }
        for (int i = 1; i <= 500; ++i) { stack.pop(); }

        std::cout << stack.top() << std::endl;
        stack.pop();

        while (stack.size() > 0) {
            std::cout << stack.top() << " ";
            stack.pop();
        }
        std::cout << std::endl;

        for (int i = 1; i <= 100; ++i) { stack.push(i); }
        for (int i = 10000; i <= 20000; ++i) { stack.push(i); }
        
        while (stack.size() > 0) {
            std::cout << stack.top() << " ";
            stack.pop();
        }
        std::cout << std::endl;

        // ----- if size == 0 -----
        // stack.top();

        // for (int i = 1; i <= 1000; ++i) { stack.push(i); }
        // for (int i = 1; i <= 1500; ++i) { stack.pop(); }

        // Stack<int> temp_stack(0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }


// ---------- Dynamic array (char) ------------
/*     try
    {        
        Stack<char> stack(CAPACITY);
        for (int i = 1; i <= 100; ++i) { stack.push('A'); }
        for (int i = 1; i <= 200; ++i) { stack.push('B'); }
        for (int i = 1; i <= 100; ++i) { stack.push('C'); }

        std::cout << stack.size() << std::endl;

        std::cout << stack.top() << std::endl;
        stack.pop();

        std::cout << stack.size() << std::endl;

        while (stack.size() > 0) {
            std::cout << stack.top() << " ";
            stack.pop();
        }
        std::cout << std::endl;

        std::cout << stack.size() << std::endl;
        std::cout << std::endl;

        for (int i = 1; i <= 100; ++i) { stack.push('1'); }
        for (int i = 1; i <= 200; ++i) { stack.push('2'); }
        for (int i = 1; i <= 150; ++i) { stack.push('3'); }
        for (int i = 1; i <= 1000; ++i) { stack.push('4'); }
        
        std::cout << stack.size() << std::endl;

        while (stack.size() > 0) {
            std::cout << stack.top() << " ";
            stack.pop();
        }
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
 */

// ------------ List (int) ------------
    /* try
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
    } */


// ------------ List (char) ------------
    /* try
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
    } */

    return 0;
}

