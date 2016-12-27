#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <time.h>
#define MAX(a,b) (a<b)?b:a
struct dint {
	int a, b;
};

extern "C" void gpu(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep) {
	//allocate memory in gpu and tans the data to it
	int *dev_rqa = 0;
	int *dev_rqb = 0;
	int *dev_res = 0;
	int *dev_equal = 0;
	cudaSetDevice(0);
	cudaMalloc((void**)&dev_rqa, numa*stepa * sizeof(int));
	cudaMalloc((void**)&dev_rqb, numb*stepb * sizeof(int));
	cudaMalloc((void**)&dev_equal, sizeof(dint)*MAX(numa, numb));

	cudaMemcpy(dev_rqa, rqa, numa*stepa * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_rqb, rqb, numb*stepb * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_res, res, numa*numb*resstep * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_equal, equal, sizeof(dint)*MAX(numa, numb), cudaMemcpyHostToDevice);

	dim3 block(16, 16);
	dim3 grid((numa + 15) / 16, (numb + 15) / 16);
	join <<<grid, block >>> (dev_rqa, dev_rqb, dev_res, dev_equal, stepa, stepb, resstep, equalsize, numa, numb);
	cudaDeviceSynchronize();
	cudaMemcpy(res, dev_res, numa*numb*resstep * sizeof(int), cudaMemcpyDeviceToHost);

	cudaFree(dev_rqa);
	cudaFree(dev_rqb);
	cudaFree(dev_res);
	cudaFree(dev_equal);
}


__global__ void join(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep, int equalsize, int numa, int numb)
{	int ea, eb;
	bool isMatch = true;
	int k, index, equalid, id;
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockIdx.y + threadIdx.y;
	if (x >= numa || y >= numb) return;

	for (k = 0; k < equalsize; k++) {
		ea = equal[k].a + x*stepa;
		eb = equal[k].b + y*stepb;
		if (rqa[ea] != rqb[eb]) {
			isMatch = false;
			break;
		}
	}
	if (isMatch) {
		equalid = 0;
		index = 0;
		id = y*resstep + x;
		for (k = 0; k < stepa + stepb; k++) {
			if (k < stepa) {
				res[id + k] = rqa[x*stepa + k];
				index = k;
			}
			else if (equal[equalid].b != k - stepa) {
				res[id + (++index)] = rqb[y*stepb + (k - stepa)];
			}
			else {
				equalid++;
			}
		}
	}
}


// Helper function for using CUDA to add vectors in parallel.
void addWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;

	cudaSetDevice(0);

	cudaMalloc((void**)&dev_c, size * sizeof(int));
	cudaMalloc((void**)&dev_a, size * sizeof(int));
    cudaMalloc((void**)&dev_b, size * sizeof(int));

    cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);

    addKernel<<<1, size>>>(dev_c, dev_a, dev_b);

    cudaGetLastError();
    cudaDeviceSynchronize();
    cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);


Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
}
