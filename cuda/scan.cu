#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "scan.h"
#include "head.h"
__global__ void scan_bel(unsigned int* inputarray, unsigned int loop, unsigned int* outputarray, int numa, int numb)
{
	unsigned int number = numa*numb;
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if (x >= numa || y >= numb) return;
	unsigned int thIdx = y*numb + x;
	int divisor = 2;
	int adder = 1;
	unsigned int temp;

	for (int i = 0; i<loop; i++)
	{
		if (thIdx % (divisor) == divisor - 1)
		{
			outputarray[thIdx] = outputarray[thIdx - adder] + outputarray[thIdx];
			if (thIdx == 255)
				printf("outputarray[%d] = %d \n", thIdx, outputarray[thIdx]);
		}
		__syncthreads();
		divisor *= 2;
		adder *= 2;
	}

	divisor = number;
	adder = divisor / 2;

	outputarray[number - 1] = 0;
	for (int i = 0; i<loop; i++)
	{
		if (thIdx % (divisor) == divisor - 1)
		{
			temp = outputarray[thIdx];
			outputarray[thIdx] = outputarray[thIdx - adder] + outputarray[thIdx];
			outputarray[thIdx - adder] = temp;
		}
		__syncthreads();
		divisor /= 2;
		adder /= 2;
	}
	__syncthreads();
}
extern "C" void Scan(unsigned int * arrayin, unsigned int * arrayout, int numa, int numb)
{
	unsigned int number = numa*numb;
	unsigned int* arrayin_d = 0;
	unsigned int* arrayout_d = 0;
	int loop;
	loop = (int)log2f(number);
	cudaSetDevice(0);

	cudaMalloc((void**)&arrayin_d, number * sizeof(unsigned int));
	cudaMalloc((void**)&arrayout_d, number * sizeof(unsigned int));

	cudaMemcpy(arrayin_d, arrayin, number * sizeof(unsigned int), cudaMemcpyHostToDevice);
	cudaMemcpy(arrayout_d, arrayin, number * sizeof(unsigned int), cudaMemcpyHostToDevice);

	dim3 block(BLOCKX, BLOCKY);
	dim3 grid((numa + BLOCKX - 1) / BLOCKX, (numb + BLOCKY - 1) / BLOCKY);
	scan_bel <<<grid, block>>>(arrayin_d, loop, arrayout_d, numa, numb);
	cudaDeviceSynchronize();

	cudaMemcpy(arrayout, arrayout_d, number * sizeof(unsigned int), cudaMemcpyDeviceToHost);

	cudaFree(arrayin_d);
	cudaFree(arrayout_d);
}
