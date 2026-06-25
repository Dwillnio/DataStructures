#include <iostream>	

#include "List.h"
#include "LinkedList.h"
#include "DoublyLinkedList.h"
#include "Vector.h"

#include "BasicSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "Heap.h"
#include "HeapSort.h"
#include "Stack.h"
#include "Queue.h"
#include "BinaryTree.h"
#include "AVLTree.h"
#include "Graph.h"
#include "HashTable.h"

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

void list_test2()
{
	//LinkedList<int> list;
	DoublyLinkedList<int> list;
	//Vector<int> list;
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

	//Heap<int> h(list);

	//print_list(h.as_vector());

	QuickSort<int> s;
	s.sort(list);

	Queue<int> st;
	st.enqueue(1).enqueue(3).enqueue(5);

	while (!st.is_empty()) {
		std::cout << st.dequeue() << std::endl;
	}


	//print_list(list);

	//std::cout << QuickSort<int>::find(list, 7) << std::endl;
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

void Search_Tree_test() 
{
	AVLTree<std::string, int> bt;
	bt.insert("A", 5);
	bt.insert("B", 3);
	bt.insert("C", 6);
	bt.insert("D", 4);

	std::cout << "Tree size: " << bt.size() << "\t Tree height: " << bt.height() << std::endl;
	std::cout << "As ordered list: ";
	auto l = bt.to_vector();
	for (unsigned int i = 0; i < l.size(); i++) {
		std::cout << l[i] << " ";
	}
	std::cout << std::endl;

	bt.print_tree();
	std::cout << std::endl;

	bt.remove(3);

	bt.print_tree();
	std::cout << std::endl;

	bt.remove(5);

	bt.print_tree();
	std::cout << std::endl;

	std::cout << bt.at(4) << std::endl << std::endl;

	bt.clear();
	bt.insert("A", 1);
	bt.insert("B", 2);
	bt.insert("C", 4);
	bt.insert("D", 8);
	bt.insert("E", 16);
	bt.print_tree();
	std::cout << std::endl;


	bt.remove(4);
	bt.print_tree();
}

///*
void MST_test()
{
	Graph<std::string, int> g;

	g.add("A");
	g.add("B");
	g.add("C");
	g.add("D");
	g.add("E");
	g.add("F");
	g.add("G");
	g.add("H");

	g.set_edge("A", "B", 4, true);
	g.set_edge("B", "C", 3, true);
	g.set_edge("C", "H", 2, true);
	g.set_edge("A", "D", 3, true);
	g.set_edge("B", "D", 5, true);
	g.set_edge("B", "E", 6, true);
	g.set_edge("C", "E", 4, true);
	g.set_edge("H", "G", 5, true);
	g.set_edge("E", "G", 3, true);
	g.set_edge("D", "F", 4, true);
	g.set_edge("E", "E", 7, true);
	g.set_edge("F", "E", 5, true);
	g.set_edge("F", "G", 7, true);

	g.print();
	std::cout << std::endl;

	auto h1 = g.minimal_spanning_tree();
	auto h2 = g.minimal_spanning_tree_kruskal();
	h1.print();
	h2.print();
	std::cout << h2.is_cyclic();
}
//*/


//	Overwrite hash<std::string> in "HashTable.h" for testing collision
void HashTable_test()
{
	HashTable<std::string, int> age;
	std::string T("Tom"), A("Alice"), J("Justin");

	age.add(T, 21);
	age.add(A, 17);
	age.add(J, 13);

	std::cout << age[T] << std::endl;
	std::cout << age[J] << std::endl;
	std::cout << age[A] << std::endl;

	age.remove(T);

	std::cout << age.stored_elements() << std::endl;

	std::cout << age[A] << std::endl;
	age.add(T, 21);
	age.remove(T);
	std::cout << age[A] << std::endl;

}

int main() {

	HashTable_test();
	//MST_test();
	/*
	Graph<std::string, int> g;

	g.add("A");
	g.add("B");
	g.add("C");
	g.add("D");
	g.add("E");

	g.print();

	g.add_edge("A", "B", true);
	g.add_edge("A", "D", true);
	g.add_edge("B", "C", true);

	g.print();
	
	std::cout << "Cyclic: " << g.is_cyclic() << std::endl;
	std::cout << "Connected: " << g.is_connected() << std::endl;

	g.add_edge("A", "C", true);
	g.add_edge("D", "E", true);

	g.print();

	std::cout << "Cyclic: " << g.is_cyclic() << std::endl;
	std::cout << "Connected: " << g.is_connected() << std::endl;

	std::cout << "Distance from B to D: " << g.dist("B", "D") << std::endl;

	auto h = g.minimal_spanning_tree();
	h.print();
	*/

	return 0;
}