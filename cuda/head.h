#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#define MAXINT 0x7ffff
using namespace std;

string Query[] = { "a", "b", "c", "d", "e", "f", "g" };
string Q1[] = { "a", "b", "c" };
string Q2[] = { "b", "g", "d", "e", "f" };
string Q3[] = { "d", "c", "a" };
string Q4[] = { "f","g","e" };
int Rq1[] = {
	10010, 10020, 10030,
	10011, 10021, 10030,
	10010, 10021, 10032 };
int Rq2[] = {
	10023, 10070, 10041, 10050, 10060,
	10020, 10070, 10042, 10050, 10060,
	10021, 10070, 10043, 10050, 10060,
	10022, 10071, 10044, 10050, 10060 };
int Rq3[] = {
	10040, 10030, 10011,
	10041, 10032, 10010,
	10041, 10030, 10011,
	10043, 10031, 10010,
	10042, 10030, 10011 };
int Rq4[] = {
	10060, 10070, 10050,
	10060, 10072, 10050,
	10061, 10070, 10051,
	10063, 10071, 10050,
	10062, 10072, 10050,
	10060, 10072, 10051 };

const int Num = 4;//关系数量
int NumOfTuple[Num] = { 3,4,5,6 };//元组大小
int SizeOfTuple[Num] = {3,5,3,3};//元组个数
int* Rq[Num] = {Rq1,Rq2,Rq3,Rq4};
string* Q[Num] = { Q1,Q2,Q3,Q4 };
typedef map<int, int> Table;
Table* tables = new Table[Num];

struct cal_key//散列函数
{
	int num;//key数量
	int sum;//key和
	cal_key(int a, int b) {
		num = a;
		sum = b;
	}
	bool operator<(const cal_key & ct) const    
	{
		if (num < ct.num) {
			return true;
		}
		else if (num == ct.num) {
			if (sum < ct.sum) {
				return true;
			}
		}

		return false;
	}
};