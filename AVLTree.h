#pragma once

#include "BinaryTree.h"

template<typename T, typename keyT>
class AVLNode {
private:
	AVLNode<T, keyT>* l, * r;

	T data;
	keyT key;

	int h_l, h_r;

public:
	AVLNode(const AVLNode<T, keyT>& n) : data(n.data), key(n.key), l(nullptr), r(nullptr), h_l(0), h_r(0) {};
	AVLNode(const T& val, const keyT& k) : data(val), key(k), l(nullptr), r(nullptr), h_l(0), h_r(0) {};
	AVLNode(AVLNode<T, keyT>&& n) : data(n.data), key(n.key), l(nullptr), r(nullptr), h_l(0), h_r(0) {};
	AVLNode(T&& val, const keyT& k) : data(val), key(k), l(nullptr), r(nullptr), h_l(0), h_r(0) {};

	int height_l() { return h_l; }
	int height_r() { return h_r; }

	void set_height_l(int h) { h_l = h; }
	void set_height_r(int h) { h_r = h; }


	void set_left(AVLNode<T, keyT>* ptr)
	{ 
		l = ptr; 
		
		h_l = (l == nullptr ? 0 : l->height());
	}

	void set_right(AVLNode<T, keyT>* ptr)
	{
		r = ptr;
		h_r = (r == nullptr ? 0 : r->height());

	}

	void update_heights()
	{
		h_l = has_left() ? left()->height() : 0;
		h_r = has_right() ? right()->height() : 0;
	}

	int get_balance() { return h_r - h_l; }
	int height() { return std::max(h_l, h_r) + 1; }

	AVLNode<T, keyT>* left() { return l; }
	bool has_left() { return l != nullptr; }
	AVLNode<T, keyT>* right() { return r; }
	bool has_right() { return r != nullptr; }

	T& get() { return data; }
	const T& get() const { return data; }
	keyT& get_key() { return key; }
	const keyT& get_key() const { return key; }

};

template<typename T, typename keyT>
class AVLTree {
private:
	AVLNode<T, keyT>* root;

	void balance(AVLNode<T, keyT>* node, AVLNode<T, keyT>* node_parent)
	{
		if (node == nullptr) return;
		node->update_heights();

		if (node->get_balance() > 1) {
			if (node->right()->get_balance() >= 0) {
				rotate_ccw(node, node_parent);
			} else if(node->right()->get_balance() < 0) {
				rotate_cw(node->left(), node);
				rotate_ccw(node_parent->right(), node_parent);
			}
		}
		else if (node->get_balance() < -1) {
			if (node->left()->get_balance() <= 0) {
				rotate_cw(node, node_parent);
			}
			else if (node->left()->get_balance() > 0) {
				rotate_ccw(node->right(), node);
				rotate_cw(node_parent->left(), node_parent);
			}
		}
	}

	void balance_root()
	{
		root->update_heights();

		if (root->get_balance() > 1) {
			if (root->right()->get_balance() >= 0) {
				rotate_ccw_root();
			}
			else if (root->right()->get_balance() < 0) {
				rotate_cw(root->left(), root);
				rotate_ccw_root();
			}
		}
		else if (root->get_balance() < -1) {
			if (root->left()->get_balance() <= 0) {
				rotate_cw_root();
			}
			else if (root->left()->get_balance() > 0) {
				rotate_ccw(root->right(), root);
				rotate_cw_root();
			}
		}
	}

	// node can not be root. node,node_parent and node->right() must not be nullptr
	void rotate_ccw(AVLNode<T, keyT>* node, AVLNode<T, keyT>* node_parent)
	{
		node_parent->set_right(node->right());
		node->set_right(node->right()->left());
		node_parent->right()->set_left(node);
	}

	// node can not be root. node,node_parent and node->left() must not be nullptr
	void rotate_cw(AVLNode<T, keyT>* node, AVLNode<T, keyT>* node_parent)
	{
		node_parent->set_left(node->left());
		node->set_left(node->left()->right());
		node_parent->left()->set_right(node);
	}

	void rotate_ccw_root()
	{
		AVLNode<T, keyT>* node = root;
		root = node->right();
		node->set_right(root->left());
		root->set_left(node);
	}

	void rotate_cw_root()
	{
		AVLNode<T, keyT>* node = root;
		root = node->left();
		node->set_left(root->right());
		root->set_right(node);
	}

	void insert_down(AVLNode<T, keyT>* n, const T& val, const keyT& key)
	{
		if (key <= n->get_key()) {
			if (n->has_left()) insert_down(n->left(), val, key);
			else n->set_left(new AVLNode<T, keyT>(val, key));

			balance(n->left(), n);
		}
		else {
			if (n->has_right()) insert_down(n->right(), val, key);
			else n->set_right(new AVLNode<T, keyT>(val, key));

			balance(n->right(), n);
		}
	}

	// n == parent->left() or n == parent->right()
	void remove_down(AVLNode<T, keyT>* n, AVLNode<T, keyT>* parent, const keyT& key)
	{
		if (n->get_key() == key) {
			bool l;
			if (parent->has_left() && parent->left()->get_key() == key) {
				n = parent->left();
				l = true;
			}
			else {
				n = parent->right();
				l = false;
			}

			if (!n->has_left() && !n->has_right()) {
				delete_node(n, parent);
			}
			else if (n->has_left()) {
				AVLNode<T, keyT>* cur = n->left(), * prev = n; // prev is parent of cur
				AVLNode<T, keyT>* r = n->right();
				if (!cur->has_right()) {
					replace_up(n, parent, cur);
				}

				while (cur->has_right()) {
					prev = cur;
					cur = cur->right();
				}

				if (cur->has_left()) {
					replace_up(cur, prev, cur->left());
				}
				else if (prev != n) {
					prev->set_right(nullptr);
				}
				else {
					prev->set_left(nullptr);
				}

				l ? parent->set_left(cur) : parent->set_right(cur);

				cur->set_right(r);
			}
			else {
				replace_up(n, parent, n->right());
			}

			delete n;
		}
		else if (key < n->get_key() && n->has_left()) {
			remove_down(n->left(), n, key);
			balance(n->left(), n);
		}
		else if (n->get_key() < key && n->has_right()) {
			remove_down(n->right(), n, key);
			balance(n->right(), n);
		}
		else {
			throw new std::exception("Could not find node to remove");
		}
	}

	void create_down(AVLNode<T, keyT>* target, const AVLNode<T, keyT>* const source)
	{
		if (source->has_left()) {
			target->set_left(new AVLNode<T, keyT>(source->left()));
			create_down(target->left(), source->left());
		}
		if (source->has_right()) {
			target->set_right(new AVLNode<T, keyT>(source->right()));
			create_down(target->right(), source->right());
		}
	}

	void clear_down(AVLNode<T, keyT>* node)
	{
		if (node->has_left()) clear_down(node->left());
		if (node->has_right()) clear_down(node->right());

		delete node;
	}

	void move_down(AVLNode<T, keyT>* target, AVLNode<T, keyT>* source)
	{
		if (source->has_left()) {
			target->set_left(new AVLNode<T, keyT>(std::move(source->left())));
			move_down(target->left(), source->left());
		}
		if (source->has_right()) {
			target->set_right(new AVLNode<T, keyT>(std::move(source->right())));
			move_down(target->right(), source->right());
		}
	}

	void to_vector_down(Vector<T>& v, AVLNode<T, keyT>* n)
	{
		if (n->has_left()) to_vector_down(v, n->left());
		v.push(n->get());
		if (n->has_right()) to_vector_down(v, n->right());
	}

	void replace_up(AVLNode<T, keyT>* target, AVLNode<T, keyT>* target_parent, AVLNode<T, keyT>* target_child)
	{
		bool left = target_parent->left() == target;

		if (left) {
			target_parent->set_left(target_child);
		}
		else {
			target_parent->set_right(target_child);
		}
	}

	void delete_node(AVLNode<T, keyT>* target, AVLNode<T, keyT>* target_parent)
	{
		replace_up(target, target_parent, nullptr);
	}

	AVLNode<T, keyT>* find_down(AVLNode<T, keyT>* node, const keyT& key)
	{
		if (key == node->get_key()) return node;
		else if (key < node->get_key()) return node->has_left() ? find_down(node->left(), key) : nullptr;
		else return node->has_right() ? find_down(node->right(), key) : nullptr;
	}

	AVLNode<T, keyT>* find_node(const keyT& key)
	{
		if (root == nullptr) return nullptr;
		else return find_down(root, key);
	}

	unsigned int size(AVLNode<T, keyT>* node)
	{
		unsigned int sz_l = 0, sz_r = 0;

		if (node->has_left()) sz_l = size(node->left());
		if (node->has_right()) sz_r = size(node->right());

		return 1 + sz_l + sz_r;
	}

public:
	AVLTree() : root(nullptr) {};

	AVLTree(const AVLTree<T, keyT>& bt) {
		if (bt.root != nullptr) {
			root = new AVLNode<T, keyT>(bt.root);
			create_down(root, bt.root);
		}
	}

	AVLTree(AVLTree<T, keyT>&& bt) {
		if (bt.root != nullptr) {
			root = new AVLNode<T, keyT>(std::move(bt.root));
			move_down(root, bt.root);
		}
	}

	AVLTree<T, keyT>& operator= (const AVLTree<T, keyT>& bt)
	{
		clear();
		if (bt.root != nullptr) {
			root = new AVLNode<T, keyT>(bt.root);
			create_down(root, bt.root);
		}
		return *this;
	}

	AVLTree<T, keyT>& operator= (AVLTree<T, keyT>&& bt)
	{
		clear();
		if (bt.root != nullptr) {
			root = new AVLNode<T, keyT>(std::move(bt.root));
			move_down(root, bt.root);
		}
		return *this;
	}

	~AVLTree() { clear(); }

	void clear() {
		clear_down(root);
		root = nullptr;
	}

	unsigned int size()
	{
		if (root == nullptr) return 0;
		else return size(root);
	}

	unsigned int height()
	{
		if (root == nullptr) return 0;
		else return root->height();
	}

	virtual AVLTree<T, keyT>& insert(const T& value, const keyT& key) 
	{
		if (root == nullptr) {
			root = new AVLNode<T, keyT>(value, key);
		}
		else {
			insert_down(root, value, key);
			balance_root();
		}

		return *this;
	}

	virtual AVLTree<T, keyT>& remove(const keyT& key) 
	{
		if (root == nullptr) throw new std::exception("Tree is empty");

		if (root->get_key() == key) {
			if (!root->has_left() && !root->has_right()) {
				delete root;
				root = nullptr;
			}
			else if (root->has_left()) {
				AVLNode<T, keyT>* cur = root->left(), * prev = root; // prev is parent of cur
				AVLNode<T, keyT>* r = root->right();
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
				AVLNode<T, keyT>* temp = root;
				root = root->right();
				delete temp;
			}
			balance_root();
		}
		else {
			remove_down(root, root, key);
			balance_root();
		}

		return *this;
	}

	Vector<T> to_vector()
	{
		Vector<T> v(size());
		if (root != nullptr) to_vector_down(v, root);
		return v;
	}

	bool contains(const keyT& key) { return find(key).has_value(); }

	T& at(const keyT& key)
	{
		AVLNode<T, keyT>* n(find_node(key));
		if (n == nullptr) throw new std::exception("Could not find key");

		return n->get();
	}

	const T& at(const keyT& key) const
	{
		AVLNode<T, keyT>* n(find_node(key));
		if (n == nullptr) throw new std::exception("Could not find key");

		return n->get();
	}

	T& operator[](const keyT& key) { return at(key); }

	const T& operator[](const keyT& key) const { return at(key); }

	std::optional<T&> find(const keyT& key)
	{
		AVLNode<T, keyT>* n(find_node(key));
		if (n == nullptr) return std::nullopt;
		else return std::optional<T&>(n->get());
	}

	void print_tree()
	{
		unsigned int tree_height = root->height();
		Queue<std::pair<AVLNode<T, keyT>*, int>> q;
		q.enqueue(std::make_pair(root, 0));

		unsigned int h = 0, prev_h = 0;

		while (true) {
			auto cur = q.dequeue();
			AVLNode<T, keyT>* n = cur.first;
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
				q.enqueue(std::make_pair(nullptr, h + 1));
			}

		}
	}
};