#pragma once
#pragma comment( lib,"winmm.lib" )
#include <Windows.h>
//#include <Mmsystem.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "data.h"
#include "cpu.h"
#include "gpu.h"
#include "order.h"
#include "head.h"
#include "scan.h"

using namespace std;
void readFile()
{
	char buffer[256];
	ifstream graphFile("C:\\Users\\lining\\Desktop\\data\\Wordnet\\wordnet3out.txt");
	if (!graphFile.is_open())
	{
		cout << "Error opening file"; exit(1);
	}

	for (int i = 0; i < Num; i++) {
		graphFile.getline(buffer, sizeof(buffer));
		cout << buffer << endl;
		for (int j = 0; j < NumOfTuple[i]; j++) {
			graphFile.getline(buffer, sizeof(buffer));
			stringstream word(buffer);
			int temp;
			for (int k = 0; k < SizeOfTuple[i]; k++) {
				word >> temp;
				Rq[i][j*SizeOfTuple[i] + k] = temp;
			}
		}
	}

	graphFile.close();
}
int run(string s = "gpuWithScan") {
	int* ord = order();
	int a, b;
	a = ord[0];
	if (!s.compare("gpu")) {
		for (int i = 1; i < Num; i++) {
			b = ord[i];
			if (gpu(a, b) == NORESULT) {
				printf("No result!\n");
				goto End;
			}
		}
	}
	if (!s.compare("gpuWithScan")) {
		for (int i = 1; i < Num; i++) {
			b = ord[i];
			if (gpuwithscan(a, b) == NORESULT) {
				printf("No result!\n");
				goto End;
			}
		}
	}
	if (!s.compare("cpu")) {
		for (int i = 1; i < Num; i++) {
			b = ord[i];
			if (cpu(a, b) == NORESULT) {
				printf("No result!\n");
				goto End;
			}
		}
	}
End:		
	return 0;
}
void Test() {
	const int numa = 16, numb = 16;
	const unsigned int number = numa*numb;
	unsigned int a[number];
	unsigned int r[number + 1] = { 0 };
	
	for (int i = 0; i < number; i++) {
		a[i] = i;
	}
	Scan(a, r, numa, numb);
	r[number] = r[number - 1] + a[number - 1];
	for (unsigned int i = 1; i <= number; i++) {
		cout << r[i] << endl;
	}
	cout << endl;
}
int main() {
	readFile();
	string s;
	cin >> s;

	DWORD t1, t2;
	t1 = timeGetTime();
	run(s);
	t2 = timeGetTime();
	printf("Use Time:%f s\n", (t2 - t1)*1.0 / 1000);

	//Test();
	system("pause");
	return 0;
}