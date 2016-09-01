/**
* NestNet Group
* Xion Object Detection Framework
* datasetController.h
* controller for all dataset related tasks
*/

#ifndef DATASET_CONTROLLER_H
#define DATASET_CONTROLLER_H

#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include "xionTypes.h"
#include "xionMessages.h"
#include "xionio.h"
#include "datasetValidator.h"
#include "dataset.h"

bool dat_createDataset(char **arguments);
BinaryDataset *dat_loadDataset(char **arguments);

#endif