/**
* NestNet Group
* Xion Object Detection Framework
* main.c
* main entry point for Xion Unit Tests
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "xionTest.h"

int datasetTestSuite();
int preprocessingTestSuit();

int main (int argc, char **argv)
{
    if(strcmp(argv[1], "dataset") == 0)
        return datasetTestSuite();
    
    if(strcmp(argv[1], "preprocessing") == 0)
        return preprocessingTestSuit();
    
    //printf("arg 1: %s\n", argv[1]);
    return EXIT_FAILURE;
}

int datasetTestSuite()
{
    if(datasetValidator_folderPathNull_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_folderPathMaxLength_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_destinationFileNull_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_destinationFileMaxLength_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(datasetValidator_dimensionMaxLength_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_dimensionNotNumber_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(datasetValidator_dimensionZero_returnFalseAndMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(datasetValidator_dimensionGreaterThanZero_returnTrue() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(createDataset_notExistingFolder_returnFalseAndErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(createDataset_notExistingDestinationFolder_returnFalseAndErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(createDataset_invalidDimension_returnFalseAndErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(createDataset_oneSinglePixelImage_fileCorrectlyCreated() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(loadDataset_onePositiveOneNegative_valuesCorrectlyLoaded() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(loadDataset_one4X4PositiveOne4X4Negative_valuesCorrectlyLoaded() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

int preprocessingTestSuit()
{
    if(computeIntegrals_nullImage_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_nullIntegral_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_nullSquared_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_dimensionZero_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(computeIntegrals_image1X1_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_image2X2_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(computeIntegrals_image3X3_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_image1X3_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_image3X1_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_image2X3_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(computeIntegrals_image3X2_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(computeIntegrals_imageMaxResolution_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(computeIntegrals_imageMaxResolutionMultipleValues_returnRightIntegrals() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(computeIntegrals_imageBiggerThanMaxResolution_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(normalization_nullImageName_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(normalization_nullImage_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(normalization_nullIntegral_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(normalization_nullSquared_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(normalization_nullNormalization_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(normalization_dimensionZero_returnErrorMessage() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(normalization_image4X4PixelsInZero_normalizationZeroAndVarianceZeroWarning() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if(normalization_image4X4PixelsIn255_normalizationZero() == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if(normalization_image4X4PixelsInMultipleValues_correctNormalization() == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}