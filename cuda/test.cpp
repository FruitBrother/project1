#pragma once
#include <iostream>
#include "head.h"
#define MAX(a,b) (a<b)?b:a
struct dint {
	int a, b;
};

using namespace std;
int main1() {
	/*init*/
	string *qa, *qb;
	int a, b, stepa, stepb, numa, numb;
	int* rqa, *rqb;
	a = 0;
	b = 1;
	qa = Q[a];
	qb = Q[b];
	rqa = Rq[a];
	rqb = Rq[b];
	stepa = SizeOfTuple[a];
	stepb = SizeOfTuple[b];
	numa = NumOfTuple[a];
	numb = NumOfTuple[b];
	/*pre process*/
	dint* equal;
	equal = (dint*)malloc(sizeof(dint)*MAX(numa, numb));
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
	/*init result*/
	int* res;
	int stepres;
	int numres = 0;
	stepres = stepa + stepb - equalsize;
	string *resQ = (string*)malloc(sizeof(string)*(stepres));
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
	
	//print
	for (int i = 0; i < numres; i++) {
		for (int j = 0; j < stepres; j++) {
			cout << res[i*stepres + j] << ' ';
		}
		cout << endl;
	}
	return 0;
}