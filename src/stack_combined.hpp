#pragma once

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>

const std::size_t DEFAULT_CAPACITY = 16;

enum class GrowthPolicy 
{
    Fixed,
    Linear,
    Double,
    Exponential
};


template<typename T>
struct DynamicData
{
    T* data_;
    std::size_t count_;
    std::size_t capacity_;

    DynamicData(const std::size_t& capacity) : data_(nullptr), count_(0), capacity_(capacity) 
    {
        data_ = new T[capacity_];
    }

    DynamicData(const std::size_t&& capacity) : data_(nullptr), count_(0), capacity_(std::move(capacity)) 
    {
        data_ = new T[capacity_];
    }

    ~DynamicData()
    {
        delete[] data_;
    }
};


template<typename T>
struct StackNode
{
    DynamicData<T>* data_;
    StackNode* next_;

    StackNode(const std::size_t& capacity) : data_(new DynamicData<T>(capacity)), next_(nullptr) {}

    StackNode(const std::size_t&& capacity) : data_(new DynamicData<T>(std::move(capacity))), next_(nullptr) {}

    ~StackNode() {}
};


template <typename T>
class StackCombined
{
private:
    StackNode<T>* head_;
    std::size_t count_;
    GrowthPolicy policy_;

    std::size_t grow_capacity(std::size_t current_capacity)
    {
        switch (policy_) {
            case GrowthPolicy::Fixed:
                return current_capacity;
            case GrowthPolicy::Linear:
                return current_capacity + DEFAULT_CAPACITY;
            case GrowthPolicy::Double:
                return current_capacity * 2;
            case GrowthPolicy::Exponential:
                return current_capacity * 3;
            default:
                return 0; // pass
        }
    }

public:
    StackCombined(GrowthPolicy policy) : head_(new StackNode<T>(DEFAULT_CAPACITY)), count_(1), policy_(policy) {}

    ~StackCombined()
    {
        while (head_ != nullptr) {
            StackNode<T>* next = head_->next_;
            delete head_->data_;
            delete head_;
            head_ = next;
        }
        count_ = 0;
    }

    StackCombined(const StackCombined& other) : head_(nullptr), count_(0)
    {
        StackNode<T>** current = &head_;

        for (StackNode<T>* source = other.head_; source != nullptr; source = source->next_) {
            *current = new StackNode<T>(source->data_->count_);
            (*current)->data_->capacity_ = source->data_->capacity_;
            (*current)->data_->count_ = source->data_->count_;

            for (size_t i = 0; i < source->data_->count_; ++i) {
                (*current)->data_->data_[i] = source->data_->data_[i];
            }

            current = &((*current)->next_);
            count_++;
        }
    }

    StackCombined& operator=(const StackCombined& other)
    {
        if (this == &other) { return *this; }

        StackCombined temp(other);
        std::swap(head_, temp.head_);
        std::swap(count_, temp.count_);

        return *this;
    }

    StackCombined(StackCombined&& other) noexcept : head_(other.head_), count_(other.count_)
    {
        other.head_ = nullptr;
        other.count_ = 0;
    }

    StackCombined& operator=(StackCombined&& other) noexcept
    {
        if ( this == &other) { return *this; }

        while (head_ != nullptr)
        {
            StackNode<T>* next = head_->next_;
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
        if (count_ == 1) {
            if (head_->data_->count_ < head_->data_->capacity_) {
                head_->data_->data_[head_->data_->count_++] = x;
            }
            else {
                StackNode<T>* new_node = new StackNode<T>(grow_capacity(head_->data_->capacity_));
                new_node->next_ = head_;
                head_ = new_node;
                head_->data_->data_[head_->data_->count_++] = x;
            }
        }
        else {
            if (head_->data_->count_ != 0) {
                if (head_->data_->count_ < head_->data_->capacity_) {
                    head_->data_->data_[head_->data_->count_++] = x;
                }
                else {
                    StackNode<T>* new_node = new StackNode<T>(grow_capacity(head_->data_->capacity_));
                    new_node->next_ = head_;
                    head_ = new_node;
                    head_->data_->data_[head_->data_->count_++] = x;
                }
            }
            else {
                if (head_->next_->data_->count_ < head_->next_->data_->capacity_) {
                    head_->next_->data_->data_[head_->next_->data_->count_++] = x;
                }
                else {
                    head_->data_->data_[head_->data_->count_++] = x;
                }
            }
        }
    }

    void push(T&& x)
    {
        if (count_ == 1) {
            if (head_->data_->count_ < head_->data_->capacity_) {
                head_->data_->data_[head_->data_->count_++] = std::move(x);
            }
            else {
                StackNode<T>* new_node = new StackNode<T>(grow_capacity(head_->data_->capacity_));
                new_node->next_ = head_;
                head_ = new_node;
                head_->data_->data_[head_->data_->count_++] = std::move(x);
                count_++;
            }
        }
        else {
            if (head_->data_->count_ != 0) {
                if (head_->data_->count_ < head_->data_->capacity_) {
                    head_->data_->data_[head_->data_->count_++] = std::move(x);
                }
                else {
                    StackNode<T>* new_node = new StackNode<T>(grow_capacity(head_->data_->capacity_));
                    new_node->next_ = head_;
                    head_ = new_node;
                    head_->data_->data_[head_->data_->count_++] = std::move(x);
                    count_++;
                }
            }
            else {
                if (head_->next_->data_->count_ < head_->next_->data_->capacity_) {
                    head_->next_->data_->data_[head_->next_->data_->count_++] = std::move(x);
                }
                else {
                    head_->data_->data_[head_->data_->count_++] = std::move(x);
                }
            }
        }
    }

    void pop()
    {
        if (count_ == 1) {
            if (head_->data_->count_ == 0) { throw std::out_of_range(" pop: stack is empty!"); }
            else {
                head_->data_->count_--;
            }
        }
        else {
            if (head_->data_->count_ == 0 && head_->next_->data_->count_ == 1) {
                head_->data_->count_--;
                StackNode<T>* temp = head_->next_;
                head_->data_->~DynamicData();
                head_->~StackNode();
                head_ = temp;
                head_->data_->count_--;
                count_--;
            }
            else if (head_->data_->count_ == 0) {
                head_->next_->data_->count_--;
            }
            else {
                head_->data_->count_--;
            }
        }
    }

    T& top()
    {
        if (count_ == 1 && head_->data_->count_ == 0) {
            throw std::out_of_range(" top: stacklist is empty!"); 
        }
        else {
            if (head_->data_->count_ == 0) {
                return head_->next_->data_->data_[head_->next_->data_->count_ - 1];
            }
            else {
                return head_->data_->data_[head_->data_->count_ - 1];
            }
        }
    }

    std::size_t size()
    {
        std::size_t res = 0;
        for (StackNode<T>* current = head_; current != nullptr; current = current->next_) {
            res += current->data_->count_;
        }
        return res;
    }


};



