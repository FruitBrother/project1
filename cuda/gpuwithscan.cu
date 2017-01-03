#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "head.h"

extern "C" void gpuwithscan(int *rqa, int *rqb, int* res, dint* equal, int stepa, int stepb, int resstep, int numa, int numb, int equalsize)
{

}