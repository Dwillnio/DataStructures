#pragma once
#include <exception>
#include <iostream>

#include "List.h"

template<typename T>
class Node
{
private:
	T data;
	Node* next_ptr;

public:

	Node(const T& data_, Node* nptr = nullptr)
	{
		data = data_;
		next_ptr = nptr;
	}

	Node() : data(T()), next_ptr(nullptr) {};

	T& get() { return data; }

	const T& get() const { return data; }

	Node* next() 
	{
		return next_ptr;
	}

	void set_next(Node* const n_ptr)
	{
		next_ptr = n_ptr;
	}

	Node<T>* traverse(int amount)
	{
		if (amount < 0) return this;

		Node<T>* current = this;
		while (amount > 0) {
			if (current == nullptr) return current;
			current = current->next();
			amount--;
		}
		return current;
	}

	~Node() {}

};

template<typename T>
class LLIterator : public IteratorInterface<T> {
private:
	Node<T>* current;

public:
	LLIterator() = delete;

	LLIterator(Node<T>* cur) : current(cur) {};

	T& get() override
	{
		return current->get();
	}

	const T& get() const override
	{
		return current->get();
	}

	T& move(int amount) override
	{
		if (amount < 0) throw new std::exception("Backwards traversing on LinkedList not possible");
		Node<T>* dest = current->traverse(amount);
		if (dest == nullptr) throw new std::exception("Out of bounds");

		current = dest;

		return current->get();
	}

	T& next() override { return move(1); }

	T& prev() override { return move(-1); }

	T& operator+(int amount) override { return move(amount); }

	T& operator-(int amount) override { return move(-amount); }

	T& operator++() override { return next(); }

	T& operator--() override { return prev(); }

	bool equals(const IteratorInterface<T>& iter) const override
	{
		if (typeid(*this) == typeid(iter)) {
			return current == (static_cast<const LLIterator<T>&>(iter)).current;
		}
		else {
			return false;
		}

	}

	bool operator==(const IteratorInterface<T>& iter) const override { return equals(iter); }
	bool operator!=(const IteratorInterface<T>& iter) const override { return !equals(iter); }

	bool is_beg() const override { return false; }
	bool is_end() const override { return current->next() == nullptr; }
	bool is_valid() const override { return current != nullptr; }

	IteratorInterface<T>* copy() const override
	{
		return new LLIterator(current);
	}
};

template<typename T>
class LinkedList : public IteratorList<T>
{
private:
	Node<T>* head;

	Node<T>* node_at(unsigned int index)
	{
		Node<T>* current = head;

		if (current == nullptr) throw new std::exception("Accessing empty list");

		while (index-- != 0) {
			current = current->next();
			if (current == nullptr) throw new std::exception("Index out of bounds");
		}

		return current;

	}

	Node<T>* tail()
	{
		Node<T>* current = head;

		if (current == nullptr) return current;

		while (current->next() != nullptr) current = current->next();

		return current;
	}

public:
	unsigned int size()  const override
	{
		if (head == nullptr) return 0;

		Node<T>* current = head;
		unsigned int sz = 1;

		while (current->next() != nullptr)
		{
			sz++;
			current = current->next();
		}

		return sz;
	}

	bool is_empty() const override { return head == nullptr; }

	T& at(unsigned int index) override
	{
		if (is_empty()) throw new std::exception("Called on at on empty list");
		
		Node<T>* current = head;
		while (index != 0)
		{
			if (current->next() == nullptr) throw new std::exception("Index out of bounds");
			current = current->next();
			index--;
		}

		return current->get();
	}

	const T& at(unsigned int index) const override
	{
		if (is_empty()) throw new std::exception("Called on at on empty list");

		Node<T>* current = head;
		while (index != 0)
		{
			if (current->next() == nullptr) throw new std::exception("Index out of bounds");
			current = current->next();
			index--;
		}

		return current->get();
	}

	T& operator[](unsigned int index)  override { return this->at(index); }
	const T& operator[](unsigned int index)  const override { return this->at(index); }

	LinkedList<T>& push(const T& data) override
	{
		Node<T>* n = new Node(data);
		if (head == nullptr) head = n;
		else tail()->set_next(n);

		return *this;
	}


	LinkedList<T>& pop() override
	{
		Node<T>* current = head;
		if (current == nullptr) {
			return *this;
		}
		else if (current->next() == nullptr) { //alternatively call remove(size()-1); but this is slower
			remove(0);
		}
		else {
			while (current->next()->next() != nullptr) {
				current = current->next();
			}
			delete current->next();
			current->set_next(nullptr);
		}

		return *this;
	}

	LinkedList<T>& insert(unsigned int index, const T& data) override 
	{
		Node<T>* n = new Node(data);

		if (index == 0) {
			n->set_next(head);
			head = n;
		}
		else {
			Node<T>* current = node_at(index-1);

			n->set_next(current->next());
			current->set_next(n);
		}

		return *this;
	}

	LinkedList<T>& remove(unsigned int index) override
	{
		if (index == 0) {
			if (head == nullptr) throw new std::exception("Removing from empty list");

			Node<T>* temp = head->next();
			delete head;
			head = temp;
		}
		else {
			Node<T>* left = node_at(index - 1);
			Node<T>* middle = left->next();
			if (middle == nullptr) throw new std::exception("Index out of bounds");
			Node<T>* right = middle->next();

			delete middle;
			left->set_next(right);
		}
		return *this;
	}

	LinkedList<T>& swap(unsigned int i1, unsigned int i2) override
	{
		if (i1 == i2) return *this;

		if (i2 < i1) {
			unsigned int temp = i1;
			i1 = i2;
			i2 = temp;
		}

		if (i1 == 0) {
			Node<T>* n1 = head;
			if (n1 == nullptr) throw new std::exception("Swapping on empty list");

			unsigned int diff = i2 - i1;
			Node<T>* n2left = n1;
			for (unsigned int i = 0; i < diff - 1; i++) {
				n2left = n2left->next();
				if (n2left == nullptr) throw new std::exception("Index out of bounds");
			}
			Node<T>* n2 = n2left->next();
			if (n2 == nullptr) throw new std::exception("Index out of bounds");

			head = n2;
			n2left->set_next(n1);

			Node<T>* ntemp = n1->next();
			n1->set_next(n2->next());
			n2->set_next(ntemp);
		}
		else {
			Node<T>* n1left = node_at(i1 - 1);
			if (n1left == nullptr) throw new std::exception("Index out of bounds");

			Node<T>* n1 = n1left->next();
			if (n1 == nullptr) throw new std::exception("Index out of bounds");

			unsigned int diff = i2 - i1;
			Node<T>* n2left = n1;
			for (unsigned int i = 0; i < diff-1; i++) {
				n2left = n2left->next();
				if(n2left == nullptr) throw new std::exception("Index out of bounds");
			}
			Node<T>* n2 = n2left->next();
			if (n2 == nullptr) throw new std::exception("Index out of bounds");

			n1left->set_next(n2);
			n2left->set_next(n1);

			Node<T>* ntemp = n1->next();
			n1->set_next(n2->next());
			n2->set_next(ntemp);
		}

		return *this;
	}

	LinkedList() : head(nullptr) {};

	LinkedList(const LinkedList& list)
	{
		Node<T>* current = list.head;
		while (current != nullptr) {
			push(current->get());
			current = current->next();
		}
	}

	virtual ~LinkedList() override
	{
		clear();
	}

	void clear() override
	{
		while (head != nullptr) {
			remove(0);
		}
	}

	LinkedList(LinkedList<T>&& list)
	{
		head = list.head;
		list.head = nullptr;
	}

	LinkedList<T>& operator=(const LinkedList<T>& list)
	{
		if (head = list.head) return *this;
		clear();
		Node<T>* current = list.head;
		while (current != nullptr) {
			push(current->get());
			current = current->next();
		}

		return *this;
	}

	LinkedList<T>& operator=(LinkedList<T>&& list)
	{
		if (head = list.head) return *this;
		clear();
		head = list.head;
		list.head = nullptr;

		return *this;
	}

	Iterator<T> begin() override
	{
		return Iterator<T>(new LLIterator<T>(head));
	}

	Iterator<T> end() override
	{
		return Iterator<T>(new LLIterator<T>(tail()));
	}
};
