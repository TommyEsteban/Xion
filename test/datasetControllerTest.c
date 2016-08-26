/**
* NestNet Group
* Xion Object Detection Framework
* xionControllerTest.c
* unit tests for dataset controller
*/

#include "xionTest.h"
#include "datasetTest.h"

int createDataset_notExistingFolder_returnFalseAndErrorMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "notExistingDirectory");

    // act
    bool isValid = dat_createDataset(arguments);

    // assert
    if(!isValid && strcmp(NOT_FOUND_DIRECTORY, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int createDataset_notExistingDestinationFolder_returnFalseAndErrorMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "data/testing/test1");
    strcpy(arguments[3], "data/testing/notExistingFolder/output.x");
    strcpy(arguments[4], "1");

    // act
    bool isValid = dat_createDataset(arguments);

    // assert
    if(!isValid && strcmp(NOT_FOUND_DIRECTORY, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int createDataset_invalidDimension_returnFalseAndErrorMessage()
{
    // arrange
    initializeDatasetTest();
    cbyte *outputFile = "data/testing/test1/output.x";
    strcpy(arguments[2], "data/testing/test1");
    strcpy(arguments[3], outputFile);
    strcpy(arguments[4], "s");

    // act
    bool isValid = dat_createDataset(arguments);

    // assert
    if(!isValid && strcmp(INVALID_DIMENSION, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int createDataset_oneSinglePixelImage_fileCorrectlyCreated()
{
    // arrange
    initializeDatasetTest();
    cbyte *outputFile = "data/testing/test1/output.x";
    strcpy(arguments[2], "data/testing/test1");
    strcpy(arguments[3], outputFile);
    strcpy(arguments[4], "1");

    // act
    bool isValid = dat_createDataset(arguments);

    // assert
    if(isValid && doesFileExist(outputFile))
    {
        releaseDatasetTest();
        remove(outputFile);
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int loadDataset_onePositiveOneNegative_valuesCorrectlyLoaded()
{
    // arrange
    char *outputPos = "data/testing/test2/pos.x";
    char *outputNeg = "data/testing/test2/neg.x";
    char *posDir = "data/testing/test2/pos";
    char *negDir = "data/testing/test2/neg";
    createSampleData(posDir, negDir, outputPos, outputNeg, "1");

    initializeDatasetTest();
    strcpy(arguments[2], outputPos);
    strcpy(arguments[3], outputNeg);
    strcpy(arguments[4], "1");
    
    // act
    BinaryDataset *dataset = dat_loadDataset(arguments);

    // assert
    if(dataset != NULL && dataset->n == 2 &&
        dataset->n0 == 1 && dataset->n1 == 1 &&
        dataset->Y[0] == 1 && dataset->Y[1] == 0 &&
        dataset->X[0] == 0 && dataset->X[1] == 0)
    {
        releaseDatasetTest();
        dat_close(dataset);
        remove(outputPos);
        remove(outputNeg);
        remove(posDir);
        remove(negDir);
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    dat_close(dataset);
    remove(outputPos);
    remove(outputNeg);
    remove(posDir);
    remove(negDir);
    return EXIT_FAILURE;
}

int loadDataset_one4X4PositiveOne4X4Negative_valuesCorrectlyLoaded()
{
    // arrange
    char *outputPos = "data/testing/test3/pos.x";
    char *outputNeg = "data/testing/test3/neg.x";
    char *posDir = "data/testing/test3/pos";
    char *negDir = "data/testing/test3/neg";
    createSampleData(posDir, negDir, outputPos, outputNeg, "2");

    initializeDatasetTest();
    strcpy(arguments[2], outputPos);
    strcpy(arguments[3], outputNeg);
    strcpy(arguments[4], "2");
    
    // act
    BinaryDataset *dataset = dat_loadDataset(arguments);

    // assert
    if(dataset != NULL && dataset->d == 2 && dataset->p == 4 && 
        dataset->n == 2 && dataset->n0 == 1 && dataset->n1 == 1 &&
        dataset->Y[0] == 1 && dataset->Y[1] == 0 &&
        dataset->X[0] > -0.64825202 && dataset->X[0] < -0.64825200 && 
        dataset->X[1] > 0.653792023 && dataset->X[1] < 0.653792025 &&
        dataset->X[2] > -0.64825202 && dataset->X[2] < -0.64825200 && 
        dataset->X[3] > 0.653792023 && dataset->X[3] < 0.653792025 &&
        dataset->X[4] > 0.646086990 && dataset->X[4] < 0.646086992 &&
        dataset->X[5] > -0.646086992 && dataset->X[5] < -0.646086990 &&
        dataset->X[6] > 0.646086990 && dataset->X[6] < 0.646086992 && 
        dataset->X[7] > -0.646086992 && dataset->X[7] < -0.646086990) 
    {
        releaseDatasetTest();
        dat_close(dataset);
        remove(outputPos);
        remove(outputNeg);
        remove(posDir);
        remove(negDir);
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    dat_close(dataset);
    remove(outputPos);
    remove(outputNeg);
    remove(posDir);
    remove(negDir);
    return EXIT_FAILURE;
}