#pragma once

#include <cstddef>
#include <stdexcept>
#include <memory>

#include "Interfaces.hpp"

template<typename T>
class ABQ : public QueueInterface<T> {
public:
    // Constructors + Big 5
    ABQ() : capacity_(1), size_(0), data_(new T[1]) {}

    explicit ABQ(const std::size_t capacity) : capacity_(capacity), size_(0), data_(new T[capacity_]) {}

    ABQ(const ABQ& other) : capacity_(other.capacity_), size_(other.size_), data_(new T[capacity_]) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    ABQ& operator=(const ABQ& rhs) {
        if (this != &rhs) {
            ABQ new_abq(rhs);
            *this = std::move(new_abq);
        }

        return *this;
    }

    ABQ(ABQ&& other) noexcept : capacity_(other.capacity_), size_(other.size_), data_(other.data_) {
        other.capacity_ = other.size_ = 0;
        other.data_ = nullptr;
    }

    ABQ& operator=(ABQ&& rhs) noexcept {
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

    ~ABQ() noexcept override {
        delete[] data_;
    }

    // Getters
    [[nodiscard]] std::size_t getSize() const noexcept override {
        return size_;
    }

    [[nodiscard]] std::size_t getMaxCapacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] T* getData() const noexcept {
        return data_;
    }

    // Insertion
    void enqueue(const T& data) override {
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

    // Access
    T peek() const override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to peek.");
        }

        return data_[0];
    }

    // Deletion
    T dequeue() override {
        if (size_ == 0) {
			throw std::runtime_error("No elements to dequeue.");
        }

        T result(std::move(data_[0]));
        std::move(data_ + 1, data_ + size_, data_);

        size_--;
        return result;
    }

private:
    std::size_t capacity_;
    std::size_t size_;
    T* data_;
    static constexpr std::size_t scale_factor_ = 2;
};
