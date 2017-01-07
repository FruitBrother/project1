#include <iostream>  
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "AdjGraph.h"
#define NodeSize 82670
using namespace std;
bool isRepeat(int* t, int end, int ele) {
	bool res = false;
	for (int i = 0; i < end; i++) {
		if (t[i] == ele) {
			res = true;
			break;
		}
	}
	return res;
}

int main()
{
	char buffer[256];

	ifstream graphFile("C:\\Users\\Lee\\Desktop\\data\\Wordnet\\wordnet3.net");
	if (!graphFile.is_open())
	{
		cout << "Error opening file"; exit(1);
	}

	string *s = new string[NodeSize + 1];
	int id;
	graphFile.getline(buffer, sizeof(buffer));
	//cout << buffer << endl;
	for (int i = 1; i <= NodeSize; i++) {
		graphFile.getline(buffer, sizeof(buffer));
		stringstream word(buffer);
		word >> id;
		word >> s[i];
	}
	AdjGraph<string> dataGraph(s, NodeSize);
	delete[] s;

	int root;
	int node;
	graphFile.getline(buffer, sizeof(buffer));
	//cout << buffer << endl;
	while (!graphFile.eof()) {
		graphFile.getline(buffer, sizeof(buffer));
		stringstream word(buffer);
		word >> root;
		while (word >> node) {
			dataGraph.Insert(root,node);
		}
	}

	graphFile.close();

	dataGraph.Show(10);

	ifstream labelFile("C:\\Users\\Lee\\Desktop\\data\\Wordnet\\wordnet3.clu");
	if (!labelFile.is_open())
	{
		cout << "Error opening file"; exit(1);
	}

	labelFile.getline(buffer, sizeof(buffer));
	int *label = new int[NodeSize];
	for (int i = 1; i < NodeSize + 1; i++) {
		labelFile.getline(buffer, sizeof(buffer));
		stringstream word(buffer);
		word >> label[i];
	}

	labelFile.close();
	//				 0 1 2 3 4 5
	int a[5 + 1] = { 0,1,2,3,1,5 };
	AdjGraph<int> queryGraph(a, 5);
	//id = label-1
	queryGraph.Insert(1, 2);
	queryGraph.Insert(1, 3);
	queryGraph.Insert(3, 2);
	queryGraph.Insert(3, 5);
	queryGraph.Insert(4, 1);
	//queryGraph.Insert(3, 5);

	const int qsize = 3;
	int q[qsize] = { 1,4,3 };

	queryGraph.Show(9);

	ofstream outFile("C:\\Users\\Lee\\Desktop\\data\\Wordnet\\wordnet3out.txt");
	for (int j = 0; j < qsize; j++) {
		queryGraph.ShowOne(q[j], outFile);
		for (int i = 1; i <= NodeSize; i++) {
		//root
			int lenq = queryGraph.lenth(q[j]);
			int lend = dataGraph.lenth(i);
			if (label[i] == queryGraph.adjlist[q[j]].vertex && lend >= lenq) {//root match
				//cout << i << " match " << q[j] << " :" << endl;
				int * temp = new int[lenq];//开始位置
				int * ans = new int[lenq];//答案
				for (int i = 0; i < lenq; i++) {
					temp[i] = 0;
				}
				int now = 0;
				while (now>=0) {
					if (now == lenq) {//output the answer
						now = 0;
						outFile << i;
						for (int o = 0; o < lenq; o++)
							outFile << ' ' << dataGraph.getk(i, temp[o]);
						outFile << endl;
					}
					for (int k = temp[now] + 1; k <= lend+1; k++) {//find the match and process the end
						if (k == lend+1) {//end
							temp[now] = lend + 1;
							break;
						}
						int a = dataGraph.getk(i, k);
						int b = queryGraph.getk(q[j], now + 1);
						if ( label[a] == queryGraph.adjlist[b].vertex && !isRepeat(temp,now,a) ) {//match
							temp[now] = k;
							break;
						}
					}
					if (temp[now] == lend + 1) {
						temp[now] = 0;
						now--;
					}else
						now++;
				}
			}
		}
		
	}
	outFile.close();

	return 0;
}