#pragma once

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>

template<typename T>
struct DynamicData
{
    T* data_;
    std::size_t count_;

    DynamicData();
    ~DynamicData();
};


template<typename T>
struct Node
{
    DynamicData<T>* data_;
    Node* next_;

    Node();
    ~Node();
};


template <typename T>
class StackList
{
private:
    Node<T>* head_;
    std::size_t count_;

public:

};


