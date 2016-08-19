/**
* parallelizer.cu
* Guardian Kids
* NestNet Group
* parallelization functions to speed up algorithms
*/

#include <cuda.h>
#include <cuda_runtime.h>
#include "../../include/cuda/helper_cuda.h"

#include "../../include/dataset.h"
#include "../../include/haarFeature.h"
#include "../../include/haarFeatureGenerator.h"

extern "C" {
#include "../../include/cuda/parallelFunctions.h"
#include "../../include/cuda/mergeSort_common.h"
}

// the number of samples must be multiple of this value
#define MAX_THREADS_PER_BLOCK 32

// variables for debugging
#define FEATURE 10818
#define LOWERID 5543
#define UPPERID 5544
#define BLOCKID 173

// number of samples
unsigned int N;

// dimension of images
unsigned int D;

// the features
HaarFeature *mFeatures;

// device variables
__managed__ HaarFeature *d_f;
float *d_X;
unsigned char *d_Y;
unsigned int *d_i;
float *d_g;
float *d_w;
float *d_s1;
float *d_s0;
float *d_ss1;
float *d_ss0;
float *d_e;
char *d_p;
unsigned int *d_bei;
float *d_BufKey;
unsigned int *d_BufVal;

// host variables
float *h_bs1;
float *h_bs0;
float *h_g;
float *h_errors;
unsigned int *h_bei;
char *h_p;

bool flag = false;
unsigned int blocksPerGrid;
unsigned int fIndex;
unsigned int tIndex;
unsigned int sizef;
unsigned int sizef2;
unsigned int sizei;
unsigned int sizeui;
unsigned int sizeui2;
unsigned int sizeuc;
unsigned int sizec;

__global__ void computeFeatureValuesKernel(
	float *X, 
	float *d_g, 
	unsigned int *d_i, 
	unsigned int n, 
	unsigned int dim, 
	unsigned int featureIndex)
{
	unsigned long int idx = blockDim.x * blockIdx.x + threadIdx.x;

	if(idx < n)
	{
		// initialize the indexes
		d_i[idx] = idx;
		
		float intensityWhite = 0.0;
		float intensityBlack = 0.0;
		
		long unsigned int stride = dim * dim * idx;
		unsigned int count = d_f[featureIndex].numberOfRects;
	
		for(unsigned int i = 0; i < count; i++)
		{
			HaarRectangle r = d_f[featureIndex].rectangles[i];
			
			float d = 0.0, a = 0.0, b = 0.0, c = 0.0;

			if(r.x - 1 >= 0 && r.y - 1 >= 0)
				d = X[stride + (dim * (r.y - 1)) + (r.x - 1)];
			
			if(r.y - 1 >= 0)
				b = X[stride + (dim * (r.y - 1)) + (r.x + r.width - 1)];
			
			if(r.x - 1 >= 0)
				c = X[stride + (dim * (r.y + r.height - 1)) + (r.x - 1)];
	
			a = X[stride + (dim * (r.y + r.height - 1)) + (r.x + r.width - 1)];
		
			if(r.isBlack)
				intensityBlack += (a - b - c + d);
			else
				intensityWhite += (a - b - c + d);
		}
		
		// save the feature value
		d_g[idx] = (intensityWhite - intensityBlack);
	}
	
	//__syncthreads();
}

__global__ void computeWeightScanKernel(
	unsigned char *d_Y, 
	float *d_w, 
	unsigned int *d_i, 
	float *d_s1,
	float *d_s0,
	float *d_ss1,
	float *d_ss0,
	unsigned int blocks, 
	unsigned int fi)
{
	__shared__ float s1[MAX_THREADS_PER_BLOCK];
	__shared__ float s0[MAX_THREADS_PER_BLOCK];
	
	unsigned int tid = threadIdx.x;
	unsigned long int idx = blockDim.x * blockIdx.x + threadIdx.x;
	unsigned int sIndex = d_i[idx];
	
	if(d_Y[sIndex] == 1)
	{
		s1[tid] = d_w[sIndex]; 
		s0[tid] = 0.0;
	}
	
	if(d_Y[sIndex] == 0)
	{
		s0[tid] = d_w[sIndex];
		s1[tid] = 0.0;
	}
	__syncthreads();
	
	//if(fi == FEATURE && idx >= LOWERID && idx < UPPERID) printf("block: %d, tid: %d, sIndex: %d, Y: %d, W: %.12f\n", blockIdx.x, tid, sIndex, d_Y[sIndex], d_w[sIndex]);
	// perform the first iteration to sum values in s1 and s0
	if(tid % 2 == 1)
	{
		s1[tid] = s1[tid] + s1[tid - 1];
		s0[tid] = s0[tid] + s0[tid - 1];
	}
	
	__syncthreads();
	
	// perform the rest of iterations to sum values in s1 and s0
	for(int s = 2; s < blockDim.x; s <<= 1)
	{
		if(tid < s)
			break;
		
		for(int i = s, j = 0, m = s - 1; i < blockDim.x; i++, j++)
		{
			if(tid < i)
				break;
				
			if(tid == i)
			{
				s1[tid] = s1[tid] + s1[m];
				s0[tid] = s0[tid] + s0[m];
			}
			
			if(j == s - 1)
			{
				i += s;
				m += s + s;
				j = -1;
			}
		}
		
		__syncthreads();
	}
	
	__syncthreads();
	
	// initialize the final scan value of the first block to zero, that means the first block of s1 and s0 is ready
	if(blockIdx.x == 0 && tid == blockDim.x - 1)
	{
		d_s1[blockIdx.x] = 0.0;
		d_s0[blockIdx.x] = 0.0;
	}
	
	// save the rest of the last scan value of every block into the global memory
	if(tid == blockDim.x - 1)
	{
		d_s1[blockIdx.x + 1] = s1[tid];
		d_s0[blockIdx.x + 1] = s0[tid];
	}
	
	__syncthreads();
	
	d_ss1[idx] = s1[tid];
	d_ss0[idx] = s0[tid];
	
	__syncthreads();
	
	//f(fi == FEATURE && idx >= LOWERID && idx < UPPERID) printf("(GPU) tid: %d, i: %d,  s1: %.12f, s0: %.12f\n", tid, sIndex, d_ss1[idx], d_ss0[idx]);
}

__global__ void computeErrorKernel(
	float *d_e, 
	unsigned int *d_bei, 
	float *d_s1,
	float *d_s0,
	float *d_ss1,
	float *d_ss0, 
	char *d_p, 
	float t1, 
	float t0, 
	unsigned int fi)
{
	__shared__ float e[MAX_THREADS_PER_BLOCK];
	__shared__ int bei[MAX_THREADS_PER_BLOCK];

	unsigned int tid = threadIdx.x;
	unsigned long int idx = blockDim.x * blockIdx.x + threadIdx.x;
	
	// increment the value given by every block to complete the full scan
	d_ss1[idx] = d_ss1[idx] + d_s1[blockIdx.x];
	d_ss0[idx] = d_ss0[idx] + d_s0[blockIdx.x];
	__syncthreads();

	// compute all possible errors using every posible threshold
	// by consider first, positive samples below the threshold and
	// secondly, negative samples below the threshold 
	float posBelowNegAbove = (d_ss0[idx] + (t1 - d_ss1[idx]));
	float negBelowPosAbove = (d_ss1[idx] + (t0 - d_ss0[idx]));

	if(posBelowNegAbove < negBelowPosAbove)
	{
		d_e[idx] = posBelowNegAbove;
		e[tid] = posBelowNegAbove;
		d_p[idx] = 1;
	}
	else
	{
		d_e[idx] = negBelowPosAbove;
		e[tid] = negBelowPosAbove;
		d_p[idx] = -1;
	}
	
	__syncthreads();
	
	// first iteration to get the index of the lower error
	for(int i = 1; i < blockDim.x; i += 2)
	{
		if(tid == i)
		{
			if(e[tid] > e[tid - 1])
			{
				bei[tid] = tid - 1;
			}
			else
			{
				bei[tid] = tid;
			}
			
			bei[tid - 1] = 0;
		}
	}
	
	__syncthreads();
	
	// subsequent iterations to get the index of the lower error
	for(int s = 4; s <= blockDim.x; s <<= 1)
	{
		for(int i = s - 1; i < blockDim.x; i += s)
		{
			if(tid == i)
			{
				int shift = s / 2;
				
				if(e[bei[tid]] > e[bei[tid - shift]])
				{
					bei[tid] = bei[tid - shift];
				}
				else
				{
					bei[tid] = bei[tid];
				}
				
				bei[tid - shift] = 0;
			}
		}
		
		__syncthreads();
	}
	
	__syncthreads();
	
	if(tid == blockDim.x - 1)
	{
		d_bei[blockIdx.x] = bei[tid];
		//if(fi == FEATURE && blockIdx.x == BLOCKID) printf("blockId: %d, bei[%d]: %d, error: %.12f\n", blockIdx.x, tid, bei[tid], e[bei[tid]]);
	}
	
	__syncthreads();
}

extern "C"
void initParallelization(float *X, UBYTE *Y, unsigned int n, unsigned int d)
{
	N = n;
	D = d;
	
	if(!flag)
		printf("allocating memory in device\n");
	
	checkCudaErrors(cudaDeviceReset());
	
	sizef = N * sizeof(float);
	sizef2 = (N / MAX_THREADS_PER_BLOCK) * sizeof(float);
	sizei = N * sizeof(int);
	sizeui = N * sizeof(unsigned int);
	sizeui2 = (N / MAX_THREADS_PER_BLOCK) * sizeof(unsigned int);
	sizeuc = N * sizeof(UBYTE);
	sizec = N * sizeof(char);
	
	// get generated features
	getFeatures(&mFeatures);

	// allocate features in device memory
	int sizeF = sizeof(HaarFeature) * getFeaturesLength();
	checkCudaErrors(cudaMallocManaged(&d_f, sizeF));

	for(unsigned int i = 0; i < getFeaturesLength(); i++)
	{
		d_f[i].numberOfRects = mFeatures[i].numberOfRects;
		int sizeR = sizeof(HaarRectangle) * mFeatures[i].numberOfRects;
		checkCudaErrors(cudaMalloc((void **)&d_f[i].rectangles, sizeR));
		checkCudaErrors(cudaMemcpy(d_f[i].rectangles, mFeatures[i].rectangles, sizeR, cudaMemcpyHostToDevice));
	}

	printf("Allocated memory for features.\n");
	//getchar();
	// allocate image data in device memory
	unsigned int sizeX = N * D * D * sizeof(float);
	checkCudaErrors(cudaMalloc((void **)&d_X, sizeX));
	checkCudaErrors(cudaMemcpy(d_X, X, sizeX, cudaMemcpyHostToDevice));
	printf("Allocated memory for images data.\n"); 
	//getchar();
	
	// allocate feature values and indexes
	checkCudaErrors(cudaMalloc((void **)&d_g, sizef));
	checkCudaErrors(cudaMalloc((void **)&d_i, sizeui));
	
	// allocate pointers for merge sorting
    checkCudaErrors(cudaMalloc((void **)&d_BufKey, sizef));
    checkCudaErrors(cudaMalloc((void **)&d_BufVal, sizeui));

	// allocate Y, W, S1, S0, e, neccesary for error computing 
	checkCudaErrors(cudaMalloc((void **)&d_Y, sizeuc));
	checkCudaErrors(cudaMalloc((void **)&d_w, sizef));
	checkCudaErrors(cudaMalloc((void **)&d_s1, sizef2));
	checkCudaErrors(cudaMalloc((void **)&d_s0, sizef2));
	checkCudaErrors(cudaMalloc((void **)&d_e, sizef));
	checkCudaErrors(cudaMalloc((void **)&d_p, sizec));
	checkCudaErrors(cudaMalloc((void **)&d_bei, sizeui2));
	checkCudaErrors(cudaMalloc((void **)&d_ss1, sizef));
	checkCudaErrors(cudaMalloc((void **)&d_ss0, sizef));

	checkCudaErrors(cudaMemcpy(d_Y, Y, sizeuc, cudaMemcpyHostToDevice));
	
	// variables to hold the last value of the first scan in s1 and s0
	h_bs1 = (float *)malloc(sizef2);
	h_bs0 = (float *)malloc(sizef2);
	
	// host variable to keep all the possible errors
	h_errors = (float *)malloc(sizef);
	
	// host variable to hold the best error indexes, the lower one per block
	h_bei =(unsigned int *)malloc(sizeui);
	
	// host variable to hold the feature values
	h_g = (float *)malloc(sizef);
	
	// host variable to hold polarities
	h_p = (char *)malloc(sizec);
	
	// check available memory in device
	if(!flag)
	{
		long unsigned int free_byte, total_byte;
		cudaMemGetInfo(&free_byte, &total_byte);
		printf("available memory in device: %lu\n", free_byte);
	}
	
	// compute number of blocks per grid needed
	blocksPerGrid = (N + MAX_THREADS_PER_BLOCK - 1) / MAX_THREADS_PER_BLOCK; 
	
	if(!flag)
	{
		printf("blocks in the grid: %d, threads per block: %d\n", blocksPerGrid, MAX_THREADS_PER_BLOCK);
		flag = true;
	}
}

extern "C"
void closeParallelization()
{
	for(unsigned int i = 0; i < getFeaturesLength(); i++)
		checkCudaErrors(cudaFree(d_f[i].rectangles));
		
	checkCudaErrors(cudaFree(d_f));
	checkCudaErrors(cudaFree(d_X));
	checkCudaErrors(cudaFree(d_Y));
	checkCudaErrors(cudaFree(d_i));
	checkCudaErrors(cudaFree(d_g));
	checkCudaErrors(cudaFree(d_w));
	checkCudaErrors(cudaFree(d_s1));
	checkCudaErrors(cudaFree(d_s0));
	checkCudaErrors(cudaFree(d_e));
	checkCudaErrors(cudaFree(d_bei));
	checkCudaErrors(cudaFree(d_BufKey));
	checkCudaErrors(cudaFree(d_BufVal));
	free(h_bs1);
	free(h_bs0);
	free(h_g);
	free(h_errors);
	free(h_bei);
	free(h_p);
}

extern "C"
void computeFeatureValuesParallel(unsigned int featureIndex, unsigned int iterationIndex)
{	
	fIndex = featureIndex;
	tIndex = iterationIndex;
	computeFeatureValuesKernel<<<blocksPerGrid, MAX_THREADS_PER_BLOCK>>>(d_X, d_g, d_i, N, D, fIndex);
	checkCudaErrors(cudaDeviceSynchronize());

	// copy the result for indexes and feature values
	//checkCudaErrors(cudaMemcpy(*g, d_g, sizef, cudaMemcpyDeviceToHost));
	
	/*if(fIndex == FEATURE)
	{
		for(int i = 9000; i < 9500; i++)
		{
			printf("*****%d: %.12f\n", i, (*g)[i]);
		}
	}*/
}

extern "C"
void sortFeatureValuesParallel()
{
	// sort direction, if ascending 1, decreasing 0
	const unsigned int DIR = 1;
	
	initMergeSort();
	checkCudaErrors(cudaDeviceSynchronize());
	mergeSort(d_g, d_i, d_BufKey, d_BufVal, d_g, d_i, N, DIR);
    checkCudaErrors(cudaDeviceSynchronize());
	closeMergeSort();
}

extern "C"
void computeErrorParallel(float **w, float t1, float t0, float *finalError, 
					float *threshold, char *polarity)
{
	int sharedMemoryAmount = MAX_THREADS_PER_BLOCK * sizeof(float) * 2;
	
	// copy the updated weights to the device and perform scan per block
	checkCudaErrors(cudaMemcpy(d_w, *w, sizef, cudaMemcpyHostToDevice));
	computeWeightScanKernel<<<blocksPerGrid, MAX_THREADS_PER_BLOCK, sharedMemoryAmount>>>(d_Y, d_w, d_i, d_s1, d_s0, d_ss1, d_ss0, blocksPerGrid, fIndex);
	checkCudaErrors(cudaDeviceSynchronize());
	
	// copy the last value of every scan block for s1 and s0
	checkCudaErrors(cudaMemcpy(h_bs1, d_s1, sizef2, cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(h_bs0, d_s0, sizef2, cudaMemcpyDeviceToHost));
	
	// sum every last block value
	for(int b = 2; b < blocksPerGrid; b++)
	{
		h_bs1[b] = h_bs1[b] + h_bs1[b - 1];
		h_bs0[b] = h_bs0[b] + h_bs0[b - 1];
	}
	
	// copy back to the device
	checkCudaErrors(cudaMemcpy(d_s1, h_bs1, sizef2, cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_s0, h_bs0, sizef2, cudaMemcpyHostToDevice));
	
	// compute error
	sharedMemoryAmount = (MAX_THREADS_PER_BLOCK * sizeof(float)) + (MAX_THREADS_PER_BLOCK * sizeof(int));
	computeErrorKernel<<<blocksPerGrid, MAX_THREADS_PER_BLOCK, sharedMemoryAmount>>>(d_e, d_bei, d_s1, d_s0, d_ss1, d_ss0, d_p, t1, t0, fIndex);
	checkCudaErrors(cudaDeviceSynchronize());
	
	// copy feature values, errors, indexes and polarities to the host
	checkCudaErrors(cudaMemcpy(h_g, d_g, sizef, cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(h_errors, d_e, sizef, cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(h_bei, d_bei, sizeui2, cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(h_p, d_p, sizec, cudaMemcpyDeviceToHost));
	
	// get the lower error given by blocks and get the best error index
	*finalError = 7.7;
	unsigned int finalIndex = 0;

	for(int i = 0; i < blocksPerGrid; i++)
	{
		unsigned int idx = h_bei[i] + i * MAX_THREADS_PER_BLOCK;

		if(abs(h_errors[idx]) < *finalError)
		{
			*finalError = h_errors[idx];
			finalIndex = idx;
			*polarity = h_p[idx]; 
		}
	}
	
	*threshold = h_g[finalIndex + 1];
	
	/*if(fIndex == FEATURE)
	{
		float *ss1 = (float *)malloc(sizef);
		float *ss0 = (float *)malloc(sizef);
		unsigned int *ind = (unsigned int *)malloc(sizeui);
		float *gg = (float *)malloc(sizef);
		
		cudaMemcpy(ss1, d_ss1, sizef, cudaMemcpyDeviceToHost);
		cudaMemcpy(ss0, d_ss0, sizef, cudaMemcpyDeviceToHost);
		cudaMemcpy(ind, d_i, sizeui, cudaMemcpyDeviceToHost);
		cudaMemcpy(gg, d_g, sizef, cudaMemcpyDeviceToHost);
		
		for(int i = 5000; i < 5550; i++)
			printf("%d: %d, g: %.12f\n", i, ind[i], gg[i]);
	
		for(int i = LOWERID; i < UPPERID; i++)
			printf("(CPU) %d, s1:%.12f, s0:%.12f\n", i, ss1[i], ss0[i]);
			
		free(ss1); free(ss0); free(ind);
		//char c = getchar(); c++;
	}*/

	/*if(fIndex == FEATURE)
	{
		//printf("finalError: %.6f\n", *finalError);
		//printf("finalIndex: %d\n", *finalIndex);
		float ee = 7;
		unsigned int ei = 0;
		
		for(int i = 0; i < N; i++){
			if(errors[i] < ee){
				ee = errors[i];
				ei = i;
			}
		}
		
		printf("The truth -> bestIndex: %d (%d), bestError: %.12f (%.12f)\n", ei, *finalIndex, ee, *finalError);
		//char c = getchar(); c++;
	}*/
}