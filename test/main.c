/**
* NestNet Group
* Xion Object Detection Framework
* main.c
* main entry point for Xion Unit Tests
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "xionTest.h"

int datasetTestSuite();

int main (int argc, char **argv)
{
    if(strcmp(argv[1], "dataset") == 0)
        return datasetTestSuite();
    
    //printf("arg 1: %s\n", argv[1]);
    return EXIT_FAILURE;
}

int datasetTestSuite()
{
    if(datasetValidator_folderPathNull_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_folderPathMaxLength_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_destinationFileNull_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_destinationFileMaxLength_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(datasetValidator_dimensionMaxLength_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_dimensionNotNumber_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_dimensionZero_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(datasetValidator_dimensionGreaterThanZero_returnTrue() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}