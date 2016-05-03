#include "graph.h"
using namespace cml;


cml::Graph::Node *cml::Graph::findNode(Vec2 data )
{
	return findNode(Node{data});
}
cml::Graph::Node *cml::Graph::findNode(Node &n)
{
	for (int a = 0; a < totalNodes; a++)
	{
		if (nodes[a] == Node{ n.data }) { return &nodes[a]; }
	}
}
cml::Graph::Node *cml::Graph::findNode(Node *n)
{
	for (int a = 0; a < totalNodes; a++)
	{
		if (nodes[a] == *n) { return &nodes[a]; }
	}

	return nullptr;
}
cml::Graph::Node *cml::Graph::findClosestNodeTo(Vec2 data, int startIndex, int endIndex)
{
	int closestIndex;
	float save;
	//Protections from out of access crashes.
	//I do not protect the user from nullptr errors on purpose. Inputting wrong index
	//	numbers is a forgivable accident, but if you use this with out adding nodes, they 
	//	deserve a crash.
	if (startIndex < 0 || startIndex >= endIndex || startIndex >= totalNodes) { startIndex = 0; }
	if (endIndex <= startIndex || endIndex > totalNodes) { endIndex = totalNodes; }
	else { endIndex += 1; }

	closestIndex = startIndex;
	save = nodes[startIndex].data.getDistBetween(data);

	for (int a = startIndex; a < endIndex; a++)
	{
		float comp = nodes[a].data.getDistBetween(data);
		if (comp < save) { closestIndex = a; }
	}

	return &nodes[closestIndex];
}
int cml::Graph::findNodeIndex(Graph::Node &n)
{
	for (int a = 0; a < totalNodes; a++)
	{
		if (nodes[a] == n) { return a; }
	}
	return -1;
}
void cml::Graph::addNode(Vec2 data)
{
	addNode(Graph::Node{ data });
}
void cml::Graph::removeNode(Node &pNode)
{
	int deadIndex = findNodeIndex(pNode);
	pNode.open = true;
	delete[] pNode.edge;
	pNode.edge = nullptr;

	for (int a = 0; a < totalNodes; a++)
		if (nodes[a].edge != nullptr)
			for (int b = 0; b < nodes[a].totalEdges; b++)
				if (nodes[a].edge[b] == deadIndex)
					nodes[a].edge[b] = -1;
}
void cml::Graph::connectNodes(Node *nodeA, Node *nodeB, float weight, bool directed)
{
	if (findNode(nodeA) != nullptr && !nodeA->open)
	{
		int openIndex = nodeA->totalEdges;

		if (nodeA->edge == NULL)
		{
			nodeA->edge = new Edge[openIndex];
			nodeA->edge[0] = Edge{-1, 0.0f};
		}
		for (int a = 0; a < openIndex; a++) 
		{ 
			if (nodeA->edge[a].index == -1) 
			{ 
				openIndex = a; 
			} 
		}

		if (openIndex == nodeA->totalEdges) 
		{ 
			Edge *temp = new Edge[nodeA->totalEdges];
			for (int a = 0; a < nodeA->totalEdges; temp[a] = nodeA->edge[a], a++); 
			delete[] nodeA->edge;
			nodeA->totalEdges++;
			nodeA->edge = new Edge[nodeA->totalEdges];
			for (int a = 0; a < openIndex; nodeA->edge[a] = temp[a], a++);
			delete[] temp;
		}
		weight += nodeA->data.getDistBetween(nodeB->data);
		nodeA->edge[openIndex] = { findNodeIndex(*nodeB), weight };
		nodeA->edge[openIndex].sentFrom = openIndex;

		//Updates the total number of possible connections. 
		//This number is used in dfs, bfs, and pathfinding.
		//It's purpose is to make sure that I never have to
		//calculate how big my queue has to be.
		maxQ++;

		if (!directed) { connectNodes(nodeB, nodeA, weight); }
	}
}
void cml::Graph::addNode(Graph::Node &n)
{
	int openIndex = totalNodes;

	for (int a = 0; a < openIndex; a++) { if (nodes[a].open) { openIndex = a; } }

	if (openIndex == totalNodes)
	{
		Node *temp = new Node[totalNodes];
		for (int a = 0; a < totalNodes; a++) { temp[a] <= nodes[a]; }
		delete[] nodes;
		totalNodes++;
		nodes = new Node[totalNodes];
		for (int a = 0; a < openIndex; a++) { nodes[a].open = false; nodes[a] <= temp[a]; }
		delete[] temp;
	}
	nodes[openIndex] = n;
	nodes[openIndex].open = false;
}

void cml::Graph::makeGrid(int col, int row, float distBetweenNodes, float distMod)
{
	for (float a = 0; a < row; a++)
		for (float b = 0; b < col; b++)
			addNode(Vec2{ a * distBetweenNodes, b * distBetweenNodes } +Vec2{ distMod, distMod });

	for (int a = 0; a < totalNodes; a++)
	{
		if (a % col != col - 1)
		{
			connectNodes(&nodes[a], &nodes[a + 1], 0.0f, false);

			if (a + col + 1 < totalNodes)
				connectNodes(&nodes[a], &nodes[a + col + 1], 0.0f, false);
		}

		if (a + col < totalNodes)
			connectNodes(&nodes[a], &nodes[a + col], 0.0f, false);

		if (a % col != 0 && a + col - 1 < totalNodes)
			connectNodes(&nodes[a], &nodes[a + col - 1], 0.0f, false);
	}
}

void cml::Graph::push(Edge *q, int place)
{
	for (int a = maxQ - 1; a > place; a--) { q[a] = q[a - 1]; }
	q[place].index = -1;
}
void cml::Graph::pull(Edge *q)
{
	for (int a = 0; a < totalNodes - 1; a++) { q[a] = q[a + 1]; }
	q[totalNodes - 1].index = -1;
}
void cml::Graph::place(Edge *q,  Node *n)
{
	pull(q);
	if (n->edge == nullptr) { return; }
	for (int a = 0; a < n->totalEdges; n->edge[a].weight += n->path.weight, a++);
	for (int a = 0, place = 0; a < n->totalEdges && place < maxQ;)
	{
		switch (q[place])
		{
		case -1:
			if (n->edge[a] != -1)
			{
				q[place] = n->edge[a];
				q[place].sentFrom = findNodeIndex(*n);
			}
			a++, place = 0;
			continue;
		default:
			if (n->edge[a].weight >= q[place].weight || nodes[n->edge[a]].evaluated) { place++; continue; }
			push(q, place);
		}
	}
	for (int a = 0; a < n->totalEdges; n->edge[a].weight -= n->path.weight, a++);
}
void cml::Graph::recordPath(Node *end)
{
	pathLength = 0;
	int startIndex = findNodeIndex(*end);
	//this will terminate if the path ends
	for (int a = 0, b = startIndex; a < 1 && nodes[b].path.index != -1 && nodes[b].path.index != b; a++)
		a--, pathLength++, b = nodes[b].path.index;

	delete[] path;
	path = new int[pathLength];
	path[0] = startIndex;

	for (int a = 0, b = startIndex, c = pathLength - 1; a < 1 && b < totalNodes && c > -1; a++, c--)
	{
		if (nodes[b].path.index != -1)
		{
			path[c] = nodes[b].path.index;
			a--, b = nodes[b].path.index;
		}
	}
}
int cml::Graph::dfs(Node *start, Node *target)
{
	if (findNode(start) == nullptr) { return -1; }
	if (findNode(target) == nullptr) { return -2; }
	if (start->edge == nullptr) { return -3; }


	start->evaluated = true;
	
	Edge *q = new Edge[maxQ];
	int	place = 1;

	for (int a = 0; a < maxQ; q[a] = -1, a++);
	for (int a = 0; a < totalNodes; nodes[a].evaluated = false, a++);
	for (int a = 0; a < start->totalEdges;  q[a] = start->edge[a], a++);

	while(true)
	{
		if (nodes[q[0]] == target)
		{ 
			delete[] q;
			return place; 
		}
		else if (nodes[q[0]].edge == nullptr)
		{

			if (q[1] != -1) { pull(q); continue; }
			delete[] q;
			return 0; 
		}



		int n = q[0]; q[0] = -1;
		int cap = nodes[n].totalEdges;
		int count = 0;
		nodes[n].evaluated = true;

		for (int a = 0; a < cap; a++)
		{
			q[0] = nodes[n].edge[a];
			if (nodes[q[0]].evaluated) 
			{ 
				count++; 
				pull(q);
				continue; 
			}
			push(q);
		}

		if (count == cap)
		{

			if (q[1] != -1) { pull(q); continue; }
			delete[] q;
			return 0;
		}

		pull(q);
		place++;
	}
}
int cml::Graph::bfs(Node *start, Node *target)
{
	if (findNode(start) == nullptr) { return -1; }
	start = findNode(start);
	if (start->edge[0] == -1) { return 0; }

	start->evaluated = true;
	Edge *q;
	int	place = 1;
	bool found = false;

	q = new Edge[maxQ];

	for (int a = 0; a < totalNodes; a++) { nodes[a].evaluated = false; }
	for (int a = 0; a < maxQ; a++)
	{
		if (a < start->totalEdges) { q[a] = start->edge[a]; }
		else { q[a] = -1; }
	}

	int neg = 0;

	while (true)
	{
		if (q[0].index == -1)
		{
			if (neg == maxQ) { return -2; }
			pull(q);
			neg++;
			continue;
		}
		else if (nodes[q[0]] == target)
		{

			delete[] q;
			return place;
		}
		else if (nodes[q[0]].edge == nullptr)
		{

			if (q[1] != -1) { pull(q); continue; }
			delete[] q;
			return 0;
		}

		neg = 0;
		

		int n = q[0];
		int cap = nodes[n].totalEdges;
		int count = 0;
		nodes[n].evaluated = true;

		for (int a = 0, b = 0; a < maxQ && b < cap;)
		{
			if (nodes[nodes[n].edge[b]].evaluated) { count++, b++; }
			else if (q[a].index == -1)
			{
				q[a] = nodes[n].edge[b];
				a++, b++;
			}
			else { a++; }
		}

		if (count == cap)
		{

			if (q[1] != -1) { pull(q); continue; }
			delete[] q;
			return 0;
		}

		pull(q);
		place++;
	}
}

int cml::Graph::dijkstra(Node *start, Node *target)
{
	if (start == nullptr || target == nullptr) { return -3; }
	if (start->edge == nullptr) { return -1; }
	if (start == nullptr || target == nullptr) { return -2; }
	
	for (int a = 0; a < totalNodes; a++)
	{

		nodes[a].evaluated = false;
		nodes[a].path = Edge();
	}


	start->path = Edge{ findNodeIndex(*start), 0.0f };
	start->evaluated = true;

	Edge *q = new Edge[maxQ];
	int path;
	bool found = true;

	place(q, start);

	while (found)
	{
		if (nodes[q[0]] == target)
		{

			nodes[q[0]].path = Edge{ q[0].sentFrom, q[0].weight };
			path = q[0];
			delete[] q;
			found = false;
			continue;
		}
		else if (nodes[q[0]].edge == nullptr)
		{

			if (q[1] != -1) { pull(q); continue; }
			delete[] q;
			return 1;
		}
		else if (q[0] == -1) { return -7; }
		else if (nodes[q[0]].evaluated && q[0].weight > nodes[q[0]].path.weight)
		{
			pull(q); continue;
		}


		nodes[q[0]].evaluated = true;
		nodes[q[0]].path = Edge{ q[0].sentFrom, q[0].weight };

		place(q, &nodes[q[0]]);
	}

	//for (int next = nodes[path].path.index; next != path;)
	//{
	//	//cout << next << " ";

	//	path = next;
	//	next = nodes[next].path.index;
	//}
	//cout << endl;

	recordPath(target);

	return 0;
}

void cml::Graph::aStar(float h)
{

}