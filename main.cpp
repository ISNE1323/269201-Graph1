#include <iostream>
#include <string>
#include "graph.h"
using namespace std;

int passed = 0, failed = 0;

void checkGraph(const string& title, const Graph& g,
	bool multigraph, bool pseudograph, bool digraph,
	bool weighted, bool complete, bool disjointed)
{
	bool ok = g.isMultigraph() == multigraph
		&& g.isPseudograph() == pseudograph
		&& g.isDigraph() == digraph
		&& g.isWeighted() == weighted
		&& g.isComplete() == complete
		&& g.isDisjointed() == disjointed;

	if (ok) {
		cout << "  [PASS] " << title << endl;
		passed++;
	}
	else {
		cout << "  [FAIL] " << title << endl;
		cout << "         got      multi=" << g.isMultigraph() << " pseudo=" << g.isPseudograph()
			<< " di=" << g.isDigraph() << " weighted=" << g.isWeighted()
			<< " complete=" << g.isComplete() << " disjointed=" << g.isDisjointed() << endl;
		cout << "         expected multi=" << multigraph << " pseudo=" << pseudograph
			<< " di=" << digraph << " weighted=" << weighted
			<< " complete=" << complete << " disjointed=" << disjointed << endl;
		failed++;
	}
}

int main()
{
	int sample[9][9] = {
		//  A  B  C  D  E  F  G  H  I
		{   0, 2, 1, 7, 0, 0, 0, 0, 0 },   // A
		{   2, 0, 5, 5, 0, 0, 0, 0, 0 },   // B
		{   1, 5, 0, 4, 0, 9, 0, 0, 0 },   // C
		{   7, 5, 4, 0, 8, 0, 0, 0, 0 },   // D
		{   0, 0, 0, 8, 0, 3, 7, 0, 0 },   // E
		{   0, 0, 9, 0, 3, 0, 5, 10, 0 },  // F
		{   0, 0, 0, 0, 7, 5, 0, 11, 6 },  // G
		{   0, 0, 0, 0, 0, 10, 11, 0, 3 }, // H
		{   0, 0, 0, 0, 0, 0, 6, 3, 0 }    // I
	};

	Graph g(sample);

	cout << "===== Sample graph from the lab =====" << endl;
	g.print();
	cout << endl;
	g.printProperties();

	cout << endl << "===== Self-check with other graphs =====" << endl;

	checkGraph("sample A-I matrix", g, false, false, false, true, false, false);

	int triangle[3][3] = { {0,1,1}, {1,0,1}, {1,1,0} };
	checkGraph("triangle (complete, not weighted)", Graph(triangle),
		false, false, false, false, true, false);

	int cycle[3][3] = { {0,1,0}, {0,0,1}, {1,0,0} };
	checkGraph("one-way cycle A->B->C->A (digraph)", Graph(cycle),
		false, false, true, false, false, false);

	int differentWeights[2][2] = { {0,2}, {3,0} };
	checkGraph("A->B = 2 but B->A = 3 (digraph, weighted, complete)", Graph(differentWeights),
		false, false, true, true, true, false);

	int withLoop[2][2] = { {1,1}, {1,0} };
	checkGraph("loop on A (pseudograph)", Graph(withLoop),
		false, true, false, false, true, false);

	int isolated[4][4] = { {0,1,0,0}, {1,0,1,0}, {0,1,0,0}, {0,0,0,0} };
	Graph withIsolated(isolated);
	checkGraph("D has no edges (isolated node, disjointed)", withIsolated,
		false, false, false, false, false, true);
	cout << "         isolated nodes found: ";
	vector<string> names = withIsolated.isolatedNodes();
	for (size_t i = 0; i < names.size(); i++)
		cout << names[i] << " ";
	cout << endl;

	int twoPieces[4][4] = { {0,1,0,0}, {1,0,0,0}, {0,0,0,1}, {0,0,1,0} };
	checkGraph("A-B and C-D, two separate pieces (disjointed)", Graph(twoPieces),
		false, false, false, false, false, true);

	int allFives[2][2] = { {0,5}, {5,0} };
	checkGraph("all weights are 5 (weighted)", Graph(allFives),
		false, false, false, true, true, false);

	Graph multi(triangle);
	multi.addEdge(0, 1, 1);
	checkGraph("triangle plus a second A->B edge (multigraph)", multi,
		true, false, false, false, true, false);

	cout << endl << "Passed: " << passed << "   Failed: " << failed << endl;
	if (failed == 0)
		cout << "All checks passed!" << endl;

	return failed == 0 ? 0 : 1;
}