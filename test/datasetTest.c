/**
* NestNet Group
* Xion Object Detection Framework
* main.c
* main entry point for Xion Unit Tests
*/

#include "xionTest.h"

char *errorMessages;
char **arguments;
cuint maxArguments = 5;
cuint maxArgumentLength = 2048;
cuint maxErrorLength = 1024;

const char *longDataSample = "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm";

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

int datasetValidator_folderPathNull_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "");

    // act
    bool isValid = dat_isValid(arguments, errorMessages);
    
    // assert
    if(!isValid && strcmp(FIRST_ARG_NAME_OF_FOLDER, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }

    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_folderPathMaxLength_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], longDataSample);

    // act
    bool isValid = dat_isValid(arguments, errorMessages);

    // assert
    if(!isValid && strcmp(ARGUMENT_EXCEEDS, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }
    
    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_destinationFileNull_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "");

    // act
    bool isValid = dat_isValid(arguments, errorMessages);
    
    // assert
    if(!isValid && strcmp(SECOND_ARG_NAME_OF_FILE, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }

    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_destinationFileMaxLength_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], longDataSample);

    // act
    bool isValid = dat_isValid(arguments, errorMessages);

    // assert
    if(!isValid && strcmp(ARGUMENT_EXCEEDS, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }
    
    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionNull_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "");

    // act
    bool isValid = dat_isValid(arguments, errorMessages);
    
    // assert
    if(!isValid && strcmp(THIRD_ARG_DIMENSION, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }

    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionMaxLength_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], longDataSample);

    // act
    bool isValid = dat_isValid(arguments, errorMessages);

    // assert
    if(!isValid && strcmp(ARGUMENT_EXCEEDS, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }
    
    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionNotNumber_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "dimension");

    // act
    bool isValid = dat_isValid(arguments, errorMessages);

    // assert
    if(!isValid && strcmp(THIRD_ARG_POSITIVE_NUMBER, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }

    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionZero_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "0");

    // act
    bool isValid = dat_isValid(arguments, errorMessages);

    // assert
    if(!isValid && strcmp(THIRD_ARG_POSITIVE_NUMBER, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }

    releaseData();
    return EXIT_FAILURE;
}

int datasetValidator_dimensionGreaterThanZero_returnTrue()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "path");
    strcpy(arguments[3], "file");
    strcpy(arguments[4], "1");

    // act
    bool isValid = dat_isValid(arguments, errorMessages);

    // assert
    if(isValid)
    {
        releaseData();
        return EXIT_SUCCESS;
    }

    releaseData();
    return EXIT_FAILURE;
}