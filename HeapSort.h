#pragma once

#include "SortingAlgorithm.h"
#include "Vector.h"

// List needs O(1) element access to be efficient. 
// Alternatively convert to Vector<T> and then convert back to other List
template<typename T>
class HeapSort : public SortingAlgorithm<T>
{
private:

	unsigned int left(unsigned int index)
	{
		return (2 * (index+1)) - 1;
	}
	unsigned int right(unsigned int index)
	{
		return (2 * (index+1));
	}

	// end = last index to be considered + 1
	List<T>& sift_down(List<T>& list, unsigned int end, unsigned int index = 0)
	{
		unsigned int sz = end;
		if (sz <= 1) return list;
		while (true) {
			unsigned int l = left(index);
			unsigned int r = right(index);
			unsigned int swap_index = index;

			if (r < sz) { // left and right child nodes exist
				swap_index = list[l] < list[r] ? r : l;
				swap_index = list[index] < list[swap_index] ? swap_index : index;
				if (index == swap_index) break;
				else {
					list.swap(index, swap_index);
					index = swap_index;
				}
			}
			else if (l < sz) { // only left child node exists
				swap_index = list[index] < list[l] ? l : index;
				if (index == swap_index) break;
				else {
					list.swap(index, swap_index);
					index = swap_index;
				}
			}
			else { // no child node exists
				break;
			}
		}

		return list;
	}

	List<T>& heapify(List<T>& list)
	{
		unsigned int sz = list.size();
		unsigned int beg = sz / 2 + 1;
		if (sz <= 1) return list;

		for (int i = beg-1; i >= 0; i--) {
			sift_down(list, sz, i);
		}

		return list;
	}

public:
	List<T>& sort(List<T>& list)
	{
		unsigned int sz = list.size();
		heapify(list);
		for (int i = sz - 1; i >= 1; i--) {
			list.swap(0, i);
			sift_down(list, i, 0);
		}

		return list;
	}
};