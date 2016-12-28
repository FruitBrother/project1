#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"
#include "head.h"
#include "data_extern.h"
#include <algorithm>
using namespace std;
int cpu(int a, int b) {
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
	for (int i = 0; i < numa; i++) {//cuda
		for (int j = 0; j < numb; j++) {//cuda

			bool isMatch = true;
			for (int k = 0; k < equalsize; k++) {
				int equala = equal[k].a + i*stepa;
				int equalb = equal[k].b + j*stepb;
				if (rqa[equala] != rqb[equalb]) {
					isMatch = false;
					break;
				}
			}
			/* 这里保存了图的所有信息，注意内存
			*  若使用key值+id可能费时间
			*/
			if (isMatch) {
				int id = numres*stepres;
				int equalid = 0;
				int index = 0;
				for (int k = 0; k < stepa+stepb; k++) {
					if (k < stepa) {
						res[id + k] = rqa[i*stepa + k];
						index = k;
					}
					else if (equal[equalid].b != k - stepa) {
						res[id + (++index)] = rqb[j*stepb + (k - stepa)];
					}
					else {
						equalid++;
					}
				}
				numres++;
			}
				
		}
	}
	//free
	//free(Rq[a]);
	//free(Q[a]);
	free(equal);
	Rq[a] = res;
	Q[a] = resQ;
	NumOfTuple[a] = numres;
	SizeOfTuple[a] = stepres;
	if (numres == 0) return NORESULT;
	//print
	for (int j = 0; j < SizeOfTuple[a]; j++) {
		cout << Q[a][j] << ' ';
	}
	cout << endl;
	for (int i = 0; i < NumOfTuple[a]; i++) {
		for (int j = 0; j < SizeOfTuple[a]; j++) {
			cout << Rq[a][i*stepres + j] << ' ';
		}
		cout << endl;
	}
	return 0;
}