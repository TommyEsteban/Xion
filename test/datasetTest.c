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
}

void releaseDatasetTest()
{
    for(uint i = 0; i < MAX_ARGUMENTS; i++)
    {
        //arguments[i][0] = "\0";
        free(arguments[i]);
    }

    free(arguments);
    free(warningMessage);
    free(errorMessage);

    /*if(doesFileExist("data/testing/oneSinglePixelImage/output.x"))
        remove("data/testing/oneSinglePixelImage/output.x");*/
}

void createSampleData(char *posDir, char *negDir, char *posFile, char *negFile, char *dim)
{
    initializeDatasetTest();
    strcpy(arguments[2], posDir);
    strcpy(arguments[3], posFile);
    strcpy(arguments[4], dim);
    dat_createDataset(arguments);
    strcpy(arguments[2], negDir);
    strcpy(arguments[3], negFile);
    dat_createDataset(arguments);
    releaseDatasetTest();
}

void createSampleDataset(char *directoryBase, char *dim)
{
    char *baseFolder = "test3";
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
    dat_createDataset(arguments);
    strcpy(arguments[2], negDir);
    strcpy(arguments[3], outputNeg);
    dat_createDataset(arguments);
    releaseDatasetTest();

    //createSampleData(posDir, negDir, outputPos, outputNeg, dim);

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