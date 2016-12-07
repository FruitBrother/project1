#pragma once
#include <map>
#include <set>
using namespace std;
typedef map<int, int> Element;
typedef map<string, Element> Table;
typedef struct {
	set<int> s;
	long size;
	long cost;
	int* order;
} one;