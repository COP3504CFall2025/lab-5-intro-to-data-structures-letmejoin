#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"
#include <utility>

template <typename T>
class ABDQ : public DequeInterface<T> {
public:
    // Big 5
    ABDQ() : capacity_(4), size_(0), front_(0), back_(0), data_(new T[4]) {}

    explicit ABDQ(std::size_t capacity) : capacity_(capacity), size_(0), front_(0), back_(0), data_(new T[capacity_]) {}

    ABDQ(const ABDQ& other) : capacity_(other.capacity_), size_(other.size_), front_(other.front_), back_(other.back_), data_(new T[capacity_]) {
        std::copy(other.data_, other.data_ + capacity_, data_);
    }

    ABDQ(ABDQ&& other) noexcept : capacity_(other.capacity_), size_(other.size_), front_(other.front_), back_(other.back_), data_(other.data_) {
        other.capacity_ = other.size_ = other.front_ = other.back_ = 0;
        other.data_ = nullptr;
    }

    ABDQ& operator=(const ABDQ& other) {
        if (this != &other) {
            ABDQ new_abdq(other);
            *this = std::move(new_abdq);
        }

        return *this;
    }

    ABDQ& operator=(ABDQ&& other) noexcept {
        if (this != &other) {
            delete[] data_;

            capacity_ = other.capacity_;
            size_ = other.size_;
            front_ = other.front_;
            back_ = other.back_;
            data_ = other.data_;

            other.capacity_ = other.size_ = other.front_ = other.back_ = 0;
            other.data_ = nullptr;
        }

        return *this;
    }

    ~ABDQ() override {
        delete[] data_;
    }

    // Insertion
    // NOTE: basic exception safety only
    void pushFront(const T& item) override {
        if (size_ == capacity_) {
            grow();
        }

        front_ = (front_ == 0 ? capacity_ : front_) - 1;
        data_[front_] = item;
        size_++;
    }

    void pushBack(const T& item) override {
        if (size_ == capacity_) {
            grow();
        }

        data_[back_] = item;
        back_ = back_ + 1 == capacity_ ? 0 : back_ + 1;
        size_++;
    }

    // Deletion
    T popFront() override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to pop.");
        }

        T result(std::move(data_[front_]));

        size_--;
        front_ = front_ + 1 == capacity_ ? 0 : front_ + 1;
        return result;
    }

    T popBack() override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to pop.");
        }

        std::size_t new_back = (back_ == 0 ? capacity_ : back_) - 1;
        T result(std::move(data_[new_back]));

        size_--;
        back_ = new_back;
        return result;
    }

    // Access
    const T& front() const override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to peek.");
        }

        return data_[front_];
    }

    const T& back() const override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to peek.");
        }

        return data_[(back_ == 0 ? capacity_ : back_) - 1];
    }

    // Resize
    void grow() {
        ABDQ new_abdq(capacity_ * SCALE_FACTOR);
        new_abdq.size_ = size_;
        new_abdq.front_ = 0;
        new_abdq.back_ = size_;

        for (std::size_t i = 0; i < size_; i++) {
            new_abdq.data_[i] = data_[(front_ + i) % capacity_];
        }

        *this = std::move(new_abdq);
    }

    // Getters
    std::size_t getSize() const noexcept override {
        return size_;
    }

private:
    T* data_;                 // underlying dynamic array
    std::size_t capacity_;    // total allocated capacity
    std::size_t size_;        // number of stored elements
    std::size_t front_;       // index of front element
    std::size_t back_;        // index after the last element (circular)

    static constexpr std::size_t SCALE_FACTOR = 2;
};
