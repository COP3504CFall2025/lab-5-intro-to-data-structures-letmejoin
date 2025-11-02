#pragma once

#include <iostream>
#include <memory>

template <typename T>
class LinkedList {
public:
	struct Node {
		T data;
		Node* prev;
		Node* next;
	};

	// Behaviors
	void printForward() const {
		if (count > 0) {
			std::cout << head->data;
			Node* curr = head->next;
			while (curr != nullptr) {
				std::cout << ' ' << curr->data;
				curr = curr->next;
			}
			std::cout << std::endl;
		}
	}

	void printReverse() const {
		if (count > 0) {
			std::cout << tail->data;
			Node* curr = tail->prev;
			while (curr != nullptr) {
				std::cout << ' ' << curr->data;
				curr = curr->prev;
			}
			std::cout << std::endl;
		}
	}

	// Accessors
	[[nodiscard]] unsigned int getCount() const {
		return count;
	}

	Node* getHead() {
		return head;
	}

	const Node* getHead() const {
		return head;
	}

	Node* getTail() {
		return tail;
	}

	const Node* getTail() const {
		return tail;
	}

	// Insertion
	void addHead(const T& data) {
		std::unique_ptr<Node> new_node = std::make_unique<Node>(data, nullptr, head);

		if (count == 0) {
			head = tail = new_node.release();
			count = 1;

		} else {
			head = head->prev = new_node.release();
			count++;
		}
	}

	void addTail(const T& data) {
		std::unique_ptr<Node> new_node = std::make_unique<Node>(data, tail, nullptr);

		if (count == 0) {
			head = tail = new_node.release();
			count = 1;
		
		} else {
			tail = tail->next = new_node.release();
			count++;
		}
	}

	// Removal
	bool removeHead() {
		if (count == 0) {
			return false;
		}

		if (count == 1) {
			delete head;
			head = tail = nullptr;
			count = 0;

		} else {
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
			count--;
		}

		return true;
	}

	bool removeTail() {
		if (count == 0) {
			return false;
		}

		if (count == 1) {
			delete tail;
			head = tail = nullptr;
			count = 0;

		} else {
			tail = tail->prev;
			delete tail->next;
			tail->next = nullptr;
			count--;
		}

		return true;
	}

	void clear() {
		Node* curr = head;
		while (curr != nullptr) {
			Node* next = curr->next;
			delete curr;
			curr = next;
		}

		head = tail = nullptr;
		count = 0;
	}

	// Operators
	LinkedList<T>& operator=(LinkedList<T>&& other) noexcept {
		if (this != &other) {
			clear();

			head = other.head;
			tail = other.tail;
			count = other.count;
			
			other.head = other.tail = nullptr;
			other.count = 0;
		}
		return *this;
	}

	LinkedList<T>& operator=(const LinkedList<T>& rhs) {
		if (this != &rhs) {
			LinkedList<T> new_list(rhs);
			*this = std::move(new_list);
		}
		return *this;
	}

	// Construction/Destruction
	LinkedList() : head(nullptr), tail(nullptr), count(0) {}

	LinkedList(const LinkedList<T>& list) : head(nullptr), tail(nullptr), count(0) {
		Node* curr = list.head;
		while (curr != nullptr) {
			addTail(curr->data);
			curr = curr->next;
		}
	}

	LinkedList(LinkedList<T>&& other) noexcept : head(other.head), tail(other.tail), count(other.count) {
		other.head = other.tail = nullptr;
		other.count = 0;
	}

	~LinkedList() {
		clear();
	}

private:
	// Stores pointers to first and last nodes and count
	Node* head;
	Node* tail;
	unsigned int count;
};
