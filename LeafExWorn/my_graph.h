#include<list>
#include<iostream>
#include"my_rng.h"
#include"my_vrbls.h"
#ifndef _MY_GRAPH_
#define _MY_GRAPH_
class Edge
{
public:
	Edge(int a, int b)
	{
		this->x = a;
		this->y = b;
	}
	Edge() 
	{
		this->x = 0;
		this->y = 0;
	}
	void Set(int a, int b)
	{
		this->x = a;
		this->y = b;
	}
	int x;
	int y;
};
class Graph
{	
public:	
	Graph() 
	{ 
	}
	~Graph() {}
	void Insert(int a, int b)
	{
		list_graph.push_front(Edge(a,b));
	}
	void Delete(int a,int b)
	{
		for (std::list<Edge>::iterator it = list_graph.begin(); it != list_graph.end(); ++it)
		{
			if ((it->x == a && it->y == b)|| (it->x == b && it->y == a))
			{
				it = list_graph.erase(it);
				break;
			}
		}
	}
	int degree(int n)
	{
		int sum = 0;
		for (std::list<Edge>::iterator it = list_graph.begin(); it != list_graph.end(); ++it)
		{
			if (it->x == n)
			{
				sum++;
			}
			if (it->y == n)
			{
				sum++;
			}
		}
		return sum;
	}
	bool Find_Delete(int a, int b)
	{
		for (std::list<Edge>::iterator it = list_graph.begin(); it != list_graph.end(); ++it)
		{
			if ((it->x == a && it->y == b) || (it->x == b && it->y == a))
			{
				it = list_graph.erase(it);
				return 1;
			}
		}
		return 0;
	}
private:
	std::list<Edge> list_graph;
};


#endif