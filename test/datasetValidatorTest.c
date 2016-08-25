/**
* NestNet Group
* Xion Object Detection Framework
* datasetTestValidator.c
* unit testing for dataset validator
*/

#include "xionTest.h"
#include "datasetTest.h"

int datasetValidator_folderPathNull_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "");

    // act
    bool isValid = dat_isValid(arguments);
    
    // assert
    if(!isValid && strcmp(FIRST_ARG_NAME_OF_FOLDER, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_folderPathMaxLength_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], LONG_DATA_SAMPLE);

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(ARGUMENT_EXCEEDS, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }
    
    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_destinationFileNull_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "");

    // act
    bool isValid = dat_isValid(arguments);
    
    // assert
    if(!isValid && strcmp(SECOND_ARG_NAME_OF_FILE, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_destinationFileMaxLength_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], LONG_DATA_SAMPLE);

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(ARGUMENT_EXCEEDS, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }
    
    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionNull_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "");

    // act
    bool isValid = dat_isValid(arguments);
    
    // assert
    if(!isValid && strcmp(THIRD_ARG_DIMENSION, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionMaxLength_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], LONG_DATA_SAMPLE);

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(ARGUMENT_EXCEEDS, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }
    
    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionNotNumber_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "dimension");

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(THIRD_ARG_POSITIVE_NUMBER, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionZero_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "0");

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(THIRD_ARG_POSITIVE_NUMBER, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionGreaterThanZero_returnTrue()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "1");

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(isValid)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    return EXIT_FAILURE;
}