#include <iostream>
#include <string>
#include "major_element.hpp"


int main(void)
{   
    std::string data = std::string(32, 'A') + std::string(23, 'B') + std::string(23, 'C') + std::string(23, 'D');
    /* std::size_t k = 3;

    if (auto res = find_frequent_element_a(data.begin(), data.end(), k);
        res != data.end())
    {
        std::cout << *res << std::endl;
    }
    else
    {
        std::cout << "element not found" << std::endl;
    } */

    if (auto res = find_frequent_element_b(data.begin(),data.end());
        res != data.end())
    {
        std::cout << *res << std::endl;
    }
    else
    {
        std::cout << "element not found" << std::endl;
    }


    return 0;
}
