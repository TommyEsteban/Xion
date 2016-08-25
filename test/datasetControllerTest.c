/**
* NestNet Group
* Xion Object Detection Framework
* xionControllerTest.c
* unit tests for dataset controller
*/

#include "xionTest.h"
#include "datasetTest.h"

int createDataset_notExistingFolder_returnFalseAndMessage()
{
    // arrange
    initializeDatasetTest();
    strcpy(arguments[2], "notexistingpath");

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