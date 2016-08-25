/**
* NestNet Group
* Xion Object Detection Framework
* preprocessingTest.c
* Data for preprocessing unit testing
*/

#include "preprocessingTest.h"

void initializePreproTestNull()
{
    errorMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    image = NULL;
    integral = NULL;
    squared = NULL;
}

void initializePreproTest(int n)
{
    errorMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    image = (ubyte *)malloc(sizeof(ubyte) * n);
    integral = (int *)malloc(sizeof(int) * n);
    squared = (int *)malloc(sizeof(int) * n);
}

void releasePreproTest()
{
    free(image);
    free(integral);
    free(squared);
}