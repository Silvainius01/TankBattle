#pragma once
#include "../Math/Matrix3.h"
using cml::Vec2;

namespace cml
{
	class Graph
	{
	private:
		struct Edge
		{
			//Used by pathfinding
			int sentFrom;
			int index;
			float weight;

			Edge(void) { index = -1; weight = 0.0f; sentFrom = 0; }
			Edge(int i, float w) : index(i), weight(w) { sentFrom = index; }

			Edge operator=(const Edge &e)
			{
				index = e.index;
				weight = e.weight;
				sentFrom = e.sentFrom;
				return *this;
			}


			Edge operator=(int a) { index = a; return *this; }
			bool operator==(int a) { return index == a; }
			bool operator!=(int a) { return index != a; }
			Edge operator=(float a) { weight = a; return *this; }
			bool operator==(float a) { return weight == a; }
			bool operator>=(float a) { return weight >= a; }

			operator int() const { return index; }
		};
		struct Node
		{
			int totalEdges = 1;
			Edge *edge = nullptr;
			bool open = false;
			bool evaluated = false;
			 
			Vec2 data;

			//Used in A* and Dijkstras.
			Edge path;

			Node() { data = Vec2{ 0, 0 },  open = true; }
			Node(Vec2 DATA) : data(DATA) { open = false; edge = new Edge[totalEdges]; edge[0] = { -1, 0.0f }; }

			//Copy over data from another node
			Node &operator=(Node &n)
			{
				data = n.data;

				return *this;
			}
			//Turn a node into an EXACT replica of another, pathing and overwriteable data included!!
			//NOTE: it will copy over ALL edges with their indicies AND weights. 
			//	That means you can copy over connections that might not exist in a host graph,
			//	and potentially cause connections over long distances with very tiny weights.
			//>>>>>USE AT YOUR OWN RISK<<<<<
			Node operator<=(const Node &n)
			{
				open = n.open;
				evaluated = n.evaluated;
				data = n.data;
				totalEdges = n.totalEdges;
				path = n.path;
				if (n.edge != nullptr)
				{
					delete[] this->edge;
					edge = new Edge[totalEdges];
					for (int a = 0; a < totalEdges; a++) edge[a] = n.edge[a];
				}
				return *this;
			}
			bool operator==(const Node &n) { return data == n.data; }
			bool operator==(const Node *n) { return data == n->data; }

			operator bool() const { return evaluated; }
		};

		int totalNodes = 1;
		int maxQ = 0;

		Node *findNode(Node &n);
		Node *findNode(Node *n);
		int findNodeIndex(Node &n);
		void addNode(Node &n);

		//This function is for use ONLY in dfs, bfs, and pathfinding. It is not for use ANYWHERE outside this class.
		//Pushes everything in q down 1 slot, and opens q[0] for new data; will delete the last index's original data.
		void push(Edge *q, int place = 0);
		//This function is for use ONLY in dfs, bfs, and pathfinding. It is not for use ANYWHERE outside this class.
		//Pulls everything in q up one slot, opening the last index for new data; will delete the first index's original data
		void pull(Edge *q);
		//This function is for use ONLY in pathfinding. It is not for use ANYWHERE outside this class.
		//Will place n's edges into q according to weight. 
		void place(Edge *q, Node *n);
		//Writes result from dijkstra or A* for use by user
		void recordPath(Node *start);
	public:
		Node *nodes = nullptr;
		//This is the result of the most recent dijkstra or A* run, it stores the indicies of all nodes on the path.
		int pathLength = 1;
		int *path = nullptr;

		Graph() { nodes = new Node[totalNodes]; }
		virtual ~Graph() {  }

		Node *findNode(Vec2 data);
		//Find the closest node to a given point, can be given a range if desired.
		// NOTE: I know endIndex says '-1', it defaults to the maximum index in the function.
		Node *findClosestNodeTo(Vec2 data, int startIndex = 0, int endIndex = -1);
		int getTotalNodes() { return totalNodes; }

		void addNode(Vec2 data);
		void removeNode(Node &pNode);
		void connectNodes(Node *nodeA, Node *nodeB, float weight = 0.0f, bool directed = true);
		void draw();
		void makeGrid(int col, int row, float distBetweenNodes, float distMod);
		//Depth First Search
		//Exit codes:
		//	-1: passed start node does not exist
		//	-2: passed target node does not exist
		//	-3: passed start node has no edges
		//	-4: Cannot find the target from the start node.
		int dfs(Node *start, Node *target);
		//Breadth First Search
		int bfs(Node *start, Node *target);

		//-3: One of the passed nodes is null
		//-2: One of the passed nodes does not exist in the graph
		//-1: A path between the passed nodes does not exist
		//+X: The length of the path
		int dijkstra(Node *start, Node *target);
		void aStar(float h);

		//Note: this will make an exact copy of a graph!!
		Graph operator=(const Graph &g)
		{
			for (int a = 0; a < g.totalNodes; a++)
			{
				addNode(Vec2());
				nodes[a] <= g.nodes[a];
			}
			return *this;
		}
	};

}
