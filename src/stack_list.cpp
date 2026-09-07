#include "stack_list.hpp"
#include <iostream>
#include <stdexcept>


template <typename T>
_node<T>::_node(T x)
{
    this->data = x;
    this->next = nullptr;
}


template <typename T>
_node<T>::~_node()
{
}


template <typename T>
StackList<T>::StackList()
{
    this->node = nullptr;
    this->count = 0;
}


template <typename T>
StackList<T>::~StackList()
{
    while (this->count != 0) {
        _node<T>* temp = this->node->next;
        delete this->node;
        this->node = temp;
        this->count--;
    }

    std::cout << " stacklist was cleared!" << std::endl;
}


template <typename T>
void StackList<T>::push(T x)
{
    _node<T>* tempptr = this->node;
    _node<T>* newnode = new _node<T>(x);

    this->node = newnode;
    this->node->next = tempptr;
    this->count++;
}


template <typename T>
void StackList<T>::pop()
{
    if (this->count == 0) { throw std::out_of_range(" pop: stack is already empty!"); }
    _node<T>* temp = this->node->next;
    delete this->node;
    this->node = temp;
    this->count--;
}


template <typename T>
T StackList<T>::top()
{
    if (!this->node) { throw std::out_of_range(" top: stack is empty!"); }
    return this->node->data;
}


template <typename T>
int StackList<T>::size()
{
    return this->count;
}
