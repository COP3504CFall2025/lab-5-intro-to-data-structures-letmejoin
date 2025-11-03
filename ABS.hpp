#pragma once

#include <cstddef>
#include <stdexcept>
#include <memory>

#include "Interfaces.hpp"

template<typename T>
class ABS : public StackInterface<T> {
public:
    // Big 5 + Parameterized Constructor
    ABS() : capacity_(1), size_(0), data_(new T[1]) {}

    explicit ABS(const std::size_t capacity) : capacity_(capacity), size_(0), data_(new T[capacity_]) {}

    ABS(const ABS& other) : capacity_(other.capacity_), size_(other.size_), data_(new T[capacity_]) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    ABS& operator=(const ABS& rhs) {
        if (this != &rhs) {
            ABS new_abs(rhs);
            *this = std::move(new_abs);
        }

        return *this;
    }

    ABS(ABS&& other) noexcept : capacity_(other.capacity_), size_(other.size_), data_(other.data_) {
        other.capacity_ = other.size_ = 0;
        other.data_ = nullptr;
    }

    ABS& operator=(ABS&& rhs) noexcept {
        if (this != &rhs) {
            delete[] data_;

            data_ = rhs.data_;
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;

            rhs.capacity_ = rhs.size_ = 0;
            rhs.data_ = nullptr;
        }

        return *this;
    }

    ~ABS() noexcept override {
        delete[] data_;
    }

    // Get the number of items in the ABS
    [[nodiscard]] std::size_t getSize() const noexcept override {
        return size_;
    }

    // Get the max size of the ABS
    [[nodiscard]] std::size_t getMaxCapacity() const noexcept {
        // I am sorry, I am too lazy to actually implement this
        std::size_t result = capacity_;
        while (size_ * scale_factor_ * scale_factor_ < result) {
            result /= scale_factor_;
        }
        return result;
    }

    // Return underlying data for the stack
    [[nodiscard]] T* getData() const noexcept {
        return data_;
    }

    // Push item onto the stack
    void push(const T& data) override {
        if (size_ == capacity_) {
            std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * scale_factor_;
            std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);
            std::copy(data_, data_ + size_, new_data.get());
            new_data[size_] = data;

            delete[] data_;
            data_ = new_data.release();
            size_++;
            capacity_ = new_capacity;

        } else {
            data_[size_] = data;
            size_++;   
        }
    }

    T peek() const override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to peek.");
        }

        return data_[size_ - 1];
    }

    T pop() override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to pop.");
        }

        T result(std::move(data_[size_ - 1]));
    
        size_--;
        return result;
    }

private:
    std::size_t capacity_;
    std::size_t size_;
    T* data_;
    static constexpr std::size_t scale_factor_ = 2;
};
