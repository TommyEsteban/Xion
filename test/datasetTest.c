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
    warningMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
    arguments = (char **)malloc(sizeof(char *) * MAX_ARGUMENTS);

    for(uint i = 0; i < MAX_ARGUMENTS; i++)
        arguments[i] = (char *)malloc(sizeof(char) * MAX_ARGUMENT_LENGTH);
    
    strcpy(arguments[0], "xion");
    strcpy(arguments[1], "data");
    strcpy(arguments[2], "folder");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "1");
    strcpy(arguments[5], "1");    
}

void releaseDatasetTest()
{
    for(uint i = 0; i < MAX_ARGUMENTS; i++)
    {
        free(arguments[i]);
    }

    free(arguments);
    free(warningMessage);
    free(errorMessage);
}

void createSampleDataset(char *directoryBase, char *dim)
{
    outputPos = (char *)malloc(sizeof(char) * 1024);
    outputNeg = (char *)malloc(sizeof(char) * 1024);
    posDir = (char *)malloc(sizeof(char) * 1024);
    negDir = (char *)malloc(sizeof(char) * 1024);
    strcpy(outputPos, "data/testing/");
    strcpy(outputNeg, "data/testing/");
    strcpy(posDir, "data/testing/");
    strcpy(negDir, "data/testing/");
    strcat(outputPos, directoryBase);
    strcat(outputPos, "/pos.x");
    strcat(outputNeg, directoryBase);
    strcat(outputNeg, "/neg.x");
    strcat(posDir, directoryBase);
    strcat(posDir, "/pos");
    strcat(negDir, directoryBase);
    strcat(negDir, "/neg");

    initializeDatasetTest();
    strcpy(arguments[2], posDir);
    strcpy(arguments[3], outputPos);
    strcpy(arguments[4], dim);
    strcpy(arguments[5], dim);
    dat_createDataset(arguments);
    strcpy(arguments[2], negDir);
    strcpy(arguments[3], outputNeg);
    dat_createDataset(arguments);
    releaseDatasetTest();

    initializeDatasetTest();
    strcpy(arguments[2], outputPos);
    strcpy(arguments[3], outputNeg);
    strcpy(arguments[4], dim);
}

void releaseSampleDataset()
{
    releaseDatasetTest();
    remove(outputPos);
    remove(outputNeg);
    free(outputPos); 
    free(outputNeg); 
    free(posDir); 
    free(negDir);
}