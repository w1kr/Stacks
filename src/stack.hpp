#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class Stack
{
private:
    T* data;
    size_t count;
    size_t capacity;

public:
    Stack(int capacity)
    {
        if (capacity <= 0) { throw std::invalid_argument(" Stack: capacity must be greater than 0!"); }
        this->capacity = capacity;
        this->count = 0;
        
        this->data = new T[this->capacity];
    }

    ~Stack()
    {
        delete[] this->data;
        std::cout << " stack was cleared!" << std::endl;
    }

    void push(T x)
    {
        if (this->count < this->capacity) {
            this->data[count++] = x;
        }
        else {
            this->capacity *= 2;

            T* newdata = new T[this->capacity];
            for (int i = 0; i < this->count; ++i) {
                newdata[i] = this->data[i];
            }

            delete[] this->data;
            this->data = newdata;
            this->data[count++] = x;
        }
    }

    void pop()
    {
        if (this->count == 0) { throw std::invalid_argument(" pop: stack is already empty!"); }
        else {
            if (this->capacity > this->count * 2) {
                    this->capacity /= 2;

                    T* newdata = new T[this->capacity];
                    for (int i = 0; i < this->count; ++i) {
                        newdata[i] = this->data[i];
                    }

                    delete[] this->data;
                    this->data = newdata;
            }
            count--;
        }
    }

    T top() 
    {
        if (this->count == 0) { throw std::out_of_range(" top: stack is empty!"); }
        return this->data[count - 1];
    }

    int size() 
    {
        return this->count;
    }
};
