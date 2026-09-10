#pragma once

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>


template <typename T>
struct Node
{
    T data_;
    Node* next_;

    Node(const T& data) : data_(data), next_(nullptr) {}
    Node(const T&& data) : data_(std::move(data)), next_(nullptr) {}
};


template <typename T>
class StackList
{
private:
    Node<T>* head_;
    std::size_t count_;

public:
    StackList() : head_(nullptr), count_(0) {}

    ~StackList()
    {
        while (head_ != nullptr)
        {
            Node<T>* next = head_->next_;
            delete head_;
            head_ = next;
        }
        count_ = 0;
    }

    StackList(const StackList& other) : head_(nullptr), count_(0)
    {
        Node<T>** current = &head_;

        for (Node<T>* source = other.head_; source != nullptr; source = source->next_) {
            *current = new Node<T>(source->data_);
            current = &((*current)->next_);
            count_++;
        }
    }

    StackList& operator=(const StackList& other)
    {
        if (this == &other) { return *this; }

        StackList temp(other);
        std::swap(head_, temp.head_);
        std::swap(count_, temp.count_);

        return *this;
    }

    StackList(StackList&& other) noexcept : head_(other.head_), count_(other.count_)
    {
        other.head_ = nullptr;
        other.count_ = 0;
    }

    StackList& operator=(StackList&& other) noexcept
    {
        if ( this == &other) { return *this; }

        while (head_ != nullptr)
        {
            Node<T>* next = head_->next_;
            delete head_;
            head_ = next;
        }
        count_ = 0;

        head_ = other.head_;
        count_ = other.count_;
        other.head_ = nullptr;
        other.count_ = 0;

        return *this;
    }


    void push(const T& x)
    {
        Node<T>* new_node = new Node<T>(x);
        new_node->next_ = head_;
        head_ = new_node;
        count_++;
    }

    void push(T&& x)
    {
        Node<T>* new_node = new Node<T>(std::move(x));
        new_node->next_ = head_;
        head_ = new_node;
        count_++;
    }

    void pop()
    {
        if (count_ == 0) { throw std::out_of_range(" pop: stacklist is empty!"); }

        Node<T>* temp_node = head_;
        head_ = head_->next_;
        delete temp_node;
        count_--;
    }

    T& top()
    {
        if (count_ == 0) { throw std::out_of_range(" top: stacklist is empty!"); }

        return head_->data_;
    }

    std::size_t size()
    {
        return count_;
    }
};
