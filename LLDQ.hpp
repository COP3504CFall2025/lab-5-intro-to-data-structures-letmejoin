#pragma once

#include <cstddef>
#include <stdexcept>

#include "Interfaces.hpp"
#include "LinkedList.hpp"

template <typename T>
class LLDQ : public DequeInterface<T> {
public:
    // Constructor
    LLDQ() = default;

    // Core Insertion Operations
    void pushFront(const T& item) override {
        list.addHead(item);
    }

    void pushBack(const T& item) override {
        list.addTail(item);
    }

    // Core Removal Operations
    T popFront() override {
        if (list.getCount() == 0) {
			throw std::out_of_range("No elements to pop.");
        }

        T item = list.getHead()->data;
        list.removeHead();
        return item;
    }

    T popBack() override {
        if (list.getCount() == 0) {
			throw std::out_of_range("No elements to pop.");
        }

        T item = list.getTail()->data;
        list.removeTail();
        return item;
    }

    // Element Accessors
    const T& front() const override {
        if (list.getCount() == 0) {
			throw std::out_of_range("No elements to peek.");
        }

        return list.getHead()->data;
    }

    const T& back() const override {
        if (list.getCount() == 0) {
			throw std::out_of_range("No elements to peek.");
        }

        return list.getTail()->data;
    }

    // Getter
    std::size_t getSize() const noexcept override {
        return list.getCount();
    }

private:
    LinkedList<T> list;
};
