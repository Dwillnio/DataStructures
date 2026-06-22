#pragma once

#include <string>
#include <iostream>

#include "Vector.h"

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

	void set_edge(const T& node_one, const T& node_two, W value, bool symmetric = false)
	{
		adjacency_matrix[node_index(node_one)][node_index(node_two)] = value;
		if (symmetric) adjacency_matrix[node_index(node_two)][node_index(node_one)] = value;
	}

	void add_edge(const T& node_one, const T& node_two, bool symmetric = false) { set_edge(node_one, node_two, CONNECTED, symmetric); }

	void remove_edge(const T& node_one, const T& node_two, bool symmetric = false) { set_edge(node_one, node_two, NOEDGE, symmetric); }

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

	int size() const { return nodes.size(); } 

	void print() const { std::cout << *this; }

	const Vector<T> to_vector() const { return nodes; } 
};

///*
template<typename nodeT, typename weightT>
std::ostream& operator<<(std::ostream& os, const Graph<nodeT, weightT>& g)
{
	os << "\t";
	for (int i = 0; i < g.size(); i++) {
		os << g.nodes[i] << "\t";
	}
	os << std::endl;

	for (int i = 0; i < g.adjacency_matrix.size(); i++) {
		os << g.nodes[i] << "\t";
		for (int j = 0; j < g.adjacency_matrix.size(); j++) {
			os << g.adjacency_matrix[i][j] << "\t";
		}
		os << std::endl;
	}
	os << std::endl;

	return os;
}
//*/