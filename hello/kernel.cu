#include <iostream>
#include <math.h>
#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
using namespace std;

__global__ void scan_bel(int* inputarray, int loop, int* outputarray, int number)
{
	unsigned int thIdx = blockIdx.x * blockDim.x + threadIdx.x;

	int divisor = 2;
	int adder = 1;
	int temp;
	for (int i = 0; i<loop; i++)
	{
		if (thIdx % (divisor) == divisor - 1)
		{
			outputarray[thIdx] = outputarray[thIdx - adder] + outputarray[thIdx];
			if (thIdx == 255)
				printf("outputarray[%d] = %d\n", thIdx, outputarray[thIdx]);
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
			//printf("outputarray[%d] = %d | ", thIdx, outputarray[thIdx]);
			outputarray[thIdx - adder] = temp;
			//printf("outputarray[%d] = %d\n", thIdx - adder, temp);
		}
		__syncthreads();
		divisor /= 2;
		adder /= 2;
	}
}
int main()
{
	int number = 256;
	int* arrayin;
	int* arrayin_d;
	int* arrayout;
	int* arrayout_d;


	arrayin = new int[number];// {1, 1, 0, 1, 0, 0, 0, 1};
	arrayout = new int[number];

	for (int i = 0; i<number; i++)
	{
		arrayin[i] = i;
	}

	int loop;
	loop = (int)log2f(number);

	cudaMalloc((void**)&arrayin_d, number * sizeof(int));
	cudaMalloc((void**)&arrayout_d, number * sizeof(int));

	cudaMemcpy(arrayin_d, arrayin, number * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(arrayout_d, arrayin, number * sizeof(int), cudaMemcpyHostToDevice);

	scan_bel <<<1, number>>>(arrayin_d, loop, arrayout_d, number);

	cudaMemcpy(arrayout, arrayout_d, number * sizeof(int), cudaMemcpyDeviceToHost);

	for (int i = 0; i<number; i++)
	{
		cout << arrayout[i] << endl;
	}
	delete[] arrayout;
	//delete[] arrayin;

	cudaFree(arrayin_d);
	cudaFree(arrayout_d);

	system("pause");

	return 0;
}