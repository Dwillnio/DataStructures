#pragma once

#include "Vector.h"

// T needs to define the operators <, <= and ==
template<typename T>
class Heap : private Vector<T>
{
private:
	int parent(unsigned int index)
	{
		return (index + 1) / 2 - 1;
	}

	unsigned int left(unsigned int index)
	{
		return (2 * (index + 1)) - 1;
	}
	unsigned int right(unsigned int index)
	{
		return (2 * (index + 1));
	}

	Heap<T>& sift_down(unsigned int end, unsigned int index = 0)
	{
		unsigned int sz = end;
		if (sz <= 1) return *this;
		while (true) {
			unsigned int l = left(index);
			unsigned int r = right(index);
			unsigned int swap_index = index;

			if (r < sz) { // left and right child nodes exist
				swap_index = Vector<T>::at(l) < Vector<T>::at(r) ? r : l;
				swap_index = Vector<T>::at(index) < Vector<T>::at(swap_index) ? swap_index : index;
				if (index == swap_index) break;
				else {
					Vector<T>::swap(index, swap_index);
					index = swap_index;
				}
			}
			else if (l < sz) { // only left child node exists
				swap_index = Vector<T>::at(index) < Vector<T>::at(l) ? l : index;
				if (index == swap_index) break;
				else {
					Vector<T>::swap(index, swap_index);
					index = swap_index;
				}
			}
			else { // no child node exists
				break;
			}
		}

		return *this;
	}

	Heap<T>& heapify()
	{
		unsigned int beg = size() / 2 + 1;
		if (size() <= 1) return *this;

		for (int i = beg - 1; i >= 0; i--) {
			sift_down(size(), i);
		}

		return *this;
	}

	Heap<T>& sift_up(unsigned int index)
	{
		while (index > 0) {
			if (Vector<T>::at(index) <= Vector<T>::at(parent(index))) return *this;
			else {
				Vector<T>::swap(index, parent(index));
				index = parent(index);
			}
		}
		return *this;
	}

public:
	Heap() : Vector() {};

	Heap(const Heap<T>& h) : Vector<T>(h) {}

	Heap(const Vector<T>& vec) : Vector<T>(vec) 
	{
		heapify();
	}

	Heap(Heap<T>&& h) : Vector<T>(h) {}

	Heap<T>& operator=(const Heap<T>& h)
	{
		this->Vector<T>::operator=(h);
	}

	Heap<T>& operator=(const Vector<T>& vec)
	{
		this->Vector<T>::operator=(vec);
		heapify();
	}

	~Heap() {}

	Heap<T>& operator=(Heap<T>&& h)
	{
		Vector<T>::operator=(h);
	}

	Heap<T>& operator=(Vector<T>&& vec)
	{
		Vector<T>::operator=(vec);
		heapify();
	}

	Heap<T>& push(const T& value)
	{
		Vector<T>::push(value);
		sift_up(size() - 1);

		return *this;
	}

	Heap<T>& pop()
	{
		Vector<T>::swap(0,size()-1);
		Vector<T>::pop();
		sift_down(size(),0);

		return *this;
	}

	const T& max() { return Vector<T>::at(0); }

	Vector<T> as_vector() { return Vector<T>(*this); }

	unsigned int size() { return Vector<T>::size(); }

	bool is_empty() { return Vector<T>::is_empty(); }
};