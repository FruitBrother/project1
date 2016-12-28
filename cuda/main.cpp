#pragma once
#include <stdio.h>
#include <iostream>
#include <string>

#include "data.h"
#include "cpu.h"
#include "gpu.h"
#include "order.h"
#include "head.h"
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
	if (!s.compare("cpu")) {
		for (int i = 1; i < Num; i++) {
			b = ord[i];
			if (cpu(a, b) == NORESULT)
				printf("No result!\n");
		}
	}
		
	return 0;
}
int main() {
	run();
	return 0;
}