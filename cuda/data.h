#pragma once

#include "head.h"
#include <string>
using namespace std;
string Query[] = { "4", "1", "2", "3", "5"};
string Q1[] = { "1", "2", "3" };
string Q2[] = { "4", "1" };
string Q3[] = { "3", "2", "5" };
//string Q4[] = { "f","g","e" };
int NumOfTuple[Num] = { 229,101459,168 };//Ԫ���С
int SizeOfTuple[Num] = { 3,2,3 };//Ԫ�����

int *Rq1 = (int*)malloc(sizeof(int)* NumOfTuple[1 - 1] * SizeOfTuple[1 - 1]);
int *Rq2 = (int*)malloc(sizeof(int)* NumOfTuple[2 - 1] * SizeOfTuple[2 - 1]);
int *Rq3 = (int*)malloc(sizeof(int)* NumOfTuple[3 - 1] * SizeOfTuple[3 - 1]);
//int *Rq4 = (int*)malloc(sizeof(int)* NumOfTuple[4 - 1] * SizeOfTuple[4 - 1]);

int* Rq[Num] = { Rq1,Rq2,Rq3 };
string* Q[Num] = { Q1,Q2,Q3 };