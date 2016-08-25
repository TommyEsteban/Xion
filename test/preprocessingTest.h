/**
* NestNet Group
* Xion Object Detection Framework
* preprocessingTest.h
* Data for preprocessing unit testing
*/

#ifndef PREPROCESSING_TEST_H
#define PREPROCESSING_TEST_H

#include "xionTest.h"

ubyte *image;
int *integral;
int *squared;

void initializePreproTestNull();

void initializePreproTest(int n);

void releasePreproTest();

#endif