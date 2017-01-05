#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "data_extern.h"
#include "head.h"
typedef map<int, int> Table;
void filter(Table* tables) {
	Table t1;
	int* t2;
	for (int i = 0; i < Num; i++) {
		t1 = tables[i];
		for (Table::iterator it = t1.begin(); it != t1.end(); it++) {
			
		}
	}
}