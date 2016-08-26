/**
* NestNet Group
* Xion Object Detection Framework
* preprocessingTest.h
* Data for preprocessing unit testing
*/

#ifndef PREPROCESSING_TEST_H
#define PREPROCESSING_TEST_H

#include "xionTest.h"

char *imageName;
ubyte *image;
int *integral;
int *squared;
float *normalization;

void initializePreproTestNull();

void initializePreproTest(int n);

void releasePreproTest();

#endif