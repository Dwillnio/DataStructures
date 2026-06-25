#pragma once

#include "Iterator.h"

template<typename T>
class List
{
public:
	virtual unsigned int size() const = 0;
	virtual bool is_empty() const = 0;

	virtual T& operator[] (unsigned int) = 0;
	virtual T& at (unsigned int) = 0;
	virtual const T& operator[] (unsigned int) const = 0;
	virtual const T& at (unsigned int) const = 0;

	virtual List<T>& insert(unsigned int, const T&) = 0;
	virtual List<T>& remove(unsigned int) = 0;
	virtual List<T>& swap(unsigned int, unsigned int) = 0;

	virtual List<T>& push(const T&) = 0;
	virtual List<T>& pop() = 0;

	virtual void clear() = 0;

	virtual ~List() = default;

	virtual int find(const T& value) const
	{
		for (unsigned int i = 0; i < size(); i++) {
			if (at(i) == value) return i;
		}
		return -1;
	}

	virtual bool contains(const T& value) const { return find(value) >= 0; }
};

template<typename T>
class IteratorList : public List<T> {

public:
	virtual Iterator<T> begin() = 0;
	virtual Iterator<T> end() = 0;
};

template<typename T>
void print_list(const List<T>& l)
{
	unsigned int sz = l.size();
	for (unsigned int i = 0; i < sz; i++) {
		std::cout << i << ": " << l[i] << std::endl;
	}
	std::cout << std::endl;
}