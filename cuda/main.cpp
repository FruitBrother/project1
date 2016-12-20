#include <stdio.h>
#include <iostream>
#include <string>
#include "head.h"
using namespace std;
//C(n,n/2)
int maxZuHe(int n) {
	int k = n / 2;
	int i = 1;
	int temp1 = 1, temp2 = 1;
	if (n == k) {
		return 1;
	} else {
		for (i = 1; i <= k; i++) {
			temp1 *= n - i + 1;
			temp2 *= i;
		}
		temp1 /= temp2;
		return temp1;
	}
}

void PrintTable(Table* table) {
	Table::iterator it;
	Element::iterator it2;
	for (it = (*table).begin(); it != (*table).end(); it++) {
		cout << "key:" << it->first << " value:<";
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			cout << it2->first << " " << it2->second << ",";
		}
		cout << ">" << endl;
	}
	printf("-----------\n");
}

/*
Size(tables,newset,num,sizeOftuple);
void Size(Table* ori,  bool* Set, int num, int* sizeOfTuple){
	for (int i = 0 ; i< num; i++){
		if (Set[i]){
			ori[i]
		}
	}
}
*/

double Size(Table* ori, int num, Table* ne, int sizeofOri, int sizeofNe){
	Table::iterator it;
	Table::iterator it1;
	Element::iterator it2;
	map<int,int> ha;
	long m1 = sizeofOri * sizeofNe;
	long m2 = 1;
	for (it = (*ne).begin(); it != (*ne).end(); it++) {//key
		string key = it->first;
		int max = 0;
		for (int i = 0 ; i<num; i++){//tables
			Table* t = ori+i;
			if ((*t)[key].size() != 0) {//key exist
				Element *EA, *EB;
				if ( (*t)[key].size() < (*ne)[key].size() ) {//smaller size is EA
					EA = &(*t)[key];
					EB = &(*ne)[key];
				} else {
					EB = &(*ne)[key];
					EA = &(*t)[key];
				}
				for (it2 = (*EA).begin(); it2 != (*EA).end(); it2++) {//id = it2->first
					if ((*EB).count(it2->first) != 0) {
						int a = it2->second;
						int b = (*EB)[it2->first];
						a = (a > b) ? a : b;
						int k = it2->first;
						if (ha.count(k) != 0)
							ha[k] = a;
						else
							if (a > ha[k])
								ha[k] = a;
					}
				}//id
			}//key exist
		}//tables
	}//key
	delete[] ori;
	for (map<int,int>::iterator i = ha.begin(); i != ha.end(); i++){
		cout<<i->first<<" "<<i->second<<endl;
		m2 *= i->second;
	}
	return m1*1.0/m2;
}

template<typename T>
void equal(T* des, T*sour,int s,int n){
	if (des!=sour){
		for (int i = s ;i<s+n; i++){
			des[i] = sour[i];
		}
	}
}
bool isAlreadyExist(bool *newset, int num, one *RowNow, int nownum){
	if (nownum==0) return false;
	bool jud;
	int i;
	for (i = 0; i<nownum;i++){
		jud = true;
		for (int j = 0 ;j < num ; j++)
			if (RowNow[i].set[j] != newset[j]){
				jud = false;
				break;
			}
		if (jud) break;
		else continue;
	}
	if (jud) return true;
	else return false;

}
int main() {
	//待处理数据
	const int rq1Size = 3;
	const int q1Szie = 3;
	int Rq1[rq1Size][q1Szie] = { 10010, 10020, 10030, 10011, 10021, 10030,
			10010, 10021, 10032 };
	char** Q1 = (char**) malloc(sizeof(char*) * q1Szie);
	Q1[0] = "a";
	Q1[1] = "b";
	Q1[2] = "c";

	const int rq2Size = 4;
	const int q2Szie = 5;
	int Rq2[rq2Size][q2Szie] = { 10023, 10070, 10041, 10050, 10060, 10020,
			10070, 10042, 10050, 10060, 10021, 10070, 10043, 10050, 10060,
			10022, 10071, 10044, 10050, 10060 };
	char** Q2 = (char**) malloc(sizeof(char*) * q2Szie);
	Q2[0] = "b";
	Q2[1] = "g";
	Q2[2] = "d";
	Q2[3] = "e";
	Q2[4] = "f";

	const int rq3Size = 5;
	const int q3Szie = 3;
	int Rq3[rq3Size][q3Szie] = { 10040, 10030, 10011, 10041, 10032, 10010,
			10041, 10030, 10011, 10043, 10031, 10010, 10042, 10030, 10011, };
	char** Q3 = (char**) malloc(sizeof(char*) * q3Szie);
	Q3[0] = "d";
	Q3[1] = "c";
	Q3[2] = "a";

	const int rq4Size = 6;
	const int q4Szie = 3;
	int Rq4[rq4Size][q4Szie] = { 10060, 10070, 10050, 10060, 10072, 10050,
			10061, 10070, 10051, 10063, 10071, 10050, 10062, 10072, 10050,
			10060, 10072, 10051, };
	char** Q4 = (char**) malloc(sizeof(char*) * q4Szie);
	Q4[0] = "f";
	Q4[1] = "g";
	Q4[2] = "e";

	string Q[] = { "a", "b", "c", "d", "e", "f", "g" };

	const int num = 4;//关系数量

	///////////////////////////
	//get datas and info
	//////////////////////////
	Table* tables = new Table[num];

	int*** datas = (int***) malloc(sizeof(int**) * num);//数据
	int** data = NULL;
	int i, j;

	int numOfTuple[num];//元组个数
	int sizeOfTuple[num];//元组大小
	numOfTuple[0] = rq1Size;
	sizeOfTuple[0] = q1Szie;
	numOfTuple[1] = rq2Size;
	sizeOfTuple[1] = q2Szie;
	numOfTuple[2] = rq3Size;
	sizeOfTuple[2] = q3Szie;
	numOfTuple[3] = rq4Size;
	sizeOfTuple[3] = q4Szie;

	//Allocate space;
	for (int index = 0; index < num; index++) {
		data = (int**) malloc(sizeof(int*) * numOfTuple[index]);
		datas[index] = data;
		for (i = 0; i < numOfTuple[index]; i++) {
			data[i] = (int*) malloc(sizeof(long) * sizeOfTuple[index]);
		}
		//assginment
		for (i = 0; i < numOfTuple[index]; i++) {
			for (j = 0; j < sizeOfTuple[index]; j++) {
				data[i][j] = (index == 0) ? Rq1[i][j] :
								(index == 1) ? Rq2[i][j] :
								(index == 2) ? Rq3[i][j] : Rq4[i][j];
				(tables[index])[(index == 0) ? Q1[j] : (index == 1) ? Q2[j] :
								(index == 2) ? Q3[j] : Q4[j]][data[i][j]]++;
			}
		}
		PrintTable(tables + index);
	}
	//////////////////
	//start cal order
	//////////////////

	//cout<<Size(tables,2,tables+2,numOfTuple[0],numOfTuple[2]);
	int n = maxZuHe(num);//最大组合数
	int now = 0;//0 or 1 当前所在行数
	int prenum = 0;//前一行pair的数量
	one** r;//两行列表
	//allocate space
	r = (one**) malloc(sizeof(one*) * 2);
	for (i = 0; i < 2; i++) {
		r[i] = (one*) malloc(sizeof(one) * n);
		for (j = 0; j < n ; j++)
			r[i][j].set = (bool*) malloc (sizeof(bool)*num);
			r[i][j].order = (int*) malloc (sizeof(int)*num);
	}
	//init
	for (i = 0; i < num; i++) {
		memset(r[now][i].set,0,sizeof(bool)*num);
		r[now][i].set[i] = true;
		r[now][i].size = numOfTuple[i];
		r[now][i].cost = 0;
		r[now][i].order[0] = i;
	}
	prenum = num;
	//process
	/*
	bool* newset = (bool*) malloc (sizeof(bool)*num);//temp
	for (i = 1; i<num; i++){// i times
		int pre = now;//~now
		now = ~now;
		int nownum = 0;//当前行pair个数
		for(int pair = 0; pair < prenum; pair++){//each pair in r[now]   pair
			for (int add = 0; add < num; add++){//try to add an element  add
				if (! r[pre][pair].set[i]){//each ele not in the set  pair + add
					equal<bool*>(newset,r[pre][pair].set,0,num);
					newset[add] = true;
					if (isAlreadyExist(newset,num,r[now],nownum))//new set do exisit in now
					{
						//比较size保留较小

					}else{//new set don't exisit in now
						equal(r[now][nownum].set,newset,0,num);

						equal(r[now][nownum].order,r[pre][pair].order,0,i);
						r[now][nownum].order[i] = add;

						r[now][nownum].size = Size(tables,newset,num,sizeOfTuple);

						r[now][nownum].cost = ;

						nownum++;
					}

				}
			}
		}
	}
*/
	//////////////////
	//free
	//////////////////
	//? free the tables
	// free the datas
	for (int k = 0; k < 2; ++k) {
		data = datas[k];
		for (i = 0; i < numOfTuple[k]; ++i) {
			delete[] data[i];
		}
		delete[] data;
	}
	delete[] datas;
}
