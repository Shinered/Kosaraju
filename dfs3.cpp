#include <iostream>
#include <ctime>
#include <stack>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
/**output:
434821
968
459
313
211
running time:8.44267s */
/********边表***********/
class EdgeNode
{
public:
	long int adjvex; //边表数据
	EdgeNode *next; //边表指向的下一个节点
	EdgeNode(long int adj, EdgeNode *n = NULL): adjvex(adj), next(n){}
	
};
/*********顶点表**********/
class VertexNode
{
public:
	long int data;	//当前顶点数据
	EdgeNode *firstEdge; //顶点第一条边
//	VertexNode(long int d, EdgeNode *fir = NULL) : data(d), firstedge(fir) {}
};
/************图**************/
class Graph
{
public:
	long int numVertexes = 875714;

	long int numEdges;
	VertexNode* adjList = new VertexNode[875714];
public:
/*************头插法*************************/
	void addEdge(long int a, long int b)
	{
		EdgeNode *enode = new EdgeNode(b,NULL);
	    enode->next = (adjList+a)->firstEdge;
		(adjList+a)->firstEdge = enode; 
	}

/*===================初始化读入数据=============*/
	void initGraph()
	{
		long int i = 0;
		for(long int i = 0; i < numVertexes; i++)	
		{	
			(adjList+i)->data = i;
			(adjList+i)->firstEdge = NULL;
		}
		ifstream fin("SCC.txt");
		string line;
		stringstream stream;
		while(getline(fin, line))
		{
			long int vertex, adjacent;
			stream.clear();
			stream << line;
			stream >> vertex;
			while(stream >> adjacent)
			{
				addEdge(adjacent-1, vertex-1);
			}
		}
	}
/*===============逆向图==================*/
	void getInversed()
	{
		long int i = 0;
		for(long int i = 0; i < numVertexes; i++)	
		{	
//			adjList[i] = new VertexNode;
			(adjList+i)->data = i;
			(adjList+i)->firstEdge = NULL;
		}
		ifstream fin("SCC.txt");
		string line;
		stringstream stream;
		while(getline(fin, line))
		{
			long int vertex, adjacent;
			stream.clear();
			stream << line;
			stream >> vertex;
			while(stream >> adjacent)
			{
				addEdge(vertex-1, adjacent-1);
			}
		}
	}
/*==================打印==================*/	
	void print()
	{
		for(int i = 0; i < numVertexes; i++)
		{
			cout << (adjList+i)->data << " ";
			EdgeNode *temp = new EdgeNode(0,NULL);
			temp = (adjList+i)->firstEdge;
			while(temp)
			{
				cout << temp->adjvex << " ";
				temp = temp->next;
			}
			cout << endl;
		}
	}
};
/**************深度优先搜索****************/
class DepthFirstSearch
{
public:
	void dfsInit(Graph graph)
	{
		marked.resize(length);
		leader.resize(length);
	}

	void markedInit()
	{
		time = 0;
		for(long int i = 0; i < length; i++)
		{
			marked[i] = false;
		}
	}
/*================第一次dfs================*/
	void dfsLoop1(Graph graph)
	{
		markedInit();
		time = 0; 
		for(long int i = length-1; i >= 0; i--)
		{
			if(!marked[i])
			{
				DFS1(graph, i);
			}
		}
	}
/*================第二次dfs================*/
	void dfsLoop2(Graph graph)
	{
		markedInit();
		time = 0;
		for(long int i = length-1; i >=0 ; i--)
		{
			if(!marked[ff[i]])
			{
				s = ff[i];
				DFS2(graph, ff[i]);
			}
		}
	}

	void DFS1(Graph graph, long int i)
	{
		stack< VertexNode* > stack;
		stack.push(graph.adjList+i);
		marked[i] = true;
		while(!stack.empty())
		{
			VertexNode *temp = new VertexNode;
			temp = stack.top();
			EdgeNode *p = new EdgeNode(0, NULL);
			p = temp->firstEdge;
			int flag = 0;
			while(p)
			{
				if(!marked[p->adjvex])
				{
					marked[p->adjvex] = true;
					stack.push(graph.adjList + (p->adjvex));		
					flag = 1;
					break;
				}
				p = p->next;
			}
			if(!flag)
			{
				ff[time] = temp->data;
				time++;
				stack.pop();
			}
		}
	}
	void DFS2(Graph graph, long int i)
	{
		stack < VertexNode* > stack;
		stack.push(graph.adjList+i);
		leader[i] = s;
		marked[i] = true;
		while(!stack.empty())
		{
			VertexNode *temp = new VertexNode;
			temp = stack.top();
			marked[temp->data] = true;
			EdgeNode *p = new EdgeNode(0,NULL);
			p = temp->firstEdge;
			int flag = 0;
			while(p)
			{
				if(!marked[p->adjvex])
				{
					marked[p->adjvex] = true;
					stack.push(graph.adjList + p->adjvex);
					flag = 1;
					break;
				}
				p = p->next;
			}
			if(!flag)
			{
				leader[temp->data] = s;
				stack.pop();
			}


		}
	}
	
	long int max(vector<long int> &_sto)
	{
		long int max = 0;
		long int index;
		int flag = 0;
		for(long int i = 0; i < length; i++)
		{
			if(max < _sto[i])
			{
				max = _sto[i];
				index = i; 
				flag = 1;
			}
		}
		
		if(flag)
		{
			_sto[index] = 0;
		}

		return max;
	}
/*===========判断最大5个分量==========*/
	void fiveMax()
	{
		vector<long int> _sto;
		_sto.resize(length);
		for(long int i = 0; i < length; i++)
		{
			_sto[leader[i]] ++;
		}
		for(int i = 0; i < 5; i++)
		{
			cout << max(_sto) << endl;
		}

	}


private:
	long int time; //访问时间
	long int s;
	long int ff[875714];
	long int length = 875714;
	vector<long int> leader;
	vector<bool> marked; //标记是否访问过
};


int main()
{
	clock_t start, end;
	start = clock();
	Graph graph;
	DepthFirstSearch depth;
	graph.initGraph();
//	graph.print();
	depth.dfsInit(graph);
	depth.dfsLoop1(graph);
	graph.getInversed();
	depth.dfsLoop2(graph);
	depth.fiveMax();
	end = clock();
//	graph.print();
	cout << "running time:" << (double)(end-start)/CLOCKS_PER_SEC  << "s" << endl;
	return 0;
	
}


