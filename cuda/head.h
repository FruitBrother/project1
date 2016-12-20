#pragma once
#include <map>
using namespace std;
typedef map<int, int> Element;
typedef map<string, Element> Table;
typedef struct {
	bool* set;
	long size;
	long cost;
	int* order;
} one;
