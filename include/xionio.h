/**
* NestNet Group
* Xion Object Detection Framework
* xionio.h
* I/O operations
*/

#ifndef XIONIO_H
#define XIONIO_H

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include "xionTypes.h"
#include "xionMessages.h"

bool doesDirectoryExist(char *directoryPath);
bool doesFileExist(cbyte* filename);

#endif
