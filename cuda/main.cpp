#include <stdio.h>
#include <iostream>
#include <string>
#include "head.h"
#include <set>
using namespace std;
//C(n,n/2)
int maxZuHe(int n)
{
	int k = n / 2;
	int i = 1;
	int temp1 = 1, temp2 = 1;
	if (n == k)
	{
		return 1;
	}
	else
	{
		for (i = 1;i <= k;i++)
		{
			temp1 *= n - i + 1;
			temp2 *= i;
		}
		temp1 /= temp2;
		return temp1;
	}
}

void PrintTable(Table* table) {
	Table::iterator  it;
	Element::iterator it2;
	for (it = (*table).begin();it != (*table).end();it++) {
		cout << "key:" << it->first << " value:<";
		for (it2 = it->second.begin(); it2 != it->second.end();it2++) {
			cout << it2->first << " " << it2->second << ",";
		}
		cout << ">" << endl;
	}
	printf("-----------\n");
}

long Size(Table* A, Table* B, int sizeofA, int sizeofB) {
	Table::iterator  it;
	Element::iterator it2;
	long m1 = sizeofA*sizeofB;
	long m2 = 1;
	if ((*A).size() < (*B).size()) {
		Table* swap;
		swap = A;
		A = B;
		B = swap;
	}
	for (it = (*A).begin(); it != (*A).end(); it++) {
		string key = it->first;
		if ((*B)[key].size() != 0) {
			Element *EA, *EB;
			if ((*A)[key].size() < (*B)[key].size()) {
				EA = &(*A)[key];
				EB = &(*B)[key];
			}
			else
			{
				EB = &(*A)[key];
				EA = &(*B)[key];
			}
			for (it2 = (*EA).begin(); it2 != (*EA).end(); it2++) {
				if ((*EB).count(it2->first) != 0) {
					int a = it2->second;
					int b = (*EB)[it2->first];
					m2 *= (a > b) ? a : b;
					cout << it2->first << " " << m2 << endl;
				}
			}
			
		}
	}
	return m1/m2;
}

int weight(set<int> s) {
	int mini;
	set<int>::iterator i;
	for (i = s.begin(); i != s.end();i++) {
		set<int> temp;
		mini = weight(s.erase)
	}
}

int main() {
	const int rq1Size = 3;
	const int q1Szie = 3;
	int Rq1[rq1Size][q1Szie]  = {
		10010,10020,10030,
		10011,10021,10030,
		10010,10021,10032
	};
	char** Q1 = (char**)malloc(sizeof(char*)*q1Szie);
	Q1[0] = "a";
	Q1[1] = "b";
	Q1[2] = "c";

	const int rq2Size = 4;
	const int q2Szie = 5;
	int Rq2[rq2Size][q2Szie] = {
		10023, 10070, 10041, 10050, 10060,
		10020, 10070 ,10042, 10050, 10060,
		10021, 10070, 10043, 10050, 10060,
		10022, 10071, 10044, 10050, 10060
	}; 
	char** Q2 = (char**)malloc(sizeof(char*)*q2Szie);
	Q2[0] = "b";
	Q2[1] = "g";
	Q2[2] = "d";
	Q2[3] = "e";
	Q2[4] = "f";

	const int rq3Size = 5;
	const int q3Szie = 3;
	int Rq3[rq3Size][q3Szie] = {
		10040, 10030, 10011,
		10041, 10032 ,10010,
		10041, 10030, 10011,
		10043, 10031, 10010,
		10042, 10030, 10011,
	};
	char** Q3 = (char**)malloc(sizeof(char*)*q3Szie);
	Q3[0] = "d";
	Q3[1] = "c";
	Q3[2] = "a";

	const int rq4Size = 6;
	const int q4Szie = 3;
	int Rq4[rq4Size][q4Szie] = {
		10060, 10070, 10050,
		10060, 10072 ,10050,
		10061, 10070, 10051,
		10063, 10071, 10050,
		10062, 10072, 10050,
		10060, 10072 ,10051,
	};
	char** Q4 = (char**)malloc(sizeof(char*)*q4Szie);
	Q4[0] = "f";
	Q4[1] = "g";
	Q4[2] = "e";

	string Q[] = { "a","b","c","d","e","f","g"};

	const int num = 4;

	///////////////////////////
	//get datas and info
	//////////////////////////
	Table* tables = new Table[num];

	int*** datas = (int***)malloc(sizeof(int**)*num);
	int** data = NULL;
	int i, j;
	
	int numOfTuple[num];
	int sizeOfTuple[num];
	numOfTuple[0] = rq1Size;
	sizeOfTuple[0] = q1Szie;
	numOfTuple[1] = rq2Size;
	sizeOfTuple[1] = q2Szie;
	numOfTuple[2] = rq3Size;
	sizeOfTuple[2] = q3Szie;
	numOfTuple[3] = rq4Size;
	sizeOfTuple[3] = q4Szie;

	//Allocate space;
	for (int index = 0;index < num;index++) {
		data = (int**)malloc(sizeof(int*)*numOfTuple[index]);
		datas[index] = data;
		for (i = 0; i < numOfTuple[index]; i++) {
			data[i] = (int*)malloc(sizeof(long)*sizeOfTuple[index]);
		}
		//assginment
		for (i = 0;i < numOfTuple[index];i++) {
			for (j = 0; j < sizeOfTuple[index];j++) {
				data[i][j] = (index == 0) ? Rq1[i][j] : (index == 1) ? Rq2[i][j] : (index == 2) ? Rq3[i][j] : Rq4[i][j];
				(tables[index])[(index == 0) ? Q1[j] : (index == 1) ? Q2[j] : (index == 2) ? Q3[j] : Q4[j]][data[i][j]]++;
			}
		}
		PrintTable(tables+index);
	}

	int n = maxZuHe(num);
	one** r;
	r = (one**)malloc(sizeof(one*) * 2);
	for (i = 0; i < 2;i++) {
		r[i] = (one*)malloc(sizeof(one)*n);
	}
	for (i = 1; )
	

	//////////////////
	//free
	//////////////////
	//? free the tables
	// free the datas
	for (int k = 0; k < 2; ++k) {
		data = datas[k];
		for (i = 0;i < numOfTuple[k];++i) {
			delete[] data[i];
		}
		delete[] data;
	}
	delete[] datas;
}
