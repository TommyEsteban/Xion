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
    warningMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    imageName = NULL;
    image = NULL;
    integral = NULL;
    squared = NULL;
    normalization = NULL;
}

void initializePreproTest(int n)
{
    errorMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    warningMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    imageName = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    image = (ubyte *)malloc(sizeof(ubyte) * n);
    integral = (int *)malloc(sizeof(int) * n);
    squared = (int *)malloc(sizeof(int) * n);
    normalization = (float *)malloc(sizeof(float) * n);
}

void releasePreproTest()
{
    free(imageName);
    free(image);
    free(integral);
    free(squared);
    free(normalization);
    free(warningMessage);
    free(errorMessage);
}