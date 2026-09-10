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

    ~Stack() { delete[] data_; }

    Stack(const Stack& other) : data_(new T[other.capacity_]), count_(other.count_), capacity_(other.capacity_) 
    {
        for (std::size_t i = 0; i < count_; ++i) { data_[i] = std::move(other.data_[i]); }
    }

    Stack& operator=(const Stack& other) 
    {
        if (this == &other) { return *this; }

        T* new_data = new T[other.capacity_];
        for (std::size_t i = 0; i < other.count_; ++i) { new_data[i] = other.data_[i]; }
        
        delete[] data_;
        data_ = new_data;
        count_ = other.count_;
        capacity_ = other.capacity_;

        return *this;
    }

    Stack(Stack&& other) noexcept : data_(other.data_), count_(other.count_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.count_ = 0;
        other.capacity_ = 0;
    }

    Stack& operator=(Stack&& other) noexcept 
    {
        if (this == &other) { return *this; }

        delete[] data_;
        data_ = other.data_;
        count_ = other.count_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.count_ = 0;
        other.capacity_ = 0;

        return *this;
    }


    void push(const T& x)
    {
        if (count_ == capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[count_++] = x;
    }

    void push(const T&& x)
    {
        if (count_ == capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[count_++] = std::move(x);
    }

    void pop()
    {
        if (count_ == 0) { throw std::out_of_range(" pop: stack is already empty!"); }
        count_--;
    }

    T& top() 
    {
        if (count_ == 0) { throw std::out_of_range(" top: stack is empty!"); }
        return data_[count_ - 1];
    }

    std::size_t size() 
    {
        return count_;
    }
};
