#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "head.h"
__global__ void work_efficient_scan_kernel(unsigned int *X, unsigned int *Y, int InputSize) {
	// XY[2*BLOCK_SIZE] is in shared memory
	__shared__ unsigned int XY[BLOCK_SIZE * 2];
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	//printf("%d=%d*%d+%d\n",i,blockIdx.x,blockDim.x,threadIdx.x);
	if (i < InputSize) {
		XY[threadIdx.x] = X[i];
		XY[threadIdx.x + BLOCK_SIZE] = 0;
	}
	printf("XY[%d]=%d\n", threadIdx.x, XY[threadIdx.x]);
	// the code below performs iterative scan on XY¡¡¡¡
	for (unsigned int stride = 1; stride <= BLOCK_SIZE; stride *= 2) {
		__syncthreads();
		int index = (threadIdx.x + 1)*stride * 2 - 1;
		if (index < 2 * BLOCK_SIZE) {
			XY[index] += XY[index - stride];//index is alway bigger than stride
			printf("XY[%d] += XY[%d]= %d |\n", index , index - stride ,XY[index]);
		}
		__syncthreads();
	}
	// threadIdx.x+1 = 1,2,3,4....
	// stridek index = 1,3,5,7...


	for (unsigned int stride = BLOCK_SIZE / 2; stride > 0; stride /= 2) {
		__syncthreads();
		int index = (threadIdx.x + 1)*stride * 2 - 1;
		if (index < 2 * BLOCK_SIZE) {
			XY[index + stride] += XY[index];
			printf("XY[%d] += XY[%d]= %d |\n", index + stride, index, XY[index + stride]);
		}
	}
	__syncthreads();
	if (i < InputSize) Y[i] = XY[threadIdx.x];
}
extern "C" void prefix_sum(unsigned int *x, unsigned int *result, unsigned int size) {
	unsigned int* dev_x = 0;
	unsigned int* dev_result = 0;
	cudaSetDevice(0);

	cudaMalloc((void**)&dev_x, size * sizeof(unsigned int));
	cudaMalloc((void**)&dev_result, size * sizeof(unsigned int));

	cudaMemcpy(dev_x, x, size * sizeof(unsigned int), cudaMemcpyHostToDevice);

	work_efficient_scan_kernel <<< 1, BLOCK_SIZE >>> (dev_x, dev_result, size);
	cudaDeviceSynchronize();
	
	cudaMemcpy(result, dev_result, size * sizeof(unsigned int), cudaMemcpyDeviceToHost);

	cudaFree(dev_result);
	cudaFree(dev_x);
}