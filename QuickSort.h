#pragma once

#include "List.h"
#include "SortingAlgorithm.h"
#include "Vector.h"

//algorithm bad (i.e. O(n^2)) unless [] accesss is O(1). Need Iterators for better performance for LinkedList etc.
template<typename T> 
class QuickSort : public SortingAlgorithm<T>
{
private:
	unsigned int pivot_sort(List<T>& list, unsigned int pivot, unsigned int begin, unsigned int end)
	{
		unsigned int count_less = 0, count_more = 0;

		unsigned int sz = end - begin + 1;
		T pivot_value = list[pivot];
		for (unsigned int i = begin; i <= end; i++) {
			if (list[i] < pivot_value) count_less++;
			else if (pivot_value < list[i]) count_more++;
		}

		list.swap(pivot, begin + count_less);
		pivot = begin + count_less;

		unsigned int upper = end;
		unsigned int index = begin;
		unsigned int eq_index = pivot + 1;
		while(index < pivot){
			if (list[index] > pivot_value) {
				list.swap(index, upper);
				upper--;
			}
			else if (list[index] == pivot_value) {
				list.swap(index, eq_index);
				eq_index++;
			}
			else { //list[index] < pivot_value
				index++;
			}
		}

		return pivot;
	}

	List<T>& sort(List<T>& list, unsigned int pivot, unsigned int begin, unsigned int end)
	{
		pivot = pivot_sort(list, pivot, begin, end);

		if (pivot > begin + 1) {
			sort(list, (begin + pivot - 1) / 2, begin, pivot - 1);
		}
		if (pivot + 1  < end ) {
			sort(list, (pivot + 1 + end) / 2, pivot + 1, end);
		}
		return list;
	}

public:
	List<T>& sort(List<T>& list) override
	{
		unsigned int sz = list.size();
		return sort(list, sz / 2, 0, sz - 1);
	}
};