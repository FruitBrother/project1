#pragma once
#define MAXINT 0x7ffff
#define NORESULT 1
#define BLOCKX 16
#define BLOCKY 16
#define BLOCK_SIZE BLOCKX*BLOCKY
//#define MAX(a,b) (a<b)?b:a
struct dint {
	int a, b;
};
const int Num = 4;
extern unsigned int MAX(int a, int b);
extern bool comp(dint a, dint b);