#pragma once

#include <string>
#include <iostream>

#include "Vector.h"
#include "Stack.h"
#include "Queue.h"

template<typename nodeT, typename weightT>
class Graph
{
	template<typename nodeT, typename weightT>
	friend std::ostream& operator<<(std::ostream&, const Graph<nodeT,weightT>&);

	using T = nodeT;
	using W = weightT;

private:
	Vector<T> nodes;
	Vector<Vector<W>> adjacency_matrix;

	static const int CONNECTED = 0;
	static const int NOEDGE = -1;

	int node_index(const T& node) const { return nodes.find(node); }

	W edge_value_ind(unsigned int i, unsigned int j) const { return adjacency_matrix[i][j]; }

	bool connected_ind(unsigned int i, unsigned int j) const { return edge_value_ind(i,j) >= 0; }

	Vector<unsigned int> traverse_ind(unsigned int start_index) const
	{
		Queue<unsigned int> q;
		Vector<unsigned int> visited(size());

		int current = start_index;
		if (current < 0) throw new std::exception("Could not find start node");

		q.enqueue(current);
		visited.push(current);
		while (!q.is_empty()) {
			current = q.dequeue();
			for (unsigned int i = 0; i < size(); i++) {
				if (connected_ind(current, i) && !visited.contains(i)) {
					q.enqueue(i);
					visited.push(i);
				}
			}
		}

		return visited;
	}

	void traverse_deep_ind(int index, Vector<unsigned int>& visited) const
	{
		visited.push(index);
		for (int i = 0; i < size(); i++) {
			if (connected_ind(index, i) && !visited.contains(i)) traverse_deep_ind(i, visited);
		}
	}

	Vector<unsigned int> traverse_deep_ind(unsigned int start_index) const
	{
		Vector<unsigned int> visited(size());
		traverse_deep_ind(start_index, visited);

		return visited;
	}

	bool is_cyclic(unsigned int index, unsigned int prev, Vector<unsigned int>& visited) const
	{
		visited.push(index);
		for (unsigned int i = 0; i < size(); i++) {
			if (i != prev && connected_ind(index, i)) {
				if (!visited.contains(i)) {
					if(is_cyclic(i, index, visited)) return true;
				}
				else {
					return true;
				}
			}
		}

		return false;
	}

	bool is_cyclic_dir(unsigned int index, Vector<unsigned int>& visited, Stack<unsigned int>& s) const
	{
		s.push(index);
		visited.push(index);

		for (unsigned int i = 0; i < size(); i++) {
			if (connected_ind(index, i)) {
				if (!visited.contains(i)) {
					if (is_cyclic_dir(i, visited, s)) return true;
				}
				else if (s.contains(i)) return true;
			}
		}

		s.pop();
		return false;
	}

public:

	//all default (can be left out)
	Graph() : nodes(), adjacency_matrix() {};
	Graph(const Graph<T, W>& g) : nodes(g.nodes), adjacency_matrix(g.adjacency_matrix) {};
	Graph(Graph<T, W>&& g) : nodes(g.nodes), adjacency_matrix(g.adjacency_matrix) {};
	Graph<T, W>& operator=(const Graph<T, W>& g)
	{
		nodes = g.nodes;
		adjacency_matrix = g.adjacency_matrix;
		return *this;
	}
	Graph<T, W>& operator=(Graph<T, W>&& g)
	{
		nodes = g.nodes;
		adjacency_matrix = g.adjacency_matrix;
		return *this;
	}
	~Graph() {}

	// adjacency_matrix[i][j] == -1 means i not connected to j
	// value of edge i -> j
	W edge_value(const T& node_one, const T& node_two) const { return adjacency_matrix[node_index(node_one)][node_index(node_two)]; } 

	bool connected(const T& node_one, const T& node_two) const { return edge_value(node_one, node_two) >= 0; } 

	void set_edge(const T& node_one, const T& node_two, W value, bool undirected = false)
	{
		adjacency_matrix[node_index(node_one)][node_index(node_two)] = value;
		if (undirected) adjacency_matrix[node_index(node_two)][node_index(node_one)] = value;
	}

	void add_edge(const T& node_one, const T& node_two, bool undirected = false) { set_edge(node_one, node_two, CONNECTED, undirected); }

	void remove_edge(const T& node_one, const T& node_two, bool undirected = false) { set_edge(node_one, node_two, NOEDGE, undirected); }

	void add(const T& node)
	{
		nodes.push(node);
		adjacency_matrix.push(Vector<W>());
		for (unsigned int i = 0; i < adjacency_matrix.size(); i++) {
			adjacency_matrix[adjacency_matrix.size() - 1].push(NOEDGE);
			adjacency_matrix[i].push(NOEDGE);
		}
	}

	void remove(const T& node)
	{
		int index = node_index(node);
		if (index < 0) throw new std::exception("Could not find node");

		for (int i = 0; i < adjacency_matrix.size(); i++) {
			adjacency_matrix[i].remove(index);
		}
		adjacency_matrix.remove(index);
		nodes.remove(index);
	}

	T& at(unsigned int index) { return nodes[index]; }
	const T& at(unsigned int index) const { return nodes[index]; }

	T& operator[](unsigned int index) { return at(index); }
	const T& operator[](unsigned int index) const { return at(index); }

	unsigned int size() const { return nodes.size(); } 

	bool is_empty() const { return size() == 0; }

	bool is_undirected() const 
	{
		for (unsigned int i = 0; i < size(); i++) {
			for (unsigned int j = 0; j < size(); j++) {
				if (adjacency_matrix[i][j] != adjacency_matrix[j][i]) return false;
			}
		}
		return true;
	}

	void print() const { std::cout << *this; }

	const Vector<T> to_vector() const { return nodes; } 

	Vector<T> traverse() const
	{
		if (nodes.size() == 0) return Vector<T>();
		return traverse(nodes[0]);
	}

	// breadth first traversal, which returns vector of visited nodes in the order they were visited
	Vector<T> traverse(const T& start_node) const
	{
		Vector<unsigned int> visited = traverse_ind(node_index(start_node));
		
		Vector<T> r(visited.size());
		for (unsigned int i = 0; i < visited.size(); i++) {
			r.push(nodes[visited[i]]);
		}
		return r;
	}

	Vector<T> traverse_deep(const T& start_node) const
	{
		int start_index = node_index(start_node);
		Vector<unsigned int> visited = traverse_deep_ind(start_index);

		Vector<T> r(visited.size());
		for (unsigned int i = 0; i < visited.size(); i++) {
			r.push(nodes[visited[i]]);
		}
		return r;
	}

	// only works for undirected graphs
	bool is_connected() const { return (is_empty() ? true : (traverse(nodes[0]).size() == size())); }

	bool is_weakly_connected() const
	{
		Graph<T, W> g(*this);
		for (unsigned int i = 0; i < size(); i++) {
			for (unsigned int j = 0; j < size()/2; j++) {
				g.adjacency_matrix[i][j] = std::max(g.adjacency_matrix[i][j], g.adjacency_matrix[j][i]);
				g.adjacency_matrix[j][i] = g.adjacency_matrix[i][j];
			}
		}

		return g.is_connected();
	}

	bool is_strongly_connected() const
	{
		for (unsigned int i = 0; i < size(); i++) {
			if (traverse_deep_ind(i).size() != size()) return false;
		}

		return true;
	}

	// only works for undirected graphs with one connected component
	bool is_cyclic() const
	{
		if (nodes.size() == 0) return false;
		Vector<unsigned int> visited(size());
		return is_cyclic(0, -1, visited);
	}

	// works for directed graphs
	bool is_cyclic_dir() const
	{
		if (nodes.size() == 0) return false;
		Vector<unsigned int> visited(size());
		Stack<unsigned int> s;

		for (unsigned int i = 0; i < size(); i++)
			if (is_cyclic_dir(i, visited, s)) return true;

		return false;
	}

	Graph<T, W> connected_subgraph(const T& start_node) const
	{
		Vector<unsigned int> n = traverse_ind(node_index(start_node));

		Graph<T, W> g(*this);

		for (int i = 0; i < n.size(); i++) {
			g.add(nodes[n[i]]);
			for (int j = 0; j <= i; j++) {
				g.adjacency_matrix[i][j] = adjacency_matrix[n[i]][n[j]];
				g.adjacency_matrix[j][i] = adjacency_matrix[n[j]][n[i]];
			}
		}

		return g;
	}
};

///*
template<typename nodeT, typename weightT>
std::ostream& operator<<(std::ostream& os, const Graph<nodeT, weightT>& g)
{
	os << "\t";
	for (unsigned int i = 0; i < g.size(); i++) {
		os << g.nodes[i] << "\t";
	}
	os << std::endl;

	for (unsigned int i = 0; i < g.adjacency_matrix.size(); i++) {
		os << g.nodes[i] << "\t";
		for (unsigned int j = 0; j < g.adjacency_matrix.size(); j++) {
			os << g.adjacency_matrix[i][j] << "\t";
		}
		os << std::endl;
	}
	os << std::endl;

	return os;
}
//*/