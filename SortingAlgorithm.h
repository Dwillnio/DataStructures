#pragma once

#include "List.h"

//type T needs to define <, <=, ==
template<typename T>
class SortingAlgorithm
{
public:
	virtual List<T>& sort(List<T>&) = 0;

	virtual ~SortingAlgorithm() = default; 

	//finds an index, such that sorted_list[index] == value or if no such value exists in the list returns -1;
	static int find(const List<T>& sorted_list, const T& value)
	{
		unsigned int sz = sorted_list.size();
		unsigned int beg = 0, end = sz - 1;
		unsigned int cur;
		while (true) {
			cur = (beg + end) / 2;
			if (end == beg) {
				if (sorted_list[cur] == value) break;
				else return -1;
			}
			else if (end == beg + 1) {
				if (sorted_list[beg] == value) return beg;
				else if (sorted_list[end] == value) return end;
				else return -1;
			}
			else if (sorted_list[cur] < value) {
				beg = cur;
			}
			else if (value < sorted_list[cur]) {
				end = cur;
			}
			else if (sorted_list[cur] == value) {
				break;
			}
		}
		return cur;
	}
};

