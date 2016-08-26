/**
* NestNet Group
* Xion Object Detection Framework
* datasetController.c
* controller for all dataset related tasks
*/

#include "../../include/datasetController.h"

bool dat_createDataset(char **arguments)
{
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

    // check that dimension is a positive number
    uint imgSize = atoi(arguments[4]);

    if(imgSize <= 0)
    {
        strcpy(errorMessage, INVALID_DIMENSION);
        return false;
    }

    dat_createInFile(arguments[2], arguments[3], imgSize);
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
    uint imgSize = atoi(arguments[4]);

    if(imgSize <= 0)
    {
        strcpy(errorMessage, INVALID_DIMENSION);
        return NULL;
    }

    return dat_loadFromFiles(arguments[2], arguments[3], imgSize);
}

void dat_closeDataset()
{

}