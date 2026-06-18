#pragma once

#include <utility>

#include "SortingAlgorithm.h"
#include "Vector.h"

//algorithm bad (i.e. O(n^2)) unless [] accesss is O(1). Need Iterators for better performance for LinkedList etc.
template<typename T>
class MergeSort : public SortingAlgorithm<T>
{
private:
	//merges two sorted sublists into one combined sorted sublist
	//first list goes from index b1 to b2-1, second from b2 to end
	void merge(List<T>& list, unsigned int b1, unsigned int b2, unsigned int end)
	{
		unsigned int i1 = b1;
		unsigned int i2 = b2;
		unsigned int it = 0;

		Vector<T> temp(end - b1 + 1);
		while (true)
		{
			if (i1 == b2 && i2 == end + 1) break;
			else if (i1 == b2) {
				temp.push(list[i2]);
				i2++;
			}
			else if (i2 == end + 1) {
				temp.push(list[i1]);
				i1++;
			}
			else if (list[i1] <= list[i2]) {
				temp.push(list[i1]);
				i1++;
			}
			else {
				temp.push(list[i2]);
				i2++;
			}
			it++;
		}

		for (unsigned int i = 0; i < temp.size(); i++) {
			list[b1 + i] = std::move(temp[i]);
		}
	}

public:
	List<T>& sort(List<T>& list) override
	{
		unsigned int width = 1;
		unsigned int sz = list.size();
		
		while (width < sz) {
			for (unsigned int i = 0; i < sz - width; i += 2*width) {
				unsigned int b1 = std::min(i, sz - 1); //redundant
				unsigned int b2 = std::min(i+width, sz-1); //redundant
				unsigned int end = std::min(i + 2 * width - 1, sz-1);
				merge(list, b1, b2, end);
			}
			width *= 2;
		}

		return list;
	}
};