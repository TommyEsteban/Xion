/**
* NestNet Group
* Xion Object Detection Framework
* xionio.c
* performs I/O operations
*/

#include "../../include/xionio.h"

bool doesDirectoryExist(char *directoryPath)
{
    DIR *dir = opendir(directoryPath);

    if (ENOENT == errno)
    {
        strcpy(errorMessage, NOT_FOUND_DIRECTORY);
        printf("ERROR: not found directory %s\n", directoryPath);
        closedir(dir);
        return false;
    }

    closedir(dir);
    return true;
    
}

bool doesFileExist(cbyte* filename)
{
    FILE* fptr = fopen(filename, "r");

    if (fptr != NULL)
    {
        fclose(fptr);
        return true;
    }

    printf("File doesnt exist: %s\n", filename);
    return false;
}