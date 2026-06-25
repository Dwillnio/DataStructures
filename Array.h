#pragma once

//	T needs to have a constructor without arguments
template<typename T>
class Array {
private:
	T* arr;
	unsigned int sz;

public:
	Array(unsigned int sz_) : sz(sz_), arr(new T[sz_]) {};

	Array(const Array<T>& a) : sz(a.sz), arr(new T[sz_]) 
	{
		for (unsigned int i = 0; i < sz; i++){
			arr[i] = a.arr[i];
		}
	};

	Array(Array<T>&& a) : sz(a.sz), arr(a.arr)
	{
		a.arr = nullptr;
		a.sz = 0;
	};

	Array& operator=(const Array<T>& a) = delete;
	Array& operator=(Array<T>&& a) = delete;

	~Array()
	{
		delete[] arr;
		arr = nullptr;
		sz = 0;
	}

	T& at(unsigned int index)
	{
		if (index >= sz) throw new std::exception("Index out of bounds");

		return arr[index];
	}

	const T& at(unsigned int index) const
	{
		if (index >= sz) throw new std::exception("Index out of bounds");

		return arr[index];
	}

	T& operator[] (unsigned int index) { return at(index); }
	const T& operator[] (unsigned int index) const { return at(index); }

	bool operator== (const Array& a) const
	{
		if (sz != a.sz) return false;

		for (unsigned int i = 0; i < sz; i++)
			if (arr[i] != a.arr[i]) return false;

		return true;
	}

	bool operator!= (const Array& a) const { return !operator==(a); }

	unsigned int size() const { return sz; }
};