#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "head.h"
__global__ void work_efficient_scan_kernel(float *X, float *Y, int InputSize) {
	// XY[2*BLOCK_SIZE] is in shared memory
	_shared__ float XY[BLOCK_SIZE * 2];
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	if (i < InputSize) { XY[threadIdx.x] = X[i]; }

	// the code below performs iterative scan on XY¡¡¡¡
	for (unsigned int stride = 1; stride <= BLOCK_SIZE; stride *= 2) {
		__syncthreads();
		int index = (threadIdx.x + 1)*stride * 2 - 1;
		if (index < 2 * BLOCK_SIZE)
			XY[index] += XY[index - stride];//index is alway bigger than stride
		__syncthreads();
	}
	// threadIdx.x+1 = 1,2,3,4....
	// stridek index = 1,3,5,7...


	for (unsigned int stride = BLOCK_SIZE / 2; stride > 0; stride /= 2) {
		__syncthreads();
		int index = (threadIdx.x + 1)*stride * 2 - 1;
		if (index < 2 * BLOCK_SIZE)
			XY[index + stride] += XY[index];

	}
	__syncthreads();
	if (i < InputSize) Y[i] = XY[threadIdx.x];
}