#pragma once

#include <iostream>
#include <stdexcept>


template <typename T>
class _node
{
public:
    T data;
    _node* next;

    _node(const T& x)
    {
        this->data = x;
        this->next = nullptr;
    }

    ~_node()
    {
    }
};

template <typename T>
class StackList
{
private:
    _node<T>* node;
    int count;

public:
    StackList()
    {
        this->node = nullptr;
        this->count = 0;
    }

    ~StackList()
    {
        while (this->count != 0) {
            _node<T>* temp = this->node->next;
            delete this->node;
            this->node = temp;
            this->count--;
        }

        std::cout << " stacklist was cleared!" << std::endl;
    }

    void push(const T& x)
    {
        _node<T>* tempptr = this->node;
        _node<T>* newnode = new _node<T>(x);

        this->node = newnode;
        this->node->next = tempptr;
        this->count++;
    }

    void pop()
    {
        if (this->count == 0) { throw std::out_of_range(" pop: stack is already empty!"); }
        _node<T>* temp = this->node->next;
        delete this->node;
        this->node = temp;
        this->count--;
    }

    T top()
    {
        if (!this->node) { throw std::out_of_range(" top: stack is empty!"); }
        return this->node->data;
    }

    int size()
    {
        return this->count;
    }
};
