#pragma warning(disable:4819)
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "head.h"
#include "scan.h"
#define HASHSIZE 10000;
__global__ void createNeta(int *data, int step, int datanum, dint *equal, int equalsize, int* hash)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int i = 0;
	if (x >= datanum) return;
	unsigned int sum = 0;
	for (i = 0; i < equalsize; i++) {
		sum += data[x*step + equal[i].a];
	}
	sum %= HASHSIZE;
	if (hash[sum] = 0)
		atomicAdd(hash[sum], 1);//1
}
__global__ void createNetb(int *data, int step, int datanum, dint *equal, int equalsize, int* hash)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int i = 0;
	if (x >= datanum) return;
	unsigned int sum = 0;
	for (i = 0; i < equalsize; i++) {
		sum += data[x*step + equal[i].a];
	}
	sum %= HASHSIZE;
	if (hash[sum] != 0){
		atomicAdd(hash[sum], 1);//>1
	}
}
__global__ void useNeta(int *data, int step, int datanum, dint *equal, int equalsize, int* hash, int *res) {
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int i = 0;
	if (x >= datanum) return;
	unsigned int sum = 0;
	for (i = 0; i < equalsize; i++) {
		sum += data[x*step + equal[i].a];
	}
	sum %= HASHSIZE;
	if (hash[sum] >1 )
		res[x*step]
}

extern "C" void filter(int *rqa, int *rqb, dint* equal, int stepa, int stepb, int numa, int numb, int equalsize)
{
	int *dev_rqa = 0;
	int *dev_rqb = 0;
	dint *dev_equal = 0;
	int *d_hash = 0;
	int *res = 0;
	cudaSetDevice(0);
	cudaError_t cudaStatus;
	CUDACHECK(cudaMalloc((void**)&dev_rqa, numa*stepa * sizeof(int)));
	CUDACHECK(cudaMalloc((void**)&dev_rqb, numb*stepb * sizeof(int)));
	CUDACHECK(cudaMalloc((void**)&dev_equal, sizeof(dint)*MAX(numa, numb)));
	CUDACHECK(cudaMalloc((void**)&d_hash, sizeof(int)*numa));//a is min

	CUDACHECK(cudaMemcpy(dev_rqa, rqa, numa*stepa * sizeof(int), cudaMemcpyHostToDevice));
	CUDACHECK(cudaMemcpy(dev_rqb, rqb, numb*stepb * sizeof(int), cudaMemcpyHostToDevice));
	CUDACHECK(cudaMemcpy(dev_equal, equal, sizeof(dint)*MAX(numa, numb), cudaMemcpyHostToDevice));

	createNeta <<< (numa + BLOCK_SIZE - 1) / BLOCK_SIZE, BLOCK_SIZE >>> (dev_rqa, stepa, numa, dev_equal, equalsize, d_hash);
	createNetb <<< (numb + BLOCK_SIZE - 1) / BLOCK_SIZE, BLOCK_SIZE >>> (dev_rqb, stepb, numb, dev_equal, equalsize, d_hash);


}