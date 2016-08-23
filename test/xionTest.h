/**
* NestNet Group
* Xion Object Detection Framework
* dataset.h
* generation and load of datasets
*/

#ifndef XION_TEST_H
#define XION_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/xionTypes.h"
#include "../include/dataset.h"
#include "../include/datasetController.h"
#include "../include/datasetValidator.h"

int datasetValidator_folderPathNull_returnFalseAndMessage();
int datasetValidator_folderPathMaxLength_returnFalseAndMessage();
int datasetValidator_destinationFileNull_returnFalseAndMessage();
int datasetValidator_destinationFileMaxLength_returnFalseAndMessage();
int datasetValidator_dimensionMaxLength_returnFalseAndMessage();
int datasetValidator_dimensionNotNumber_returnFalseAndMessage();
int datasetValidator_dimensionZero_returnFalseAndMessage();
int datasetValidator_dimensionGreaterThanZero_returnTrue();
int createDataset_notExistingFolder_returnFalseAndMessage();

#endif