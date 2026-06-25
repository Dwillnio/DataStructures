#pragma once

#include <string>

#include "LinkedList.h"

template<typename T>
unsigned int hash(const T& value);

template<>
unsigned int hash<int>(const int& value)
{
	return value;
}

template<>
unsigned int hash<float>(const float& value)
{
	std::hash<float> hasher;
	return hasher(value);
}

template<>
unsigned int hash<std::string>(const std::string& value)
{
#ifdef TEST
	if (value == "Tom") return 1;
	if (value == "Alice") return 1;
#endif

	std::hash<std::string> hasher;
	return hasher(value);
}

// hash<keyT> needs to be specialised
template<typename keyT, typename valueT>
class HashTable {
	using K = keyT;
	using V = valueT;

private:
	unsigned int sz;
	unsigned int elements;
	static const unsigned int STANDARD_SIZE = 2 ^ 8;
	static const unsigned int GROWTH_FACTOR = 2;
	static inline const float LOAD_FACTOR_THRESHOLD_MAX = 2.0f;
	static inline const float LOAD_FACTOR_THRESHOLD_MIN = 0.25f;

	LinkedList<std::pair<K,V>>* table;

	unsigned int bucket(const K& key)
	{
		return hash<K>(key) % sz;
	}

	float load_factor() const { return elements / ((float)sz); }

	void resize(unsigned int new_sz) 
	{
		HashTable ht(new_sz);

		for (unsigned int i = 0; i < sz; i++) {
			auto list = table[i];
			auto iter = list.begin();
			while (iter.is_valid()) {
				ht.add(iter.get().first, iter.get().second);
				++iter;
			} 
		}

		operator=(std::move(ht));
	}

public:
	HashTable(unsigned int sz_ = STANDARD_SIZE) : sz(sz_), elements(0), table(new LinkedList<std::pair<K, V>>[sz_]) {};

	HashTable(const HashTable<K, V>& ht) : sz(ht.sz), elements(ht.elements), table(new LinkedList<std::pair<K, V>>[ht.sz])
	{
		for (unsigned int i = 0; i < sz; i++) {
			table[i] = ht.table[i];
		}
	}

	HashTable(HashTable<K, V>&& ht) : sz(ht.sz), table(ht.table), elements(ht.elements)
	{
		ht.sz = 0;
		ht.elements = 0;
		ht.table = nullptr;
	}

	HashTable<K, V>& operator=(const HashTable<K, V>& ht)
	{
		delete[] table;

		sz = ht.sz;
		elements = ht.elements;
		table = new LinkedList<std::pair<K, V>>[ht.sz];
		for (unsigned int i = 0; i < sz; i++) {
			table[i] = ht.table[i];
		}

		return *this;
	}

	HashTable<K, V>& operator=(HashTable<K, V>&& ht)
	{
		delete[] table;

		sz = ht.sz;
		table = ht.table;
		elements = ht.elements;

		ht.sz = 0;
		ht.elements = 0;
		ht.table = nullptr;

		return *this;
	}

	~HashTable()
	{
		delete[] table;
		table = nullptr;
		sz = 0;
		elements = 0;
	}

	HashTable<K, V>& add(const K& key, const V& value)
	{
		table[bucket(key)].push(std::make_pair(key, value));
		elements++;
		if (load_factor() > LOAD_FACTOR_THRESHOLD_MAX) resize(sz * GROWTH_FACTOR);

		return *this;
	}

	HashTable<K, V>& remove(const K& key)
	{
		LinkedList<std::pair<K, V>>& list = table[bucket(key)];

		unsigned int index = 0;
		auto iter = list.begin();
		if (!iter.is_valid()) throw new std::exception("Could not find key");

		while (iter.is_valid()) {
			if (iter.get().first == key) break;
			if (iter.is_end()) throw new std::exception("Could not find key");
			iter.next();
			index++;
		}
		list.remove(index);
		elements--;

		if (load_factor() < LOAD_FACTOR_THRESHOLD_MIN && sz >= GROWTH_FACTOR * STANDARD_SIZE) resize(sz/GROWTH_FACTOR);

		return *this;
	}

	unsigned int stored_elements() { return elements; }

	V& at(const K& key)
	{
		LinkedList<std::pair<K, V>>& list = table[bucket(key)];
		if (list.is_empty()) throw new std::exception("Could not find key");
		

		auto iter = list.begin();
		while (true) {
			if (iter.get().first == key) return iter.get().second;
			if (iter.is_end()) throw new std::exception("Could not find key in bucket");
			iter.next();
		}
	}

	std::optional<V&> find(const K& key)
	{
		LinkedList<std::pair<K, V>>& list = table[bucket(key)];
		if (list.is_empty()) return std::optional();

		auto iter = list.begin();
		while (true) {
			if (iter.get().first == key) return std::make_optional<V&>(iter.get().second);
			if (iter.is_end()) return std::optional();
			iter.next();
		}
	}

	V& operator[] (const K& key) { return at(key); }
};