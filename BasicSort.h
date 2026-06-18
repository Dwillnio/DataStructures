#pragma once

#include "SortingAlgorithm.h"

template<typename T>
class BasicSort : public SortingAlgorithm<T>
{

public:
	List<T>& sort(List<T>& list) override
	{
		unsigned int sz = list.size();
		bool swapped = true;
		while (swapped) {
			swapped = false;
			for (unsigned int i = 0; i < sz - 1; i++) {
				if (list[i + 1] < list[i]) {
					list.swap(i, i + 1);
					swapped = true;
				}
			}
		}
		return list;
	}
};