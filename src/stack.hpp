#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>
#include <cstddef>

const int CAPACITY = 64;

template <typename T>
class Stack
{
private:
    T* data_;
    std::size_t count_;
    std::size_t capacity_;

    void resize(const std::size_t& new_capacity)
    {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; count_; ++i) {
            new_data[i] = std::move(data_[i]);
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    Stack(const std::size_t& capacity = CAPACITY) : data_(new T[capacity]), count_(0), capacity_(capacity) {}

    ~Stack()
    {
        delete[] data_;
        std::cout << " stack was cleared!" << std::endl;
    }

    // work


    void push(const T& x)
    {
        if (count_ < capacity_) {
            data_[count_++] = x;
        }
        else {
            capacity_ *= 2;

            T* newdata = new T[capacity_];
            for (int i = 0; i < count_; ++i) {
                newdata[i] = data_[i];
            }

            delete[] data_;
            data_ = newdata;
            data_[count_++] = x;
        }
    }

    void pop()
    {
        if (count_ == 0) { throw std::invalid_argument(" pop: stack is already empty!"); }
        else {
            if (capacity_ > count_ * 2) {
                    capacity_ /= 2;

                    T* newdata = new T[capacity_];
                    for (int i = 0; i < count_; ++i) {
                        newdata[i] = data_[i];
                    }

                    delete[] data_;
                    data_ = newdata;
            }
            count_--;
        }
    }

    T top() 
    {
        if (count_ == 0) { throw std::out_of_range(" top: stack is empty!"); }
        return data_[count_ - 1];
    }

    int size() 
    {
        return count_;
    }
};
