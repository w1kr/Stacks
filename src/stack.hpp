#pragma once

template <typename T>
class Stack
{
private:
    T* data;
    int count;
    int capacity;

public:
    Stack(int);
    ~Stack();

    void push(T);
    void pop();
    T top();
    int size();
};
