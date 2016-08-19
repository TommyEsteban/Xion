/**
* NestNet Group
* Xion Object Detection Framework
* datasetController.c
* controller for all dataset related tasks
*/

#include "../../include/datasetController.h"

bool dat_createDataset(char **arguments, char *errorMessage)
{
    DIR *dir = opendir(arguments[2]);

    if (ENOENT == errno)
    {
        errorMessage = NOT_FOUND_DIRECTORY;
        free(dir);
        return false;
    }

    free(dir);
    
    uint imgSize = atoi(arguments[4]);
    dat_createInFile(arguments[2], arguments[3], imgSize);
    return true;
}