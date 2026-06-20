#pragma once

#include "DoublyLinkedList.h"

template<typename T>
class Queue : private DoublyLinkedList<T>
{
public:
	Queue() : DoublyLinkedList<T>() {};

	Queue(const Queue<T>& s) : DoublyLinkedList<T>(s) {};

	Queue(Queue<T>&& s) : DoublyLinkedList<T>(s) {};

	Queue<T>& operator=(const Queue<T>& s)
	{
		DoublyLinkedList<T>::operator=(s);
		return *this;
	}

	Queue<T>& operator=(Queue<T>&& s)
	{
		DoublyLinkedList<T>::operator=(s);
		return *this;
	}

	~Queue() = default;

	unsigned int size() { return DoublyLinkedList<T>::size(); }

	bool is_empty() { return size() == 0; }

	T& peek() { return DoublyLinkedList<T>::at(0); }

	const T& peek() const { return DoublyLinkedList<T>::at(0); }

	Queue<T>& enqueue(const T& value)
	{
		DoublyLinkedList<T>::push(value);

		return *this;
	}

	T dequeue()
	{
		T val = std::move(DoublyLinkedList<T>::at(0));
		DoublyLinkedList<T>::remove(0);

		return val;
	}
};