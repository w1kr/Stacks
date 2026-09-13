// #include "stack.hpp"
// #include "stack_list.hpp"
#include "stack_combined.hpp"

#include <iostream>
#include <chrono>

int main()
{
// ----------- Stack Dynamic -------------
    /* try
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

    return 0; */

// ------------ Stack List ------------
    /* try
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

    return 0; */


// ------------ Stack Combined  ------------
    try
    {
// push tests
        {
            StackCombined<int> stackcombined(GrowthPolicy::Fixed);

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Fixed push:\t\t" << duration.count() << "\tmillisec" << std::endl;
        }
        {
            StackCombined<int> stackcombined(GrowthPolicy::Linear);

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Linear push:\t\t" << duration.count() << "\tmillisec" << std::endl;
        }
        {
            StackCombined<int> stackcombined(GrowthPolicy::Double);

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Double push:\t\t" << duration.count() << "\tmillisec" << std::endl;
        }
        {
            StackCombined<int> stackcombined(GrowthPolicy::Exponential);

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Exponential push:\t" << duration.count() << "\tmillisec" << std::endl << std::endl;
        }
// tests pop
        {
            StackCombined<int> stackcombined(GrowthPolicy::Fixed);
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.pop(); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Fixed pop:\t\t" << duration.count() << "\tmillisec" << std::endl;
        }
        {
            StackCombined<int> stackcombined(GrowthPolicy::Linear);
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.pop(); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Linear pop:\t\t" << duration.count() << "\tmillisec" << std::endl;
        }
        {
            StackCombined<int> stackcombined(GrowthPolicy::Double);
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.pop(); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Double pop:\t\t" << duration.count() << "\tmillisec" << std::endl;
        }
        {
            StackCombined<int> stackcombined(GrowthPolicy::Exponential);
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.push(i); }

            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 1; i <= 1000000; ++i) { stackcombined.pop(); }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << "Exponential pop:\t" << duration.count() << "\tmillisec" << std::endl;
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
