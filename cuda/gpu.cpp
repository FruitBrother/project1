#pragma comment( lib,"winmm.lib" )
#include <Windows.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "head.h"
#include "data_extern.h"
#include <algorithm>
extern "C" void gpu(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep, int numa, int numb, int equalsize);
int gpu(int a, int b) {
	/*init*/
	string *qa, *qb;
	int stepa, stepb, numa, numb;
	int* rqa, *rqb;
	qa = Q[a];
	qb = Q[b];
	rqa = Rq[a];
	rqb = Rq[b];
	stepa = SizeOfTuple[a];
	stepb = SizeOfTuple[b];
	numa = NumOfTuple[a];
	numb = NumOfTuple[b];
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
	sort(equal, equal + equalsize, comp);
	/*init result*/
	int* res;
	int stepres;
	int numres = 0;
	stepres = stepa + stepb - equalsize;
	string *resQ = new string[stepres];
	int temp = 0, temp1 = 0;
	for (int k = 0; k < stepa + stepb; k++) {
		if (k < stepa) {
			resQ[k] = qa[k];
			temp = k;
		}
		else if (equal[temp1].b != k - stepa) {
			resQ[(++temp)] = qb[(k - stepa)];
		}
		else {
			temp1++;
		}
	}
	res = (int*)malloc(sizeof(int)*stepres*(numa*numb));
	/*process*/
	DWORD t1, t2;
	t1 = timeGetTime();
	gpu(rqa,rqb,res,equal,stepa,stepb,stepres,numa,numb,equalsize);
	numres = numa*numb;
	t2 = timeGetTime();
	printf("process Use Time:%f s\n", (t2 - t1)*1.0 / 1000);
	//process the result 
	t1 = timeGetTime();
	//**IF the memory allocated in GPU defult is 0
	int* tem = (int*)malloc(sizeof(int)*numres);
	int temnum = 0;
	for (int i = 0; i < numres; i++) {
		if (res[i*stepres] != 0) {
			tem[temnum++] = i;
		}
	}
	numres = temnum;
	int *new_res = (int*)malloc(sizeof(int)*stepres*(temnum));
	for (int i = 0; i < numres; i++) {
		for (int j = 0; j < stepres; j++) {
			new_res[i*stepres + j] = res[tem[i]*stepres + j];
		}
	}
	t2 = timeGetTime();
	printf("process the result Use Time:%f s\n", (t2 - t1)*1.0 / 1000);
	//free
	//free(Rq[a]);
	//free(Q[a]);
	free(res);
	free(tem);
	free(equal);
	Rq[a] = new_res;
	Q[a] = resQ;
	NumOfTuple[a] = numres;
	SizeOfTuple[a] = stepres;
	if (numres == 0) return NORESULT;
	return HASRESULT;
}

extern "C" void gpuwithscan(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep, int numa, int numb, int equalsize, int* numresgpu);
int gpuwithscan(int a, int b)
{
	/*init*/
	string *qa, *qb;
	int stepa, stepb, numa, numb;
	int* rqa, *rqb;
	qa = Q[a];
	qb = Q[b];
	rqa = Rq[a];
	rqb = Rq[b];
	stepa = SizeOfTuple[a];
	stepb = SizeOfTuple[b];
	numa = NumOfTuple[a];
	numb = NumOfTuple[b];
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
	sort(equal, equal + equalsize, comp);
	/*init result*/
	int* res;
	int stepres;
	int numres = 0;
	stepres = stepa + stepb - equalsize;
	string *resQ = new string[stepres];
	int temp = 0, temp1 = 0;
	for (int k = 0; k < stepa + stepb; k++) {
		if (k < stepa) {
			resQ[k] = qa[k];
			temp = k;
		}
		else if (equal[temp1].b != k - stepa) {
			resQ[(++temp)] = qb[(k - stepa)];
		}
		else {
			temp1++;
		}
	}
	res = (int*)malloc(sizeof(int)*stepres*(numa*numb));
	/*process*/
	DWORD t1, t2;
	t1 = timeGetTime();
	gpuwithscan(rqa, rqb, res, equal, stepa, stepb, stepres, numa, numb, equalsize, &numres);	//numres = numresgpu;
	t2 = timeGetTime();
	printf("process Use Time:%f s\n", (t2 - t1)*1.0 / 1000);
	//free
	//free(Rq[a]);
	//free(Q[a]);
	free(equal);
	Rq[a] = res;
	Q[a] = resQ;
	NumOfTuple[a] = numres;
	SizeOfTuple[a] = stepres;
	if (numres == 0) return NORESULT;
	return HASRESULT;
}