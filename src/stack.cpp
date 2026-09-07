#include "stack.hpp"
#include <iostream>
#include <stdexcept>


template<typename T>
Stack<T>::Stack(int capacity)
{
    if (capacity <= 0) { throw std::invalid_argument(" Stack: capacity must be greater than 0!"); }
    this->capacity = capacity;
    this->count = 0;
    
    this->data = new T[this->capacity];
}


template<typename T>
Stack<T>::~Stack()
{
    delete[] this->data;
    std::cout << " stack was cleared!" << std::endl;
}


template<typename T>
void Stack<T>::push(T x)
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


template<typename T>
void Stack<T>::pop()
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


template<typename T>
T Stack<T>::top() 
{
    if (this->count == 0) { throw std::out_of_range(" top: stack is empty!"); }
    return this->data[count - 1];
}


template<typename T>
int Stack<T>::size() 
{
    return this->count;
}
