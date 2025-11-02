#pragma once

#include <stdlib.h>
#include <stdexcept>

#include "Interfaces.hpp"
#include "LinkedList.hpp"

template <typename T>
class LLS : public StackInterface<T> {
public:
    // Constructor
    LLS() = default;

    // Insertion
    void push(const T& item) override {
        list.addHead(item);
    }

    // Deletion
    T pop() override {
        if (list.getCount() == 0) {
			throw std::out_of_range("No elements to pop.");
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

    //Getters
    std::size_t getSize() const noexcept override {
        return list.getCount();
    }

private:
    LinkedList<T> list;
};
