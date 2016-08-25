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
#include "../include/xionMessages.h"
#include "../include/dataset.h"
#include "../include/datasetController.h"
#include "../include/datasetValidator.h"
#include "../include/integralImage.h"

//dataset tests
int datasetValidator_folderPathNull_returnFalseAndMessage();
int datasetValidator_folderPathMaxLength_returnFalseAndMessage();
int datasetValidator_destinationFileNull_returnFalseAndMessage();
int datasetValidator_destinationFileMaxLength_returnFalseAndMessage();
int datasetValidator_dimensionMaxLength_returnFalseAndMessage();
int datasetValidator_dimensionNotNumber_returnFalseAndMessage();
int datasetValidator_dimensionZero_returnFalseAndMessage();
int datasetValidator_dimensionGreaterThanZero_returnTrue();
int createDataset_notExistingFolder_returnFalseAndMessage();

// preprocessing tests
int computeIntegrals_nullImage_returnErrorMessage();
int computeIntegrals_nullIntegral_returnErrorMessage();
int computeIntegrals_nullSquared_returnErrorMessage();
int computeIntegrals_dimensionZero_returnErrorMessage();
int computeIntegrals_image1X1_returnRightIntegrals();
int computeIntegrals_image2X2_returnRightIntegrals();
int computeIntegrals_image3X3_returnRightIntegrals();
int computeIntegrals_image1X3_returnRightIntegrals();
int computeIntegrals_image3X1_returnRightIntegrals();
int computeIntegrals_image2X3_returnRightIntegrals();
int computeIntegrals_image3X2_returnRightIntegrals();
int computeIntegrals_imageMaxResolution_returnRightIntegrals();
int computeIntegrals_imageMaxResolutionMultipleValues_returnRightIntegrals();
int computeIntegrals_imageBiggerThanMaxResolution_returnErrorMessage();

#endif