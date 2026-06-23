#pragma once

#include "DoublyLinkedList.h"

template<typename T>
class Stack : private DoublyLinkedList<T>
{
public:
	Stack() : DoublyLinkedList<T>() {};

	Stack(const Stack<T>& s) : DoublyLinkedList<T>(s) {};

	Stack(Stack<T>&& s) : DoublyLinkedList<T>(s) {};

	Stack<T>& operator=(const Stack<T>& s)
	{
		DoublyLinkedList<T>::operator=(s);
		return *this;
	}

	Stack<T>& operator=(Stack<T>&& s)
	{
		DoublyLinkedList<T>::operator=(s);
		return *this;
	}

	~Stack() = default;

	unsigned int size() const { return DoublyLinkedList<T>::size(); }

	bool is_empty() const { return size() == 0; }

	bool contains(const T& value) const { return DoublyLinkedList<T>::find(value); }

	T& top() { return DoublyLinkedList<T>::at(0); }

	const T& top() const { return DoublyLinkedList<T>::at(0); }

	Stack<T>& push(const T& value) 
	{
		DoublyLinkedList<T>::insert(0,value);

		return *this;
	}

	Stack<T>& pop()
	{
		DoublyLinkedList<T>::remove(0);

		return *this;
	}
};