#pragma once

#include <optional>
#include <iostream>
#include <climits>
#include <algorithm>
#include <utility>

#include "Vector.h"
#include "Queue.h"

template<typename T, typename keyT>
class TreeNode
{
private:
	TreeNode<T,keyT>* l,* r;

	T data;
	keyT key;
public:
	TreeNode(const TreeNode<T, keyT>& n) : data(n.data), key(n.key), l(nullptr), r(nullptr) {};
	TreeNode(const T& val, const keyT& k) : data(val), key(k), l(nullptr), r(nullptr) {};
	TreeNode(TreeNode<T, keyT>&& n) : data(n.data), key(n.key), l(nullptr), r(nullptr) {};
	TreeNode(T&& val, const keyT& k) : data(val), key(k), l(nullptr), r(nullptr) {};

	TreeNode<T,keyT>* left() { return l; }
	bool has_left() { return l != nullptr; }
	void set_left(TreeNode<T, keyT>* ptr) { l = ptr; }
	TreeNode<T,keyT>* right() { return r; }
	bool has_right() { return r != nullptr; }
	void set_right(TreeNode<T, keyT>* ptr) { r = ptr; }

	T& get() { return data; }
	const T& get() const { return data; }
	keyT& get_key() { return key; }
	const keyT& get_key() const { return key; }
};

// Type keyT needs to define the operators <, <=, ==
// Tree can have duplicate keys. They are pushed to the left of the preceeding equal key
// find and remove only interact with the first added duplicate key
template<typename T, typename keyT>
class BinarySearchTree
{
private:
	TreeNode<T, keyT>* root;

	void clear_down(TreeNode<T, keyT>* node)
	{
		if (node->has_left()) clear_down(node->left());
		if (node->has_right()) clear_down(node->right());

		delete node;
	}

	// copies children of source onto target
	void create_down(TreeNode<T, keyT>* target, const TreeNode<T, keyT>* const source)
	{
		if (source->has_left()) {
			target->set_left(new TreeNode<T, keyT>(source->left()));
			create_down(target->left(), source->left());
		}
		if (source->has_right()) {
			target->set_right(new TreeNode<T, keyT>(source->right()));
			create_down(target->right(), source->right());
		}
	}

	// moves children of source onto target
	void move_down(TreeNode<T, keyT>* target, TreeNode<T, keyT>* source)
	{
		if (source->has_left()) {
			target->set_left(new TreeNode<T>(std::move(source->left())));
			move_down(target->left(), source->left());
		}
		if (source->has_right()) {
			target->set_right(new TreeNode<T>(std::move(source->right())));
			move_down(target->right(), source->right());
		}
	}

	unsigned int size(TreeNode<T, keyT>* node)
	{
		unsigned int sz_l = 0, sz_r = 0;

		if (node->has_left()) sz_l = size(node->left());
		if (node->has_right()) sz_r = size(node->right());

		return 1 + sz_l + sz_r;
	}

	unsigned int height(TreeNode<T, keyT>* node)
	{
		unsigned int h_l = 0, h_r = 0;

		if (node->has_left()) h_l = height(node->left());
		if (node->has_right()) h_r = height(node->right());

		return 1 + std::max(h_l, h_r);
	}

	TreeNode<T, keyT>* find_down(TreeNode<T, keyT>* node, const keyT& key)
	{
		if (key == node->get_key()) return node;
		else if (key < node->get_key()) return node->has_left() ? find_down(node->left(), key) : nullptr;
		else return node->has_right() ? find_down(node->right(), key) : nullptr;
	}

	TreeNode<T, keyT>* find_node(const keyT& key)
	{
		if (root == nullptr) return nullptr;
		else return find_down(root, key);
	}

	int node_height(TreeNode<T, keyT>* node, const keyT& key)
	{
		if (key == node->get_key()) return 1;
		else if (key < node->get_key()) return (node->has_left() ? node_height(node->left(), key)+1 : INT_MIN);
		else return (node->has_right() ?  node_height(node->right(), key)+1 : INT_MIN);
	}

	int node_height(const keyT& key)
	{
		return node_height(root, key);
	}

	TreeNode<T, keyT>* find_parent(TreeNode<T, keyT>* n, const keyT& key)
	{
		if (n->has_left()) {
			if (n->left()->get_key() == key) return n;
			else if (key < n->get_key()) return find_parent(n->left(), key);
		}
		if (n->has_right()) {
			if (n->right()->get_key() == key) return n;
			else if (n->get_key() < key) return find_parent(n->right(), key);
		}
		return nullptr;
	}

	void to_vector_down(Vector<T>& v, TreeNode<T, keyT>* n)
	{
		if (n->has_left()) to_vector_down(v, n->left());
		v.push(n->get());
		if (n->has_right()) to_vector_down(v, n->right());
	}

	void push_down(TreeNode<T, keyT>* n, const T& val, const keyT& key)
	{
		if (key <= n->get_key()) {
			if (n->has_left()) push_down(n->left(), val, key);
			else n->set_left(new TreeNode<T, keyT>(val, key));
		}
		else {
			if (n->has_right()) push_down(n->right(), val, key);
			else n->set_right(new TreeNode<T, keyT>(val, key));
		}
	}

	// intended use is to remove target from tree and replace it with its single child
	void replace_up(TreeNode<T, keyT>* target, TreeNode<T, keyT>* target_parent, TreeNode<T, keyT>* target_child)
	{
		bool left = target_parent->left() == target;

		if (left) {
			target_parent->set_left(target_child);
		}
		else {
			target_parent->set_right(target_child);
		}
	}

	void delete_node(TreeNode<T, keyT>* target, TreeNode<T, keyT>* target_parent)
	{
		replace_up(target, target_parent, nullptr);
	}

public:
	BinarySearchTree() : root(nullptr) {};

	BinarySearchTree(const BinarySearchTree<T, keyT>& bt)
	{
		if (bt.root != nullptr) {
			root = new TreeNode<T, keyT>(bt.root);
			create_down(root, bt.root);
		}
	}

	BinarySearchTree(BinarySearchTree<T, keyT>&& bt)
	{
		if (bt.root != nullptr) {
			root = new TreeNode<T, keyT>(std::move(bt.root));
			move_down(root, bt.root);
		}
	}

	BinarySearchTree<T, keyT>& operator= (const BinarySearchTree<T, keyT>&bt)
	{
		clear();
		if (bt.root != nullptr) {
			root = new TreeNode<T, keyT>(bt.root);
			create_down(root, bt.root);
		}
		return *this;
	}

	BinarySearchTree<T, keyT>& operator= (BinarySearchTree<T, keyT>&& bt)
	{
		clear();
		if (bt.root != nullptr) {
			root = new TreeNode<T, keyT>(std::move(bt.root));
			move_down(root, bt.root);
		}
		return *this;
	}

	~BinarySearchTree() { clear(); }

	void clear() { 
		clear_down(root); 
		root = nullptr;
	}

	unsigned int size() { return root == nullptr ? 0 : size(root); }
	unsigned int height() { return root == nullptr ? 0 : height(root); }

	BinarySearchTree<T, keyT>& push(const T& val, const keyT& key)
	{
		if (root == nullptr) {
			root = new TreeNode<T, keyT>(val, key);
		}
		else{
			push_down(root, val, key);
		}

		return *this;
	}

	BinarySearchTree<T, keyT>& remove(const keyT& key)
	{
		if (root == nullptr) throw new std::exception("Tree is empty");

		if (root->get_key() == key) {
			if (!root->has_left() && !root->has_right()) {
				delete root;
				root = nullptr;
			}
			else if (root->has_left()) {
				TreeNode<T, keyT>* cur = root->left(),* prev = root; // prev is parent of cur
				TreeNode<T, keyT>* r = root->right();
				while (cur->has_right()) {
					prev = cur;
					cur = cur->right();
				}

				if (cur->has_left()) {
					replace_up(cur, prev, cur->left());
				}
				else if (prev != root) {
					prev->set_right(nullptr);
				}
				else {
					prev->set_left(nullptr);
				}

				delete root;
				root = cur;

				cur->set_right(r);
			}
			else {
				TreeNode<T, keyT>* temp = root;
				root = root->right();
				delete temp;
			}

		}
		else {
			TreeNode<T, keyT>* p(find_parent(root, key)),* n;
			if (p == nullptr) throw new std::exception("Could not find key to be removed");

			bool l;
			if (p->has_left() && p->left()->get_key() == key) {
				n = p->left();
				l = true;
			}
			else {
				n = p->right();
				l = false;
			}


			if (!n->has_left() && !n->has_right()) {
				delete_node(n, p);
			}
			else if (n->has_left()) {
				TreeNode<T, keyT>* cur = n->left(),* prev = n; // prev is parent of cur
				TreeNode<T, keyT>* r = n->right();
				if (!cur->has_right()) {
					replace_up(n, p, cur);
				}

				while (cur->has_right()) {
					prev = cur;
					cur = cur->right();
				}

				if (cur->has_left()) {
					replace_up(cur, prev, cur->left());
				}
				else if (prev != n){
					prev->set_right(nullptr);
				}
				else {
					prev->set_left(nullptr);
				}

				if (l) {
					p->set_left(cur);
				}
				else {
					p->set_right(cur);
				}
				cur->set_right(r);
			}
			else {
				replace_up(n, p, n->right());
			}

			delete n;
		}
		

		return *this;
	}

	bool contains(const keyT& key) { return find(key).has_value(); }

	T& at(const keyT& key) 
	{
		TreeNode<T, keyT>* n(find_node(key));
		if (n == nullptr) throw new std::exception("Could not find key");

		return n->get();
	}

	const T& at(const keyT& key) const
	{
		TreeNode<T, keyT>* n(find_node(key));
		if (n == nullptr) throw new std::exception("Could not find key");

		return n->get();
	}

	T& operator[](const keyT& key) { return at(key); }

	const T& operator[](const keyT& key) const { return at(key); }

	std::optional<T&> find(const keyT& key)
	{
		TreeNode<T, keyT>* n(find_node(key));
		if (n == nullptr) return std::nullopt;
		else return std::optional<T&>(n->get());
	}

	Vector<T> to_vector()
	{
		Vector<T> v(size());
		if(root != nullptr) to_vector_down(v, root);
		return v;
	}

	void print_tree()
	{
		unsigned int tree_height = height();
		Queue<std::pair<TreeNode<T, keyT>*, int>> q;
		q.enqueue(std::make_pair(root,0));

		unsigned int h = 0, prev_h = 0;

		while (true) {
			auto cur = q.dequeue();
			TreeNode<T, keyT>* n = cur.first;
			h = cur.second;
			if (h >= tree_height) break;
			if (h != prev_h) std::cout << std::endl;
			prev_h = h;

			if (n != nullptr) {
				std::cout << n->get() << " : " << n->get_key() << "\t";
			}
			else {
				std::cout << "x" << "\t";
			}

			if (n != nullptr && n->has_left()) {
				q.enqueue(std::make_pair(n->left(), h + 1));
			}
			else {
				q.enqueue(std::make_pair(nullptr, h + 1));
			}
			if (n != nullptr && n->has_right()) {
				q.enqueue(std::make_pair(n->right(), h + 1));
			}
			else {
				q.enqueue(std::make_pair(nullptr, h+1));
			} 

		}
	}
};