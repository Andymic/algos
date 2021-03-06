#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Edge{
	int vertexIndex;
	Edge * next;
};

struct Vertex{
	string name;
	Edge * adjList;
};

Edge * makeEdge(int v, Edge * nb)
{
	Edge * n = new Edge();
	n->vertexIndex = v;
	n->next = nb;

	return n;
}

Vertex makeVertex(string n, Edge * nb)
{
	Vertex v;
	v.name = n;
	v.adjList = nb;

	return v;
}

class Graph {
private:
	Vertex * adjList;
	int adjListLength;
	bool directed = false;

	int GetVertexIndex(Vertex * vt, string name, int adj_len)
	{
		for(int i=0; i<adj_len; i++)
		{
			if(vt[i].name == name)
				return i;
		}

		return -1;
	}
public:
	Graph(string filePath)
	{
		ifstream infile(filePath.c_str());

		if(infile.good())
		{
			string line;
			string delimiter = " ";

			infile >> line;

			if(line == "directed")
				directed = true;

			infile >> line;

			adjListLength = atoi(line.c_str());
			adjList = new Vertex[adjListLength];

			for(int v = 0; v<adjListLength; v++)
			{
				infile >> line;
				adjList[v] = makeVertex(line, NULL);
			}

			while(getline(infile, line))
			{
				if(line != "")
				{
					string a = line.substr(0, line.find(delimiter));
					string b = line.substr(line.find(delimiter)+1, line.length());

					int ai = GetVertexIndex(adjList, a, adjListLength);
					int bi = GetVertexIndex(adjList, b, adjListLength);

					adjList[ai].adjList = makeEdge(bi, adjList[ai].adjList);

					if(!directed)
					{
						adjList[bi].adjList = makeEdge(ai, adjList[bi].adjList);
					}
					
				}
			}

		}
		else
		{
			cout<<"ERROR file not found\n";
			exit(0);
		}
	}
	void Print()
	{
		for(int i =0; i<adjListLength; i++)
		{
			cout<<adjList[i].name;

			while(adjList[i].adjList != NULL)
			{
				cout<<"-->"<<adjList[adjList[i].adjList->vertexIndex].name;
				adjList[i].adjList = adjList[i].adjList->next;
			}
			cout<<endl;
		}
	}

	bool RouteExist(string a, string b)
	{
		int ar = GetVertexIndex(adjList, a, adjListLength);

		while(adjList[ar].adjList != NULL)
		{
			if(b == adjList[adjList[ar].adjList->vertexIndex].name)
				return true;

			adjList[ar].adjList = adjList[ar].adjList->next;
		}

		if(directed)
		{
			int br = GetVertexIndex(adjList, b, adjListLength);

			while(adjList[br].adjList != NULL)
			{
				if(a == adjList[adjList[br].adjList->vertexIndex].name)
					return true;

				adjList[br].adjList = adjList[br].adjList->next;
			}
		}


		return false;
	}
};
int main(void)
{
	Graph * g = new Graph("friends.txt");
	cout<<g->RouteExist("Me", "You");
	return 0;
}