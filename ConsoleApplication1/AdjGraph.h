#pragma once
#include <iostream>
using namespace std;

struct ArcNode              //�߱�ڵ�  
{
	int adjvex;
	ArcNode* next;
};

template <class T>
struct VertexNode           //���嶥���ڵ�  
{
	T vertex;
	ArcNode* firstedge;
};

template <class T>
class AdjGraph
{
public:
	AdjGraph(T a[], int n);        //���캯������������n������e���ߵ�ͼ   
	void Insert(int a, int b);       //����߱�  
	void Show(int Num);                        //��ʾ�ڽӱ�  
	VertexNode<T> *adjlist = NULL; //��Ŷ���������  
	int vertexNum;       //ͼ�Ķ������ͱ���  
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
	for (int i = 1; i<=vertexNum; i++) //��ʼ��ͷ�ڵ��  
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
	cout << "�ڽӱ�Ϊ��" << endl;
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