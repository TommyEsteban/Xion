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