#pragma once
#include <iostream>
using namespace std;

struct ArcNode              //边表节点  
{
	int adjvex;
	ArcNode* next;
};

template <class T>
struct VertexNode           //定义顶点表节点  
{
	T vertex;
	ArcNode* firstedge;
};

template <class T>
class AdjGraph
{
public:
	AdjGraph(T a[], int n);        //构造函数，建立具有n个顶点e条边的图   
	void Insert(int a, int b);       //插入边表  
	void Show(int Num);                        //显示邻接表  
	VertexNode<T> *adjlist = NULL; //存放顶点表的数组  
	int vertexNum;       //图的顶点数和边数  
	int lenth(int i);
	int getk(int i,int k);
	void ShowOne(int i, ofstream &out);
};

/*
*
*
**realise*/
template <class T>
int AdjGraph<T>::getk(int i, int k)
{
	int res = 0;
	ArcNode *first = adjlist[i].firstedge;
	while (k--) {
		res = first->adjvex;
		first = first->next;
	}
	return res;
}

template <class T>
int AdjGraph<T>::lenth(int i)
{
	int res = 0;
	ArcNode *first = adjlist[i].firstedge;
	while (first) {
		res++;
		first = first->next;
	}
	return res;
}

template <class T>
AdjGraph<T>::AdjGraph(T a[], int n)
{
	adjlist = new VertexNode<T>[n + 1];
	vertexNum = n;
	for (int i = 1; i<=vertexNum; i++) //初始化头节点表  
	{
		adjlist[i].vertex = a[i];
		adjlist[i].firstedge = NULL;
	}
}

template <class T>
void AdjGraph<T>::Insert(int vertex, int arcNode)
{
	ArcNode *pArc = new ArcNode();
	pArc->adjvex = arcNode;
	if (adjlist[vertex].firstedge == NULL)
		adjlist[vertex].firstedge = pArc;
	else
	{
		ArcNode *p = adjlist[vertex].firstedge;
		while (p->next != NULL)
			p = p->next;
		p->next = pArc;
	}
}

template <class T>
void AdjGraph<T>::Show(int Num)
{
	cout << "邻接表为：" << endl;
	if (Num > vertexNum) Num = vertexNum;
	for (int i = 1; i <= Num; i++)
	{
		cout << i << ' ' << adjlist[i].vertex;
		ArcNode *p = adjlist[i].firstedge;
		while (p != NULL)
		{
			cout << "->" << p->adjvex;
			p = p->next;
		}
		cout << "->NULL" << endl;
	}
}

template <class T>
void AdjGraph<T>::ShowOne(int i, ofstream &out)
{
	out << adjlist[i].vertex;
	ArcNode *p = adjlist[i].firstedge;
	while (p != NULL)
	{
		out << ' ' << p->adjvex;
		p = p->next;
	}
	out << endl;
}