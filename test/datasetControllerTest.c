/**
* NestNet Group
* Xion Object Detection Framework
* xionControllerTest.c
* unit tests for dataset controller
*/

#include "xionTest.h"
#include "datasetTest.h"

/*char *errorMessages;
char **arguments;
cuint maxArguments = 5;
cuint maxArgumentLength = 2048;
cuint maxErrorLength = 1024;

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
}*/

int createDataset_notExistingFolder_returnFalseAndMessage()
{
    // arrange
    initializeData();
    strcpy(arguments[2], "notexistingpath");

    // act
    bool isValid = dat_createDataset(arguments, errorMessages);

    // assert
    if(!isValid && strcmp(NOT_FOUND_DIRECTORY, errorMessages) == 0)
    {
        releaseData();
        return EXIT_SUCCESS;
    }

    releaseData();
    return EXIT_FAILURE;
}