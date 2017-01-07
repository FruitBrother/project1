#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "data_extern.h"
#include "head.h"
#include <algorithm>
void swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}
extern "C" void filter(int *rqa, int *rqb, dint* equal, int stepa, int stepb, int numa, int numb, int equalsize);
void filter(int a, int b) {
	/*init*/
	int numa = NumOfTuple[a];
	int numb = NumOfTuple[b];
	if (numa > numb) {
		swap(numa, numb);
		swap(a, b);
	}//a < b;
	string *qa, *qb;
	int stepa, stepb, numa, numb;
	int* rqa, *rqb;
	qa = Q[a];
	qb = Q[b];
	rqa = Rq[a];
	rqb = Rq[b];
	stepa = SizeOfTuple[a];
	stepb = SizeOfTuple[b];
	/*pre process*/
	dint* equal = 0;
	equal = (dint*)malloc(sizeof(dint)*MAX(stepa, stepb));
	int equalsize = 0;
	for (int i = 0; i < stepa; i++) {
		for (int j = 0; j < stepb; j++) {
			if (!qa[i].compare(qb[j])) {
				equal[equalsize].a = i;
				equal[equalsize].b = j;
				equalsize++;
			}
		}
	}
	//sort(equal, equal + equalsize, comp);
	filter(rqa, rqb, equal, stepa, stepb, numa, numb, equalsize);
}