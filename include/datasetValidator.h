/**
* NestNet Group
* Xion Object Detection Framework
* datasetValidator.h
* validates data passed to generate datasets
*/

#ifndef DATASET_VALIDATOR_H
#define DATASET_VALIDATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "xionTypes.h"
#include "xionMessages.h"

#define MAX_PATH_CHARACTERS 1024

bool dat_isValid(char **arguments, char *errorMessage);

#endif