/**
* NestNet Group
* Xion Object Detection Framework
* datasetTest.c
* Data for dataset unit testing
*/

#include "datasetTest.h"

void initializeDatasetTest()
{
    errorMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    arguments = (char **)malloc(sizeof(char *) * MAX_ARGUMENTS);

    for(uint i = 0; i < MAX_ARGUMENTS; i++)
        arguments[i] = (char *)malloc(sizeof(char) * MAX_ARGUMENT_LENGTH);
    
    strcpy(arguments[0], "xion");
    strcpy(arguments[1], "data");
}

void releaseDatasetTest()
{
    for(uint i = 0; i < MAX_ARGUMENTS; i++)
        free(arguments[i]);

    free(arguments);
    free(errorMessage);
}