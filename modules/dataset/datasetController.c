/**
* NestNet Group
* Xion Object Detection Framework
* datasetController.c
* controller for all dataset related tasks
*/

#include "../../include/datasetController.h"

bool dat_createDataset(char **arguments)
{
    // check arguments
    if(!dat_isValid(arguments))
        return false;

    // check the first argument is an existing directory
    if(!doesDirectoryExist(arguments[2]))
        return false;

    // check the destination file directory exists
    char *outputFile = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    strcpy(outputFile, arguments[3]);
    
    if(!doesDirectoryExist(dirname(outputFile)))
    {
        free(outputFile);
        return false;
    }
    
    free(outputFile);

    uint width = atoi(arguments[4]);
    uint height = atoi(arguments[5]);

    dat_createInFile(arguments[2], arguments[3], width, height);
    return true;
}

BinaryDataset *dat_loadDataset(char **arguments)
{
    // check the first file
    if(!doesFileExist(arguments[2]))
        return NULL;
    
    // check the second file
    if(!doesFileExist(arguments[3]))
        return NULL;
    
    // check that dimension is a positive number
    uint width = atoi(arguments[4]);
    uint height = atoi(arguments[5]);

    if(width <= 0 || height <= 0)
    {
        strcpy(errorMessage, INVALID_DIMENSION);
        return NULL;
    }

    return dat_loadFromFiles(arguments[2], arguments[3], width, height);
}
