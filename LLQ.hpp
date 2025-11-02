#pragma once

#include <stdlib.h>
#include <stdexcept>

#include "Interfaces.hpp"
#include "LinkedList.hpp"

template <typename T>
class LLQ : public QueueInterface<T> {
public:
    // Constructor
    LLQ() = default;

    // Insertion
    void enqueue(const T& item) override {
        list.addTail(item);
    }

    // Deletion
    T dequeue() override {
        if (list.getCount() == 0) {
			throw std::out_of_range("No elements to dequeue.");
        }

        T item = list.getHead()->data;
        list.removeHead();
        return item;
    }

    // Access
    T peek() const override {
        if (list.getCount() == 0) {
			throw std::out_of_range("No elements to peek.");
        }

        return list.getHead()->data;
    }

    // Getter
    std::size_t getSize() const noexcept override {
        return list.getCount();
    }

private:
    LinkedList<T> list;
};
