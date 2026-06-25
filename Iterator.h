#pragma once

template<typename T>
class IteratorInterface {

public:
	virtual T& get() = 0;
	virtual const T& get() const = 0;
	virtual T& next() = 0;
	virtual T& prev() = 0;

	virtual bool is_beg() const = 0;
	virtual bool is_end() const = 0;
	virtual bool is_valid() const = 0;

	virtual T& operator++() = 0;
	virtual T& operator+(int) = 0;
	virtual T& operator--() = 0;
	virtual T& operator-(int) = 0;
	virtual T& move(int) = 0;

	virtual bool equals(const IteratorInterface<T>&) const = 0;
	virtual bool operator==(const IteratorInterface<T>&) const = 0;
	virtual bool operator!=(const IteratorInterface<T>&) const = 0;

	virtual IteratorInterface<T>* copy() const = 0;
	//virtual IteratorInterface<T>* beg() const = 0;
	//virtual IteratorInterface<T>* end() const = 0;
};

template<typename T>
class Iterator {
private:
	IteratorInterface<T>* iter;

public:
	Iterator() = delete;

	Iterator(IteratorInterface<T>* i) : iter(i)
	{
		if (iter == nullptr) throw new std::exception("Iterator not properly initialized");
	}

	Iterator(const Iterator<T>& it) : Iterator(it->iter.copy()) {};

	Iterator(Iterator<T>&& it) : iter(it->iter)
	{
		it->iter = nullptr;
	};

	Iterator<T>& operator= (const Iterator<T>&it)
	{
		delete iter;
		iter = it->iter.copy();
	}

	Iterator<T>& operator= (Iterator<T>&& it)
	{
		iter = it->iter;
		it->iter = nullptr;
	}

	~Iterator() { delete iter; }

	T& get() { return iter->get(); }
	const T& get() const { return iter->get(); }

	T& move(int amount) { return iter->move(amount); }
	T& next() { return iter->next(); }
	T& prev() { return iter->prev(); }

	T& operator+(int i) { return iter->operator+(i); }
	T& operator-(int i) { return iter->operator-(i); }
	T& operator++() { return iter->operator++(); }
	T& operator--() { return iter->operator--(); }

	bool equals(const Iterator<T>& it) const { return iter->operator==(it->iter); }
	bool operator==(const Iterator<T>& it) const { return iter->operator==(it->iter); }
	bool operator!=(const Iterator<T>& it) const { return iter->operator!=(it->iter); }

	bool is_beg() const { return iter->is_beg(); }
	bool is_end() const { return iter->is_end(); }
	bool is_valid() const { return iter->is_valid(); }
};