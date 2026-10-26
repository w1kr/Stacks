#pragma once

#include "stack_combined.hpp"
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <utility>


using vector_graph = std::vector<std::vector<std::size_t>>;
using set_graph = std::vector<std::vector<std::size_t>>;


// --------------------------------------------------
// count_if - for counting odd vertexes
// std::count_if(g.begin(), 
//        g.end(),
//        [](auto& graph)
//        { return graph.size() % 2; });
//
// std::size_t start_vertex; //(first odd vertex);
// std::find_if - for find first odd vertex
// --------------------------------------------------


std::vector<std::size_t> find_euler_path_vector(vector_graph graph) 
{
    if (graph.size() <= 1)
    {
        return {};
    }

    std::size_t vertex_count = graph.size() - 1;
    std::size_t odd_degree_count = 0;
    std::size_t start_vertex = 1;
    std::size_t edge_count = 0;

    for (std::size_t vertex = 1; vertex <= vertex_count; ++vertex) 
    {
        edge_count += graph[vertex].size();

        if (graph[vertex].size() % 2) 
        {
            odd_degree_count++;
            start_vertex = vertex;
        }
    }

    if (odd_degree_count != 0 && odd_degree_count != 2) 
    {
        return {};
    }

    if (odd_degree_count == 0) 
    {
        for (std::size_t vertex = 1; vertex <= vertex_count; ++vertex) 
        {
            if (!graph[vertex].empty()) 
            {
                start_vertex = vertex;
                break;
            }
        }
    }

    std::vector<std::size_t> path;
    StackCombined<std::size_t> stack;
    stack.push(start_vertex);

    while (stack.size()) 
    {
        std::size_t u = stack.top();

        if (graph[u].empty())
        {
            path.push_back(u);
            stack.pop();
        } 
        else 
        {
            std::size_t v = graph[u].back();
            graph[u].pop_back();

            /* for (std::size_t i = 0; i < graph[v].size(); ++i) 
            {
                if (graph[v][i] == u)
                {
                    graph[v].erase(graph[v].begin() + i);
                    break;
                }
            } */

            // --------------------------------------------------------------------------
            std::iter_swap(std::find(graph[v].begin(), graph[v].end(), u), graph[v].end() - 1);
            graph[v].pop_back();
            // --------------------------------------------------------------------------

            stack.push(v);
        }
    }

    if (path.size() != edge_count / 2 + 1) 
    {
        return {};
    }

    std::reverse(path.begin(), path.end());
    return path;
}


vector_graph generate_euler_graph(std::size_t n, std::size_t m)
{
    if (n < 1 ||
        m < 0 ||
        m > n * (n - 1) / 2 ||
        n - 1 > m)
    {
        return {};
    }

    vector_graph graph(n + 1);
    std::size_t edge_count = 0;

    for (std::size_t vertex = 1; vertex < n; ++vertex)
    {
        graph[vertex].push_back(vertex + 1);
        graph[vertex + 1].push_back(vertex);
        ++edge_count;
    }

    if (edge_count == m)
    {
        return graph;
    }

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<std::size_t> vertex_distribution(1, n);

    std::size_t attempts = 0;
    std::size_t max_attempts = m * 100 + 100;

    while (edge_count < m && attempts < max_attempts)
    {
        ++attempts;

        size_t u = vertex_distribution(generator);
        size_t v = vertex_distribution(generator);
        
        if (u == v)
        {
            continue;
        }

        bool edge_exists = false;

        for (std::size_t neigh : graph[u])
        {
            if (neigh == v)
            {
                edge_exists = true;
                break;
            }
        }

        if (edge_exists)
        {
            continue;
        }

        graph[u].push_back(v);
        graph[v].push_back(u);

        std::size_t odd_degree_count = 0;

        for (std::size_t vertex = 1; vertex <= n; ++vertex)
        {
            if (graph[vertex].size() % 2)
            {
                ++odd_degree_count;
            }
        }

        if (odd_degree_count != 0 && odd_degree_count != 2)
        {
            graph[u].pop_back();
            graph[v].pop_back();
        }
        else
        {
            ++edge_count;
        }
    }

    if (edge_count != m)
    {
        return {};
    }

    return graph;
}


template<class graph_t>
vector_graph generate_euler_graph_v2(std::size_t verts, std::size_t edges)
{
    if (!verts)
    {
        return {};
    }
    
    graph_t graph(verts + 1);

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<std::size_t> vertex_distribution(1, verts);

    std::size_t start =  vertex_distribution(generator);
    std::size_t edge_count = 0;

    while (edge_count < edges)
    {
        std::size_t next = vertex_distribution(generator);

        if (next != start)
        {
            // add_edge(graph, start, next);
            graph[start].push_back(next);
            graph[next].push_back(start);
            start = next;
            edge_count++;
        }
    }

    return graph;
}


using egraph = std::vector<std::vector<std::pair<int, int>>>;


egraph convert_to_egraph(const vector_graph& g)
{
    egraph eg(g.size() - 1);
    size_t e_id = 0;

    for (size_t i = 0; i < g.size(); ++i)
    {
        for (size_t u : g[i])
        {
            if (i < u)
            {
                eg[i].emplace_back(u, e_id);
                eg[u].emplace_back(i, e_id);
                e_id++;
            }
        }
    }

    return eg;
}


/* std::vector<std::size_t> find_euler_path_egraph(const vector_graph& g)
{
    std::vector<std::size_t> res;
    egraph eg = convert_to_egraph(g);
    std::vector<std::size_t> visited_count(eg.size());
    // size_t E = std::accumulate(g.begin(), g.end(), 0, [](size_t sum, const)); // edge count
    // std::vector<bool> visited(E, false) 
    
    return res;
} */
