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
    printf("FAILED: datasetValidator_folderPathNull_returnFalseAndMessage");
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
    printf("FAILED: datasetValidator_folderPathMaxLength_returnFalseAndMessage");
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
    printf("FAILED: datasetValidator_destinationFileNull_returnFalseAndMessage");
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
    printf("FAILED: datasetValidator_destinationFileMaxLength_returnFalseAndMessage");
    return EXIT_FAILURE;
}

int datasetValidator_widthNull_returnFalseAndMessage()
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
    printf("FAILED: datasetValidator_widthNull_returnFalseAndMessage");
    return EXIT_FAILURE;
}

int datasetValidator_widthMaxLength_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], LONG_DATA_SAMPLE);
    strcpy(arguments[5], LONG_DATA_SAMPLE);

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(ARGUMENT_EXCEEDS, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }
    
    releaseDatasetTest();
    printf("FAILED: datasetValidator_widthMaxLength_returnFalseAndMessage");
    return EXIT_FAILURE;
}

int datasetValidator_widthNotNumber_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "dimension");
    strcpy(arguments[5], "dimension");

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(THIRD_ARG_POSITIVE_NUMBER, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    printf("FAILED: datasetValidator_widthNotNumber_returnFalseAndMessage");
    return EXIT_FAILURE;
}

int datasetValidator_widthZero_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "0");
    strcpy(arguments[5], "0");

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(!isValid && strcmp(THIRD_ARG_POSITIVE_NUMBER, errorMessage) == 0)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    printf("FAILED: datasetValidator_widthZero_returnFalseAndMessage");
    return EXIT_FAILURE;
}

int datasetValidator_widthGreaterThanZero_returnTrue()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "1");
    strcpy(arguments[5], "1");

    // act
    bool isValid = dat_isValid(arguments);

    // assert
    if(isValid)
    {
        releaseDatasetTest();
        return EXIT_SUCCESS;
    }

    releaseDatasetTest();
    printf("FAILED: datasetValidator_widthGreaterThanZero_returnTrue");
    return EXIT_FAILURE;
}