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
#include <dirent.h>
#include "../include/xionTypes.h"
#include "../include/xionMessages.h"
#include "../include/xionio.h"
#include "../include/dataset.h"
#include "../include/datasetController.h"
#include "../include/datasetValidator.h"
#include "../include/integralImage.h"
#include "../include/normalization.h"

//dataset tests
int datasetValidator_folderPathNull_returnFalseAndMessage();
int datasetValidator_folderPathMaxLength_returnFalseAndMessage();
int datasetValidator_destinationFileNull_returnFalseAndMessage();
int datasetValidator_destinationFileMaxLength_returnFalseAndMessage();
int datasetValidator_widthNull_returnFalseAndMessage();
int datasetValidator_widthMaxLength_returnFalseAndMessage();
int datasetValidator_widthNotNumber_returnFalseAndMessage();
int datasetValidator_widthZero_returnFalseAndMessage();
int datasetValidator_heightNull_returnFalseAndMessage();
int datasetValidator_heightMaxLength_returnFalseAndMessage();
int datasetValidator_heightNotNumber_returnFalseAndMessage();
int datasetValidator_heightZero_returnFalseAndMessage();
int datasetValidator_widthAndHeightGreaterThanZero_returnTrue();
int createDataset_notExistingFolder_returnFalseAndErrorMessage();
int createDataset_notExistingDestinationFolder_returnFalseAndErrorMessage();
int createDataset_oneSinglePixelImage_fileCorrectlyCreated();
int loadDataset_onePositiveOneNegative_valuesCorrectlyLoaded();
int loadDataset_one2X2PositiveOne2X2Negative_valuesCorrectlyLoaded();
int loadDataset_two2X2PositiveTwo2X2Negative_valuesCorrectlyLoaded();

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

int normalization_nullImageName_returnErrorMessage();
int normalization_nullImage_returnErrorMessage();
int normalization_nullIntegral_returnErrorMessage();
int normalization_nullSquared_returnErrorMessage();
int normalization_nullNormalization_returnErrorMessage();
int normalization_dimensionZero_returnErrorMessage();
int normalization_image4X4PixelsInZero_normalizationZeroAndVarianceZeroWarning();
int normalization_image4X4PixelsIn255_normalizationZero();
int normalization_image4X4PixelsInMultipleValues_correctNormalization();

#endif