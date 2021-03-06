#pragma warning(disable:4819)
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "head.h"
__global__ void join(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep, int equalsize, int numa, int numb)
{
	int ea, eb;
	bool isMatch = true;
	int k, index, equalid, id;
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
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
		//printf("(%d,%d)\n", x, y);
		id = (x*numb + y)*resstep;
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

extern "C" void gpu(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep, int numa, int numb, int equalsize) {
	//allocate memory in gpu and tans the data to it
	int *dev_rqa = 0;
	int *dev_rqb = 0;
	int *dev_res = 0;
	dint *dev_equal = 0;
	cudaSetDevice(0);
	cudaError_t cudaStatus;

	CUDACHECK(cudaMalloc((void**)&dev_rqa, numa*stepa * sizeof(int)));
	CUDACHECK(cudaMalloc((void**)&dev_rqb, numb*stepb * sizeof(int)));
	CUDACHECK(cudaMalloc((void**)&dev_res, numa*numb*resstep * sizeof(int)));
	CUDACHECK(cudaMalloc((void**)&dev_equal, sizeof(dint)*MAX(stepa, stepb)));

	CUDACHECK(cudaMemcpy(dev_rqa, rqa, numa*stepa * sizeof(int), cudaMemcpyHostToDevice));
	CUDACHECK(cudaMemcpy(dev_rqb, rqb, numb*stepb * sizeof(int), cudaMemcpyHostToDevice));
	CUDACHECK(cudaMemcpy(dev_equal, equal, sizeof(dint)*MAX(stepa, stepb), cudaMemcpyHostToDevice));

	dim3 block(BLOCKX, BLOCKY);
	dim3 grid((numa + BLOCKX - 1) / BLOCKX, (numb + BLOCKY - 1) / BLOCKY);
	join <<<grid,block>>> (dev_rqa, dev_rqb, dev_res, dev_equal, stepa, stepb, resstep, equalsize, numa, numb);
	CUDAKERNELCHECK;
	CUDACHECK(cudaDeviceSynchronize());
	
	CUDACHECK(cudaMemcpy(res, dev_res, numa*numb*resstep * sizeof(int), cudaMemcpyDeviceToHost));

	cudaFree(dev_rqa);
	cudaFree(dev_rqb);
	cudaFree(dev_res);
	cudaFree(dev_equal);
}