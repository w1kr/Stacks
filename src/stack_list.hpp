#pragma once

template <typename T>
class _node
{
public:
    T data;
    _node* next;

    _node(T);
    ~_node();
};

template <typename T>
class StackList
{
private:
    _node<T>* node;
    int count;

public:
    StackList();
    ~StackList();

    void push(T);
    void pop();
    T top();
    int size();
};
