#pragma once
#pragma warning(disable:4819)
#define WITHORDER 1
#define WITHOUTORDER 0
#define MAXINT 0x7ffffff
#define NORESULT 1
#define HASRESULT 0
#define BLOCKX 16
#define BLOCKY 16
#define BLOCK_SIZE BLOCKX*BLOCKY
#define CUDACHECK(cudaStatus)\
    if (cudaStatus != cudaSuccess) {\
		printf("Error: %s\n", cudaGetErrorString(cudaStatus));\
		exit(-1);\
	}
#define CUDAKERNELCHECK\
    cudaStatus = cudaGetLastError();\
	if (cudaStatus != cudaSuccess) {\
		fprintf(stderr, "Kernel failed: %s\n", cudaGetErrorString(cudaStatus));\
		exit(-1);\
	}
//#define MAX(a,b) (a<b)?b:a
struct dint {
	int a, b;
};
const int Num = 3;
extern unsigned int MAX(int a, int b);
extern bool comp(dint a, dint b);