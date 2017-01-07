#pragma warning(disable:4819)
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "head.h"
#include "scan.h"
__global__ void prejoin(int *rqa, int *rqb, unsigned int* flag, dint* equal, int stepa, int stepb, int equalsize, int numa, int numb)
{
	int ea, eb;
	bool isMatch = true;
	int k;
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
		flag[x*numb + y] = 1;
	}
	else {
		flag[x*numb + y] = 0;
	}
}

__global__ void result_output(int *rqa, int *rqb, int* res, unsigned int* flag, unsigned int* flag_compute,dint* equal, int stepa, int stepb, int resstep, int numa, int numb)
{
	int k, index, equalid, id;
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if (x >= numa || y >= numb) return;
	int ID = x*numb + y;
	if (flag[ID]) {
		equalid = 0;
		index = 0;
		//printf("(%d,%d)\n", x, y);
		id = ID*resstep;
		for (k = 0; k < stepa + stepb; k++) {
			if (k < stepa) {
				res[flag_compute[ID]*resstep + k] = rqa[x*stepa + k];

				index = k;
			}
			else if (equal[equalid].b != k - stepa) {
				res[flag_compute[ID]*resstep + (++index)] = rqb[y*stepb + (k - stepa)];
			}
			else {
				equalid++;
			}
		}
	}
}

extern "C" void gpuwithscan(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep, int numa, int numb, int equalsize, int* numresgpu)
{
	//allocate memory in gpu and tans the data to it
	int *dev_rqa = 0;
	int *dev_rqb = 0;
	int *dev_res = 0;
	unsigned int *dev_flag = 0;
	unsigned int *dev_flag_compute = 0;
	unsigned int *flag_compute = 0;
	dint *dev_equal = 0;
	cudaError_t cudaStatus;
	cudaSetDevice(0);

	CUDACHECK(cudaMalloc((void**)&dev_rqa, numa*stepa * sizeof(int)));
	CUDACHECK(cudaMalloc((void**)&dev_rqb, numb*stepb * sizeof(int)));
	CUDACHECK(cudaMalloc((void**)&dev_flag, numa*numb * sizeof(unsigned int)));
	CUDACHECK(cudaMalloc((void**)&dev_equal, sizeof(dint)*MAX(numa, numb)));

	CUDACHECK(cudaMemcpy(dev_rqa, rqa, numa*stepa * sizeof(int), cudaMemcpyHostToDevice));
	CUDACHECK(cudaMemcpy(dev_rqb, rqb, numb*stepb * sizeof(int), cudaMemcpyHostToDevice));
	CUDACHECK(cudaMemcpy(dev_equal, equal, sizeof(dint)*MAX(numa, numb), cudaMemcpyHostToDevice));

	dim3 block(BLOCKX, BLOCKY);
	dim3 grid((numa + BLOCKX - 1) / BLOCKX, (numb + BLOCKY - 1) / BLOCKY);
	
	//prejoin
	prejoin<<<grid, block>>>(dev_rqa, dev_rqb, dev_flag, dev_equal, stepa, stepb, equalsize, numa, numb);
	CUDAKERNELCHECK;
	//CUDACHECK(cudaDeviceSynchronize());
	
	//get_result_info
	CUDACHECK(cudaMalloc((void**)&dev_flag_compute, numa*numb * sizeof(unsigned int)));
	get_result_info(dev_flag_compute, dev_flag, numa*numb);
	//CUDACHECK(cudaDeviceSynchronize());

	//allcate the result-- **may be more optimization needed here**
	flag_compute = (unsigned int*)malloc(sizeof(unsigned int)* numa*numb);
	CUDACHECK(cudaMemcpy(flag_compute, dev_flag_compute, numa*numb * sizeof(unsigned int), cudaMemcpyDeviceToHost));
	*numresgpu = flag_compute[numa*numb-1];
	free(flag_compute);
	CUDACHECK(cudaMalloc((void**)&dev_res, (*numresgpu)*resstep * sizeof(int)));
	
	//output the result	
	result_output<<<grid, block>>>(dev_rqa, dev_rqb, dev_res, dev_flag, dev_flag_compute, dev_equal, stepa, stepb, resstep, numa, numb);
	CUDAKERNELCHECK;
	//CUDACHECK(cudaDeviceSynchronize());

	CUDACHECK(cudaMemcpy(res, dev_res, (*numresgpu)*resstep * sizeof(int), cudaMemcpyDeviceToHost));

	cudaFree(dev_flag);
	cudaFree(dev_flag_compute);
	cudaFree(dev_rqa);
	cudaFree(dev_rqb);
	cudaFree(dev_res);
	cudaFree(dev_equal);	
}