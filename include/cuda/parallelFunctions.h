/**
* parallelFunctions.h
* Guardian Kids
* NestNet Group
* parallel functions to speed up algorithms
*/

#ifndef PARALLELFUNCTIONS_H
#define PARALLELFUNCTIONS_H

#include <stdio.h>
#include <math.h>

typedef unsigned char UBYTE;

#ifdef __cplusplus
extern "C"
#endif
void initParallelization(float *X, UBYTE *Y, unsigned int n, unsigned int d);

#ifdef __cplusplus
extern "C"
#endif
void computeFeatureValuesParallel(unsigned int featureIndex, unsigned int iterationIndex);

#ifdef __cplusplus
extern "C"
#endif
void sortFeatureValuesParallel();

#ifdef __cplusplus
extern "C"
#endif
void computeErrorParallel(float **w, float t1, float t0, float *finalError, 
							float *threshold, char *polarity);

#ifdef __cplusplus
extern "C"
#endif
void closeParallelization();

#endif