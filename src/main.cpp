#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "find_euler_path.hpp"


vector_graph graph = {
    {},
    {2, 3},
    {3, 1},
    {1, 2}
};


int main(void)
{   
    /* std::vector<std::size_t> res = find_euler_path_vector(graph);
    
    if (res.empty())
    {
        std::cout << "euler path not found" << std::endl;
        return 0;
    }

    for (auto& v : res) 
    {
        std::cout << v << " ";
    }
    std::cout << std::endl; */


    vector_graph rand_graph = generate_euler_graph(100, 1000);
    if (rand_graph.empty())
    {
        std::cout << " graph could not be generated" << std::endl;
        return 0;
    }

    std::vector<std::size_t> res2 = find_euler_path_vector(rand_graph);
    
    if (res2.empty())
    {
        std::cout << " euler path not found" << std::endl;
        return 0;
    }
    
    for (auto& v : res2) 
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    return 0;
}
