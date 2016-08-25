/**
* NestNet Group
* Xion Object Detection Framework
* datasetValidator.c
* validates data passed to generate datasets
*/

#include "../../include/datasetValidator.h"

bool dat_isValid(char **arguments)
{
    // check the folder path is not null
    if(arguments[2][0] == '\0')
    {
        strcpy(errorMessage, FIRST_ARG_NAME_OF_FOLDER);
        return false;
    }
    
    // check the first argument does not exceeds the max number of characters allowed
    if(strlen(arguments[2]) > MAX_PATH_CHARACTERS)
    {
        strcpy(errorMessage, ARGUMENT_EXCEEDS);
        return false;
    }

    // check the destination file name is not null
    if(arguments[3][0] == '\0')
    {
        strcpy(errorMessage, SECOND_ARG_NAME_OF_FILE);
        return false;
    }
    
    // check the second argument does not exceeds the max number of characters allowed
    if(strlen(arguments[3]) > MAX_PATH_CHARACTERS)
    {
        strcpy(errorMessage, ARGUMENT_EXCEEDS);
        return false;
    }

    // check the dimension is not null
    if(arguments[4][0] == '\0')
    {
        strcpy(errorMessage, THIRD_ARG_DIMENSION);
        return false;
    }

    // check the dimension does not exceeds the max number of characters allowed
    if(strlen(arguments[4]) > MAX_PATH_CHARACTERS)
    {
        strcpy(errorMessage, ARGUMENT_EXCEEDS);
        return false;
    }

    // check valid dimension, it must be equal or greater than zero
    if(atoi(arguments[4]) <= 0)
    {
        strcpy(errorMessage, THIRD_ARG_POSITIVE_NUMBER);
        return false;
    }

    return true;
}