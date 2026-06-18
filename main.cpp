#include <iostream>	

#include "List.h"
#include "LinkedList.h"
#include "DoublyLinkedList.h"
#include "Vector.h"

#include "BasicSort.h"
#include "MergeSort.h"
#include "QuickSort.h"

#define MOVE_ENABLED


static void list_test1(List<int>& list)
{
	list.push(3);
	list.push(1);
	list.push(4);

	list.swap(0, 1);
	list.swap(0, 2);

	print_list(list);
	std::cout << std::endl;

	list.insert(0, 2);

	print_list(list);
	std::cout << std::endl;

	list.insert(4, 5);

	print_list(list);
	std::cout << std::endl;

	list.remove(0);

	print_list(list);
	std::cout << std::endl;

	list.remove(3);

	print_list(list);
	std::cout << std::endl;

	list.remove(1);

	print_list(list);
	std::cout << std::endl;
}

class INT_Array
{
	int* data;
	unsigned int sz;

public:
	INT_Array()
	{
		data = nullptr;
		sz = 0;
		std::cout << "Constructor" << std::endl;
	}

	INT_Array(unsigned int i)
	{
		if (i > 0) {
			sz = i;
			data = new int[sz];
			std::cout << "Constructor with size" << std::endl;
		}
		else {
			data = nullptr;
			sz = 0;
			std::cout << "Constructor" << std::endl;
		}
	}

	INT_Array(const INT_Array& t)
	{
		sz = t.sz;
		data = nullptr;
		if (sz != 0) {
			data = new int[sz];
			for (unsigned int i = 0; i < sz; i++) {
				data[i] = t.data[i];
			}
		}
		std::cout << "Copy Initialization" << std::endl;
	}

#ifdef MOVE_ENABLED
	INT_Array(INT_Array&& t) noexcept
	{
		sz = t.sz;
		data = t.data;

		t.data = nullptr;
		t.sz = 0;

		std::cout << "Move Initialization" << std::endl;
	}
#endif

	INT_Array& operator= (const INT_Array& t) 
	{
		delete[] data;
		sz = t.sz;
		if (sz != 0) {
			data = new int[sz];
			for (unsigned int i = 0; i < sz; i++) {
				data[i] = t.data[i];
			}
		}
		std::cout << "Copy Assignment Initialization" << std::endl;
		return *this;
	}

#ifdef MOVE_ENABLED
	INT_Array& operator= (INT_Array&& t) noexcept
	{
		delete[] data;
		sz = t.sz;
		data = t.data;

		t.data = nullptr;
		t.sz = 0;

		std::cout << "Move Assignment" << std::endl;
		return *this;
	}
#endif

	unsigned int size() const { return sz; }

	~INT_Array()
	{
		delete[] data;
		sz = 0;

		std::cout << "Destructor" << std::endl;
	}

	int& operator[] (unsigned int index)
	{
		if (index < sz) return data[index];
		else throw new std::exception("Index out of bounds");
	}
};

std::ostream& operator<< (std::ostream& s, INT_Array& a)
{
	if (a.size() > 0) {
		for (unsigned int i = 0; i < a.size(); i++) {
			s << i << ": " << a[i] << std::endl;
		}
	}
	return s;
}

INT_Array test_func(INT_Array a)
{
	std::cout << "Test" << std::endl;
	return a;
}

void INT_Array_test()
{
	INT_Array l1;
	INT_Array l2(5);
	l2[2] = 3;
	std::cout << l2;
	l1 = l2;
	INT_Array l3 = l2;
	INT_Array l4 = INT_Array(3);
	INT_Array l5 = l2;

	std::cout << l5;
	std::cout << std::endl;

	INT_Array l6 = test_func(l2);
	std::cout << std::endl;

	INT_Array l7(4);
	l7 = test_func(l2);
	std::cout << std::endl;

}

int main() {
	//LinkedList<int> list;
	//DoublyLinkedList<int> list;
	Vector<int> list;
	//list_test1(list);
	list.push(4);
	list.push(6);
	list.push(2);
	list.push(1);
	list.push(3);
	list.push(8);
	list.push(0);

	print_list(list);
	std::cout << std::endl;
	
	QuickSort<int> s;
	s.sort(list);

	print_list(list);

	std::cout << QuickSort<int>::find(list, 7) << std::endl;

	return 0;
}