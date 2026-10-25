#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

class Node {
public:
	std::string name;
	Node(const std::string& n = "") : name(n) {}
};

class Edge {
public:
	int from, to;
	int weight;
	Edge(int f = 0, int t = 0, int w = 1) : from(f), to(t), weight(w) {}
};

class Graph {
public:
	Graph() {}

	template<int N>
	Graph(const int (&matrix)[N][N]) {
		for (int i = 0; i < N; i++)
			addNode(defaultName(i));
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (matrix[i][j] != 0)
					addEdge(i, j, matrix[i][j]);
	}

	void addNode(const std::string& name);
	bool addEdge(int from, int to, int weight = 1);   

	// ----- information -----
	int nodeCount() const;
	int edgeCount() const;
	int componentCount() const;                     
	std::vector<std::string> isolatedNodes() const;  

	// ----- the six questions -----
	bool isMultigraph() const;    
	bool isPseudograph() const;   
	bool isDigraph() const;       
	bool isWeighted() const;      
	bool isComplete() const;     
	bool isDisjointed() const;    

	// ----- printing -----
	void print(std::ostream& out = std::cout) const;            
	void printProperties(std::ostream& out = std::cout) const;  

private:
	std::vector<Node> nodes;
	std::vector<Edge> edges;

	static std::string defaultName(int i);
	static const char* yesNo(bool b) { return b ? "Yes" : "No"; }
	std::vector<std::vector<bool> > adjacency() const;   
};


//  Function bodies

inline std::string Graph::defaultName(int i)
{
	if (i < 26)
		return std::string(1, (char)('A' + i));
	return "N" + std::to_string(i);
}

inline void Graph::addNode(const std::string& name)
{
	nodes.push_back(Node(name));
}

inline bool Graph::addEdge(int from, int to, int weight)
{
	if (from < 0 || from >= nodeCount() || to < 0 || to >= nodeCount())
		return false;
	edges.push_back(Edge(from, to, weight));
	return true;
}

inline int Graph::nodeCount() const { return (int)nodes.size(); }
inline int Graph::edgeCount() const { return (int)edges.size(); }

inline std::vector<std::vector<bool> > Graph::adjacency() const
{
	int n = nodeCount();
	std::vector<std::vector<bool> > adj(n, std::vector<bool>(n, false));
	for (size_t k = 0; k < edges.size(); k++)
		adj[edges[k].from][edges[k].to] = true;
	return adj;
}

// ---- Multigraph ----------------------------------------------------

inline bool Graph::isMultigraph() const
{
	for (size_t a = 0; a < edges.size(); a++)
		for (size_t b = a + 1; b < edges.size(); b++)
			if (edges[a].from == edges[b].from && edges[a].to == edges[b].to)
				return true;
	return false;
}

// ---- Pseudograph ---------------------------------------------------

inline bool Graph::isPseudograph() const
{
	for (size_t k = 0; k < edges.size(); k++)
		if (edges[k].from == edges[k].to)
			return true;
	return false;
}

// ---- Digraph (directed graph) --------------------------------------

inline bool Graph::isDigraph() const
{
	for (size_t a = 0; a < edges.size(); a++) {
		if (edges[a].from == edges[a].to)
			continue;                       // a loop has no direction
		bool hasMirror = false;
		for (size_t b = 0; b < edges.size(); b++) {
			if (edges[b].from == edges[a].to && edges[b].to == edges[a].from
				&& edges[b].weight == edges[a].weight) {
				hasMirror = true;
				break;
			}
		}
		if (!hasMirror)
			return true;
	}
	return false;
}

// ---- Weighted graph ------------------------------------------------

inline bool Graph::isWeighted() const
{
	for (size_t k = 0; k < edges.size(); k++)
		if (edges[k].weight != 1)
			return true;
	return false;
}

// ---- Complete graph ------------------------------------------------

inline bool Graph::isComplete() const
{
	int n = nodeCount();
	std::vector<std::vector<bool> > adj = adjacency();
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j && !adj[i][j])
				return false;
	return true;
}

// ---- Isolated nodes and disjointed graph --------------------------

inline std::vector<std::string> Graph::isolatedNodes() const
{
	std::vector<bool> hasEdge(nodes.size(), false);
	for (size_t k = 0; k < edges.size(); k++) {
		if (edges[k].from != edges[k].to) {       
			hasEdge[edges[k].from] = true;
			hasEdge[edges[k].to] = true;
		}
	}
	std::vector<std::string> result;
	for (size_t i = 0; i < nodes.size(); i++)
		if (!hasEdge[i])
			result.push_back(nodes[i].name);
	return result;
}

inline int Graph::componentCount() const
{
	int n = nodeCount();
	std::vector<std::vector<bool> > adj = adjacency();
	std::vector<bool> visited(n, false);
	int pieces = 0;

	for (int start = 0; start < n; start++) {
		if (visited[start])
			continue;
		pieces++;
		std::vector<int> toVisit(1, start);
		visited[start] = true;
		while (!toVisit.empty()) {
			int current = toVisit.back();
			toVisit.pop_back();
			for (int next = 0; next < n; next++) {
				if (!visited[next] && (adj[current][next] || adj[next][current])) {
					visited[next] = true;
					toVisit.push_back(next);
				}
			}
		}
	}
	return pieces;
}

inline bool Graph::isDisjointed() const
{
	return componentCount() > 1;
}

// ---- Printing ------------------------------------------------------

inline void Graph::print(std::ostream& out) const
{
	out << "Nodes (" << nodeCount() << "):";
	for (size_t i = 0; i < nodes.size(); i++)
		out << " " << nodes[i].name;
	out << "\n";
	out << "Edges (" << edgeCount() << "), shown as  node: neighbour(weight) ...\n";
	for (size_t i = 0; i < nodes.size(); i++) {
		out << "  " << nodes[i].name << ":";
		bool any = false;
		for (size_t k = 0; k < edges.size(); k++) {
			if (edges[k].from == (int)i) {
				out << " " << nodes[edges[k].to].name << "(" << edges[k].weight << ")";
				any = true;
			}
		}
		if (!any)
			out << " (none)";
		out << "\n";
	}
}

inline void Graph::printProperties(std::ostream& out) const
{
	out << std::left;
	out << std::setw(28) << "Is it a multigraph?"      << yesNo(isMultigraph())  << "\n";
	out << std::setw(28) << "Is it a pseudograph?"     << yesNo(isPseudograph()) << "\n";
	out << std::setw(28) << "Is it a digraph?"         << yesNo(isDigraph())     << "\n";
	out << std::setw(28) << "Is it a weighted graph?"  << yesNo(isWeighted())    << "\n";
	out << std::setw(28) << "Is it a complete graph?"  << yesNo(isComplete())    << "\n";
	out << std::setw(28) << "Is it a disjointed graph?" << yesNo(isDisjointed()) << "\n";

	std::vector<std::string> isolated = isolatedNodes();
	out << std::setw(28) << "Isolated nodes:";
	if (isolated.empty()) {
		out << "none";
	}
	else {
		for (size_t i = 0; i < isolated.size(); i++)
			out << (i > 0 ? ", " : "") << isolated[i];
	}
	out << "\n";
}

#endif