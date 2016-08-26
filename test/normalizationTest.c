/**
* NestNet Group
* Xion Object Detection Framework
* normalizationTest.c
* unit testing for normalization computations
*/

#include "xionTest.h"
#include "preprocessingTest.h"

int normalization_nullImageName_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();

    // act
    normalizeByMedian(imageName, image, integral, squared, normalization, 1);

    // assert
    if(strcmp(NULL_IMAGE_NAME, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_nullImage_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();
    imageName = (char *)malloc(sizeof(char) * 1);

    // act
    normalizeByMedian(imageName, image, integral, squared, normalization, 1);

    // assert
    if(strcmp(NULL_IMAGE, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_nullIntegral_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();
    imageName = (char *)malloc(sizeof(char) * 1);
    image = (ubyte *)malloc(sizeof(ubyte) * 1);

    // act
    normalizeByMedian(imageName, image, integral, squared, normalization, 1);

    // assert
    if(strcmp(NULL_INTEGRAL_POINTER, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_nullSquared_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();
    imageName = (char *)malloc(sizeof(char) * 1);
    image = (ubyte *) malloc(sizeof(ubyte) * 1);
    integral = (int *) malloc(sizeof(int) * 1);

    // act
    normalizeByMedian(imageName, image, integral, squared, normalization, 1);

    // assert
    if(strcmp(NULL_SQUARED_POINTER, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_nullNormalization_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();
    imageName = (char *)malloc(sizeof(char) * 1);
    image = (ubyte *) malloc(sizeof(ubyte) * 1);
    integral = (int *) malloc(sizeof(int) * 1);
    squared = (int *) malloc(sizeof(int) * 1);

    // act
    normalizeByMedian(imageName, image, integral, squared, normalization, 1);

    // assert
    if(strcmp(NULL_NORMALIZATION, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_dimensionZero_returnErrorMessage()
{
    // arrange
    initializePreproTest(1);

    // act
    normalizeByMedian(imageName, image, integral, squared, normalization, 0);

    // assert
    if(strcmp(NULL_DIMENSION, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_image4X4PixelsInZero_normalizationZeroAndVarianceZeroWarning()
{
    // arrange
    int n = 4;
    initializePreproTest(n);
    strcpy(imageName, "testImage");
    image[0] = 0; image[1] = 0;
    image[2] = 0; image[3] = 0;

    // act
    computeIntegrals(image, integral, squared, n, 2);
    normalizeByMedian(imageName, image, integral, squared, normalization, n);

    // assert
    if(normalization[0] == 0 && normalization[1] == 0 && 
        normalization[2] == 0 && normalization[3] == 0 &&
        strcmp(ZERO_VARIANCE, warningMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_image4X4PixelsIn255_normalizationZero()
{
    // arrange
    int n = 4;
    initializePreproTest(n);
    strcpy(imageName, "testImage");
    image[0] = 255; image[1] = 255;
    image[2] = 255; image[3] = 255;

    // act
    computeIntegrals(image, integral, squared, n, 2);
    normalizeByMedian(imageName, image, integral, squared, normalization, n);

    // assert
    // (even variance is different from Zero, the normalization is zero)
    if(normalization[0] == 0 && normalization[1] == 0 && 
        normalization[2] == 0 && normalization[3] == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int normalization_image4X4PixelsInMultipleValues_correctNormalization()
{
    // arrange
    int n = 4;
    initializePreproTest(n);
    strcpy(imageName, "testImage");
    image[0] = 1; image[1] = 255;
    image[2] = 1; image[3] = 255;

    // act
    computeIntegrals(image, integral, squared, n, 2);
    normalizeByMedian(imageName, image, integral, squared, normalization, n);

    // assert
    if(normalization[0] > -0.705718936 && normalization[0] < -0.705718934 &&
        normalization[1] > 0.705718934 && normalization[1] < 0.705718936 &&
        normalization[2] > -0.705718936 && normalization[2] < -0.705718934 &&
        normalization[3] > 0.705718934 && normalization[3] < 0.705718936)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}