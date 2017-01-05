#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "data_extern.h"
#include "head.h"
typedef map<int, int> Table;
void filter(Table* tables) {
	int size = 0;
	Table t;
	for (int i = 0; i < Num; i++) {
		for (Table::iterator it = tables[i].begin(); it != tables[i].end(); it++) {
			if (t[it->first] == 1) {
				size++;
			}
			t[it->first]++;
		}
	}
	int *hash = (int*)malloc(sizeof(int)*size);
	for (Table::iterator it = t.begin(); it != t.end(); it++) {
		if (it->second > 1) {
			//hash[func()] = it->first;
		}
	}
}