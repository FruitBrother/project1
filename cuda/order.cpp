#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "data_extern.h"
#include "head.h"
#include "filter.h"
using namespace std;
typedef map<int, int> Table;
Table* tables = new Table[Num];

void printTable(Table* table) {
	Table::iterator it;
	cout << (*table).size() << ':' << endl;
	for (it = (*table).begin(); it != (*table).end(); it++) {
		cout << it->first << " " << it->second << endl;
	}
	printf("-----------\n");
}
void tans_value(bool* des, bool* sour) {
	for (int i = 0; i < Num; i++) {
		des[i] = sour[i];
	}
}
/*
*	计算size
*/
long calSize(bool S[Num]) {
	unsigned int min = MAXINT;//table的大小
	int row = 0;//第几个table
	for (int i = 0;i < Num; i++) {
		if (S[i] && min > tables[i].size()) {
			min = tables[i].size();
			row = i;
		}
	}

	map<int, long> n;
	Table::iterator it;
	map<int, int> mini;

	for (int i = 0;i < Num; i++) {//O(Num)
		if (S[i] && i != row) {
			for (it = tables[row].begin(); it != tables[row].end(); it++) {//O(min)
				int key = it->first;
				if (tables[i].count(key) != 0) {
					int temp = tables[i][key];
					if (n[key] == 0) n[key] = 1;
					n[key] *= temp;
					if (mini[key] == 0 || mini[key] > temp) mini[key] = temp;
				}
			}
		}
	}

	long res = 1;
	for (map<int, long>::iterator it = n.begin(); it != n.end(); it++) {
		int v = tables[row][it->first];
		if (v > mini[it->first]) {
			it->second /= mini[it->first];
			it->second *= v;
		}
	}

	for (int i = 0;i < Num; i++) {
		if (S[i]) res *= NumOfTuple[i];
	}
	//cout << res << endl;
	for (map<int, long>::iterator it = n.begin(); it != n.end();it++) {
		res = res / it->second;
		//cout <<"/"<< it->first << ' ' << it->second << ' ' << res << endl;
	}
	if (res == 0) return 1;
	return res;
}
/* 贪心
*/
void calOrder(bool S[Num], int* order, int pos) {
	if (pos == -1) return;

	Table::iterator it;
	long min = MAXINT;
	int o = 0;
	bool s_new[Num];

	for (int i = 0;i < Num;i++) {
		bool temp_new[Num];
		if (S[i]) {
			tans_value(temp_new, S);
			temp_new[i] = false;
			long temp = calSize(temp_new);
			if (min > temp) {
				min = temp;
				o = i;
				tans_value(s_new, temp_new);
			}
		}
	}
	order[pos] = o;
	calOrder(s_new, order, --pos);
	return;
}
/*
*/
int* order(bool isorder = WITHOUTORDER) {
	///////////////////////////
	//get datas and info
	//////////////////////////
	for (int i = 0; i < Num; i++) {
		Table* t = tables + i;
		int* r = Rq[i];
		for (int j = 0; j < NumOfTuple[i] * SizeOfTuple[i]; j++) {
			(*t)[r[j]]++;
		}
		//printTable(t);
	}
	//////////////////
	//start cal order
	//////////////////
	bool s[Num];
	for (int i = 0; i < Num; i++) {
		s[i] = true;
	}

	int* order = (int*)malloc(sizeof(int)*Num);
	calOrder(s, order, Num - 1);
	//////////////////////////
	//free
	/////////////////////////
	if (isorder) filter(tables);
	delete[] tables;
	return order;
	/*
	for (int i = 0; i < Num;i++) {
	cout << order[i] << " ";
	}
	cout << endl;
	*/
}