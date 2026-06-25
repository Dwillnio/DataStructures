#pragma once
#include "List.h"
#include "Iterator.h"

template<typename T>
class DNode
{
private:
	DNode* prev_ptr;
	DNode* next_ptr;

	T data;

public:
	DNode* prev() { return prev_ptr; }
	DNode* next() { return next_ptr; }

	void set_prev(DNode* d) { prev_ptr = d; }
	void set_next(DNode* d) { next_ptr = d; }

	DNode(const T& d) : data(d), prev_ptr(nullptr), next_ptr(nullptr) {};
	DNode() : data(T()), prev_ptr(nullptr), next_ptr(nullptr) {};

	T& get() { return data; }
	const T& get() const { return data; }

	DNode* traverse(int amount)
	{
		DNode* current = this;

		if (amount >= 0) {
			for (int i = 0; i < amount; i++) {
				if (current == nullptr) return current;
				current = current->next_ptr;
			}
		} else {
			for (int i = 0; i < -amount; i++) {
				if (current == nullptr) return current;
				current = current->prev_ptr;
			}
		}

		return current;
	}
};

template<typename T>
class DLLIterator : public IteratorInterface<T> {
private:
	DNode<T>* current;

public:
	DLLIterator() = delete;

	DLLIterator(DNode<T>* cur) : current(cur) {};

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
		DNode<T>* dest = current->traverse(amount);
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
			return current == (static_cast<const DLLIterator<T>&>(iter)).current;
		}
		else {
			return false;
		}
		
	}

	bool operator==(const IteratorInterface<T>& iter) const override { return equals(iter); }
	bool operator!=(const IteratorInterface<T>& iter) const override { return !equals(iter); }

	bool is_beg() const override { return current->prev() == nullptr; }
	bool is_end() const override { return current->next() == nullptr; }
	bool is_valid() const override { return current != nullptr; }

	IteratorInterface<T>* copy() const override
	{
		return new DLLIterator(current);
	}
};

template<typename T>
class DoublyLinkedList : public IteratorList<T>
{
private:
	DNode<T>* head;
	DNode<T>* tail;

	unsigned int sz;

	DNode<T>* node_at(unsigned int index) const
	{
		if (index >= sz) return nullptr;

		return head->traverse(index);
	}

public:
	DoublyLinkedList() : head(nullptr), tail(nullptr), sz(0) {};

	DoublyLinkedList(const DoublyLinkedList& l)
	{
		sz = l.sz;
		if (sz != 0) {
			DNode<T>* iterc = l.head;
			head = new DNode<T>(l.head->get());

			for (unsigned int i = 1; i < l.sz; i++) {
				iterc = iterc->next();
				insert(i, iterc->get());
			}
		} else {
			head = nullptr;
			tail = nullptr;
		}
	}

	DoublyLinkedList& operator= (const DoublyLinkedList& l)
	{
		if (head == l.head) return *this;

		clear();
		if (l.sz != 0) {
			DNode<T>* iterc = l.head;
			head = new DNode<T>(l.head->get());

			for (unsigned int i = 1; i < l.sz; i++) {
				iterc = iterc->next();
				insert(i, iterc->get());
			}
		}
		else {
			head = nullptr;
			tail = nullptr;
		}
		return *this;
	}

	DoublyLinkedList(DoublyLinkedList&& l)
	{
		sz = l.sz;
		head = l.head;
		tail = l.tail;

		l.sz = 0;
		l.head = nullptr;
		l.tail = nullptr;
	}

	DoublyLinkedList& operator=(DoublyLinkedList&& l)
	{
		if (head == l.head) return *this; //can this even happen?
		clear();

		sz = l.sz;
		head = l.head;
		tail = l.tail;

		l.sz = 0;
		l.head = nullptr;
		l.tail = nullptr;

		return *this;
	}

	void clear() override
	{
		while (sz != 0) {
			remove(0);
		}
	}

	~DoublyLinkedList()
	{
		clear();
	}

	unsigned int size() const override { return sz; }

	bool is_empty() const override { return sz == 0; }

	DoublyLinkedList<T>& insert(unsigned int index, const T& value) override
	{
		if (index > sz) throw new std::exception("Index out of bounds");

		DNode<T>* temp = new DNode<T>(value);
		if (index == 0) {
			temp->set_next(head);
			if (sz != 0) {
				head->set_prev(temp);
			}
			else {
				tail = temp;
			}
			head = temp;
		}
		else if (index < sz) {
			DNode<T>* left = node_at(index - 1);
			DNode<T>* right = left->next();

			left->set_next(temp);
			temp->set_prev(left);

			right->set_prev(temp);
			temp->set_next(right);
		}
		else { //index == sz > 0
			tail->set_next(temp);
			temp->set_prev(tail);
			tail = temp;
		}

		sz++;
		return *this;
	}

	DoublyLinkedList<T>& remove(unsigned int index) override
	{
		if (index >= sz) throw new std::exception("Index out of bounds");

		if (index == 0) {
			DNode<T>* temp = head->next();
			delete head;
			head = temp;
			if (temp != nullptr) {
				temp->set_prev(nullptr);
			}
			else {
				tail = nullptr;
			}
		} else {
			DNode<T>* left = node_at(index - 1);
			DNode<T>* middle = left->next();
			DNode<T>* right = middle->next();
			
			delete middle;
			left->set_next(right);
			if (right != nullptr) {
				right->set_prev(left);
			}
			else {
				tail = right;
			}
		}

		sz--;
		return *this;
	}

	DoublyLinkedList<T>& swap(unsigned int i1, unsigned int i2) override
	{
		if (i1 == i2) return *this;

		if(i1 >= sz || i2 >= sz) throw new std::exception("Index out of bounds");

		if (i2 < i1) {
			unsigned int temp = i1;
			i1 = i2;
			i2 = temp;
		}

		if (i1 == 0) {
			DNode<T>* n1 = head;
			if (n1 == nullptr) throw new std::exception("Index out of bounds");
			DNode<T>* n1right = n1->next();
			if (n1right == nullptr) throw new std::exception("Index out of bounds");

			unsigned int diff = i2 - i1;
			DNode<T>* n2left = n1;
			for (unsigned int i = 0; i < diff - 1; i++) {
				n2left = n2left->next();
				if (n2left == nullptr) throw new std::exception("Index out of bounds");
			}
			DNode<T>* n2 = n2left->next();
			if (n2 == nullptr) throw new std::exception("Index out of bounds");
			DNode<T>* n2right = n2->next();

			head = n2;
			n2left->set_next(n1);

			n1right->set_prev(n2);
			if (n2right == nullptr) tail = n1;
			else n2right->set_prev(n1);

			DNode<T>* ntemp_next = n1->next();
			DNode<T>* ntemp_prev = n1->prev();
			n1->set_next(n2->next());
			n1->set_prev(n2->prev());
			n2->set_next(ntemp_next);
			n2->set_prev(ntemp_prev);
		}
		else {
			DNode<T>* n1left = node_at(i1 - 1);
			if (n1left == nullptr) throw new std::exception("Index out of bounds");
			DNode<T>* n1 = n1left->next();
			if (n1 == nullptr) throw new std::exception("Index out of bounds");
			DNode<T>* n1right = n1->next();
			if (n1right == nullptr) throw new std::exception("Index out of bounds");

			unsigned int diff = i2 - i1;
			DNode<T>* n2left = n1;
			for (unsigned int i = 0; i < diff - 1; i++) {
				n2left = n2left->next();
				if (n2left == nullptr) throw new std::exception("Index out of bounds");
			}
			DNode<T>* n2 = n2left->next();
			if (n2 == nullptr) throw new std::exception("Index out of bounds");
			DNode<T>* n2right = n2->next();

			n1left->set_next(n2);
			n2left->set_next(n1);

			n1right->set_prev(n2);
			if (n2right == nullptr) tail = n1;
			else n2right->set_prev(n1);

			DNode<T>* ntemp_next = n1->next();
			DNode<T>* ntemp_prev = n1->prev();
			n1->set_next(n2->next());
			n1->set_prev(n2->prev());
			n2->set_next(ntemp_next);
			n2->set_prev(ntemp_prev);
		}

		return *this;
	}

	DoublyLinkedList<T>& push(const T& t) override
	{
		insert(sz, t);

		return *this;
	}

	DoublyLinkedList<T>& pop() override
	{
		remove(sz - 1);

		return *this;
	}

	T& at(unsigned int index) override
	{
		if (index >= sz) throw new std::exception("Index out of bounds");

		return node_at(index)->get();
	}

	const T& at(unsigned int index) const override
	{
		if (index >= sz) throw new std::exception("Index out of bounds");

		return node_at(index)->get();
	}

	T& operator[] (unsigned int index) override
	{
		return at(index);
	}

	const T& operator[] (unsigned int index) const override
	{
		return at(index);
	}

	Iterator<T> begin() override
	{
		return Iterator<T>(new DLLIterator<T>(head));
	}

	Iterator<T> end() override
	{
		return Iterator<T>(new DLLIterator<T>(tail));
	}
	
};

