#include <iostream>
#include <vector>
#include <map>
#include <iterator>

struct Node {
	Node(){}
	int node_num_;
	bool is_warehouse_;
	bool visited_;
	std::vector<Node*> connections_;	
	typedef std::vector<Node*>::iterator Iterator;

	Node(int num, bool is_ware = false, bool visit = false):
            node_num_(num), is_warehouse_(is_ware), visited_(visit){}

};

typedef std::map<int, Node>::iterator Git;
void find_your_ex(std::map<int, Node>& graph)
{
	Git it = graph.begin();
	Git iend = graph.end();	

	for (; it != iend; ++it)
	{
		if (it->second.connections_.size() > 1) // qualifies as a warehouse
		{
			bool is_tmp = false;
			Node::Iterator c_it = it->second.connections_.begin();
			Node::Iterator c_end = it->second.connections_.end();
			
			for (; c_it != c_end; ++c_it)
			{
				if ((*c_it)->is_warehouse_ == true) {
					is_tmp = true;
					break;
				}
			}
			if (!is_tmp) it->second.is_warehouse_ = true;
		}
		else 
		{
			if (it->second.connections_.size() == 0) continue;

			if (!it->second.connections_[0]->is_warehouse_)
			{
				//if (it->second.connections_[0]->connections_[0]->node_num_ == it->second.node_num_)
					it->second.is_warehouse_ = true;
					
			}
		}
	}
}


int main()
{
	int N, M;
	std::cin >> N >> M;

	std::map<int, Node> graph;
	for (int i = 0; i < M; i++)	
	{
		int source, dest;
		std::cin >> source >> dest;
		
		if (graph.find(source) == graph.end())
			graph[source] = Node(source);
		if (graph.find(dest) == graph.end())
			graph[dest] = Node(dest);

		graph[source].connections_.push_back(&graph[dest]);
		graph[dest].connections_.push_back(&graph[source]);
	}

	find_your_ex(graph);

	Git it = graph.begin();
	Git it_end = graph.end();
	int result = 0;

	for (; it != it_end; ++it)
	{
		if (it->second.is_warehouse_) {
			std:: cout << "Node: " << it->second.node_num_ << std::endl;
			result++; 
		}
	}
	for (int i = 0; i < N; i++)
	{
		if (graph.find(i) == graph.end())
			result++;
	}

	std::cout << result << std::endl;

	return 0;
}


/* Problem Statement

Strategic Warehouse placementsMax. Score100
A big international retailer is setting up shop in India and plans to open stores in N towns (3 ≤ N ≤ 1000), denoted by 1, 2, . . . , N. There are direct routes connecting M pairs among these towns. The company decides to build warehouses to ensure that for any town X, there will be a warehouse located either in X or in an immediately neighboring town of X.

Write a program to find the minimum number of warehouses the company has to build.

[Input]:
Input will be given in the following format
N M
S1 E1 (Start Pt and End Pt of a direct route)
S2 E2
S3 E3
....
SM EM
Each route is bidirectional
No combination of towns will be repeated.

[Output]:
Output should be in a file in the following format
Wx - Minimum # of warehouses

Sample Input
10 7
1 2
2 4
2 5
3 6
8 6
9 7
10 7
Sample Output
3
*/
