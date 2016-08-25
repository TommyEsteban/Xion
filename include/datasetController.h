/**
* NestNet Group
* Xion Object Detection Framework
* datasetController.h
* controller for all dataset related tasks
*/

#ifndef DATASET_CONTROLLER_H
#define DATASET_CONTROLLER_H

#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include "xionTypes.h"
#include "xionMessages.h"
#include "dataset.h"

bool dat_createDataset(char **arguments);

#endif