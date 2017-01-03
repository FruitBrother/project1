#pragma once
#include <stdio.h>
#include <iostream>
#include <string>

#include "data.h"
#include "cpu.h"
#include "gpu.h"
#include "order.h"
#include "head.h"
#include "scan.h"

using namespace std;
int run() {
	string s;
	cin >> s;
	int* ord = order();
	int a, b;
	a = ord[0];
	if (!s.compare("gpu")) {
		for (int i = 1; i < Num; i++) {
			b = ord[i];
			if (gpu(a, b) == NORESULT)
				printf("No result!\n");
		}
	}
	if (!s.compare("gpu with scan")) {
		for (int i = 1; i < Num; i++) {
			b = ord[i];
			if (gpuwithscan(a, b) == NORESULT)
				printf("No result!\n");
		}
	}
	if (!s.compare("cpu")) {
		for (int i = 1; i < Num; i++) {
			b = ord[i];
			if (cpu(a, b) == NORESULT)
				printf("No result!\n");
		}
	}
		
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
	run();
	//Test();
	return 0;
}