#include <stdio.h>
#include <iostream>
#include <string>
#include "head.h"
using namespace std;


void printTable(Table* table) {
	Table::iterator it;
	cout << (*table).size() << ' :'<<endl;
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

long calSize(bool S[Num]) {
	unsigned int min = tables[0].size();//table的大小
	int row = 0;//第几个table
	for (int i = 0;i < Num; i++) {
		if (S[i] && min > tables[i].size()) {
			min = tables[i].size();
			row = i;
		}
	}
	
	map<int,long> n;
	Table::iterator it;

	for (int i = 0;i < Num; i++) {//O(Num)
		if (S[i] && i != row) {
			for (it = tables[row].begin(); it != tables[row].end(); it++) {//O(min)
				int key = it->first;
				if (tables[i].count(key)!=0) {
					if (n[key] == 0) n[key] = 1;
					n[key] *= tables[i][key];
				}
			}
		}
	}

	long res = 1;
	
	for (int i = 0;i < Num; i++) {
		if (S[i]) res *= NumOfTuple[i];
	}
	cout << res << endl;
	for (map<int, long>::iterator it = n.begin(); it != n.end();it++) {
		res = res / it->second;
		cout << it->first << ' ' << it->second << ' ' << res << endl;
	}
	return res;
}

int* calOrder() {
	int order[Num];
	int* ori_order = order;

	bool s[Num];
	for (int i = 0;i < Num; i++) {
		s[i] = true;
	}

	if (count == 2) {
		long res = 1;
		for (int i = 0;i < Num; i++) {
			if (S[i]) {
				res *= NumOfTuple[i];
			}
		}
		return res;
	}
	else {
		for (int i = 0; i < Num;i++) {
			if (S[i]) {
				bool s_new[Num];
				tans_value(s_new, S);
				s_new[i] = false;
				if (max < calSize(s_new, order)) {
					max = calSize(s_new, order);
					*order = i;
				}
			}
		}
		order++;
		cout << *order << " ";
		return max;
	}

	return ori_order;
}

int main() {
	///////////////////////////
	//get datas and info
	//////////////////////////
	for (int i = 0; i < Num; i++) {
		Table* t = tables+i;
		int* r = Rq[i];
		for (int j = 0; j < NumOfTuple[i] * SizeOfTuple[i]; j++) {
			(*t)[r[j]]++;
		}
		//cout << endl;
		//printTable(&t);
	}
	//////////////////
	//start cal order
	//////////////////
	bool s[Num] = {false};
	s[0] = true;
	s[1] = true;
	cout<<calSize(s)<<endl;
	//////////////////
	//free
	//////////////////
	//? free the tables
	// free the datas
	return 0;
}
