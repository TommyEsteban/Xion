/**
* NestNet Group
* Xion Object Detection Framework
* datasetTest.c
* Data for dataset unit testing
*/

#include "datasetTest.h"

void initializeData()
{
    errorMessages = (char *)malloc(sizeof(char) * maxErrorLength);
    arguments = (char **)malloc(sizeof(char *) * maxArguments);

    for(uint i = 0; i < maxArguments; i++)
        arguments[i] = (char *)malloc(sizeof(char) * maxArgumentLength);
    
    strcpy(arguments[0], "xion");
    strcpy(arguments[1], "data");
}

void releaseData()
{
    for(uint i = 0; i < maxArguments; i++)
        free(arguments[i]);

    free(arguments);
    free(errorMessages);
}