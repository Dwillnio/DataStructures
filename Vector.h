#pragma once

#include<utility>

#include "List.h"

template<typename T>
class Vector : public List<T> 
{
private:
	T* array;
	unsigned int sz;
	unsigned int allocated;

	const static unsigned int growth_factor = 2;
	const static unsigned int begin_allocation = 1;

	void increase_allocation()
	{
		Vector<T> new_vec(allocated * growth_factor);
		for (unsigned int i = 0; i < sz; i++) {
			new(&new_vec.array[i]) T(array[i]);
		}
		new_vec.sz = sz;
		*this = std::move(new_vec);
	}

	void trim()
	{
		Vector<T> new_vec(sz);
		for (unsigned int i = 0; i < sz; i++) {
			new(&new_vec.array[i]) T(array[i]);
		}
		new_vec.sz = sz;
		*this = std::move(new_vec);
	}

public:
	Vector(unsigned int starting_size = begin_allocation) : sz(0)
	{
		allocated = starting_size;
		if (allocated > 0) {
			void* mem = operator new[](allocated * sizeof(T));
			if (mem == nullptr) throw new std::exception("Failed to allocate memory");
			array = static_cast<T*>(mem);
		}
		else {
			array = nullptr;
		}
	};

	Vector(const Vector<T>& v) 
	{
		sz = v.sz;
		allocated = v.allocated;

		void* mem = operator new[](allocated * sizeof(T));
		if (mem == nullptr) throw new std::exception("Failed to allocate memory");
		array = static_cast<T*>(mem);

		for (unsigned int i = 0; i < sz; i++) {
			new(&array[i]) T(v.array[i]);
		}
	}

	Vector(Vector<T>&& v)
	{
		sz = v.sz;
		allocated = v.allocated;
		array = v.array;

		v.sz = 0;
		v.allocated = 0;
		v.array = 0;
	}

	Vector<T>& operator=(const Vector<T>& v)
	{
		if (this == &v) return *this;

		clear();

		sz = v.sz;
		allocated = v.allocated;

		void* mem = operator new[](allocated * sizeof(T));
		if (mem == nullptr) throw new std::exception("Failed to allocate memory");
		array = static_cast<T*>(mem);

		for (unsigned int i = 0; i < sz; i++) {
			new(&array[i]) T(v.array[i]);
		}

		return *this;
	}

	Vector<T>& operator=(Vector<T>&& v) noexcept
	{
		if (this == &v) return *this;

		clear();

		sz = v.sz;
		allocated = v.allocated;
		array = v.array;

		v.sz = 0;
		v.allocated = 0;
		v.array = nullptr;

		return *this;
	}

	unsigned int size() const override { return sz; }
	bool is_empty() const override { return sz == 0; }

	T& at(unsigned int index) override
	{
		if (index < sz) return array[index];
		else throw new std::exception("Index out of bounds");
	}

	T& operator[] (unsigned int index) { return at(index); }

	const T& at(unsigned int index) const override
	{
		if (index < sz) return array[index];
		else throw new std::exception("Index out of bounds");
	}

	const T& operator[] (unsigned int index) const override { return at(index); }

	Vector<T>& insert(unsigned int index, const T& value) override
	{
		if (index > sz) throw new std::exception("Index out of bounds");

		push(value);
		for (unsigned int i = sz-1; i > index; i--) {
			swap(i - 1, i);	// only good solution if move is cheap for T
		}

		return *this;
	}

	Vector<T>& remove(unsigned int index) override
	{
		if (index >= sz) throw new std::exception("Index out of bounds");

		for (unsigned int i = index; i < sz-1; i++) {
			swap(i, i+1);	// only good solution if move is cheap for T
		}
		pop();

		return *this;
	}

	Vector<T>& swap(unsigned int i1, unsigned int i2) override
	{
		if (i1 >= sz || i2 >= sz) throw new std::exception("Index out of bounds");

		if (i1 == i2) return *this;

		T temp(std::move(array[i1]));
		array[i1] = std::move(array[i2]);
		array[i2] = std::move(temp);

		return *this;
	}

	Vector<T>& push(const T& value) override
	{
		if (sz + 1 > allocated) increase_allocation();
		
		new(&array[sz]) T(value);
		sz++;
		return *this;
	}

	Vector<T>& pop() override
	{
		if (sz >= 1) {
			array[sz - 1].~T();
			sz--;
		}
		return *this;
	}

	void clear() override 
	{
		for (int i = sz - 1; i >= 0; i--) {
			array[i].~T();
		}
		delete[] static_cast<void*>(array);
		sz = 0;
		allocated = 0;
		array = nullptr;
	}

	~Vector() override 
	{
		clear();
	}
};