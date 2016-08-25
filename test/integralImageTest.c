/**
* NestNet Group
* Xion Object Detection Framework
* integralImageTest.c
* unit testing for dataset validator
*/

#include "xionTest.h"
#include "preprocessingTest.h"

int computeIntegrals_nullImage_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();

    // act
    computeIntegrals(image, integral, squared, 1, 1);

    // assert
    if(strcmp(NULL_IMAGE, errorMessage) == 0)
    {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int computeIntegrals_nullIntegral_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();
    image = (ubyte *)malloc(sizeof(ubyte) * 1);

    // act
    computeIntegrals(image, integral, squared, 1, 1);

    // assert
    if(strcmp(NULL_INTEGRAL_POINTER, errorMessage) == 0)
    {
        free(image);
        return EXIT_SUCCESS;
    }

    free(image);
    return EXIT_FAILURE;
}

int computeIntegrals_nullSquared_returnErrorMessage()
{
    // arrange
    initializePreproTestNull();
    image = (ubyte *) malloc(sizeof(ubyte) * 1);
    integral = (int *) malloc(sizeof(int) * 1);

    // act
    computeIntegrals(image, integral, squared, 1, 1);

    // assert
    if(strcmp(NULL_SQUARED_POINTER, errorMessage) == 0)
    {
        free(image);
        free(integral);
        return EXIT_SUCCESS;
    }

    free(image);
    free(integral);
    return EXIT_FAILURE;
}

int computeIntegrals_dimensionZero_returnErrorMessage()
{
    // arrange
    initializePreproTest(1);

    // act
    computeIntegrals(image, integral, squared, 0, 0);

    // assert
    if(strcmp(NULL_DIMENSION, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_image1X1_returnRightIntegrals()
{
    // arrange
    initializePreproTest(1);
    image[0] = 255;

    // act
    computeIntegrals(image, integral, squared, 1, 1);

    // assert
    if(integral[0] == 255 && squared[0] == (255 * 255))
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_image2X2_returnRightIntegrals()
{
    // arrange
    initializePreproTest(4);
    image[0] = 255; image[1] = 255;
    image[2] = 255; image[3] = 255;

    // act
    computeIntegrals(image, integral, squared, 4, 2);

    // assert
    if(integral[0] == 255 && squared[0] == 255 * 255 &&
        integral[1] == 510 && squared[1] == 510 * 255 &&
        integral[2] == 510 && squared[2] == 510 * 255 &&
        integral[3] == 1020 && squared[3] == 1020 * 255)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_image3X3_returnRightIntegrals()
{
    // arrange
    initializePreproTest(9);
    image[0] = 255; image[1] = 255; image[2] = 255;
    image[3] = 255; image[4] = 255; image[5] = 255;
    image[6] = 255; image[7] = 255; image[8] = 255;

    // act
    computeIntegrals(image, integral, squared, 9, 3);

    // assert
    if(integral[0] == 255 && squared[0] == 255 * 255 &&
        integral[1] == 510 && squared[1] == 510 * 255 &&
        integral[2] == 765 && squared[2] == 765 * 255 &&
        integral[3] == 510 && squared[3] == 510 * 255 &&
        integral[4] == 1020 && squared[4] == 1020 * 255 &&
        integral[5] == 1530 && squared[5] == 1530 * 255 &&
        integral[6] == 765 && squared[6] == 765 * 255 &&
        integral[7] == 1530 && squared[7] == 1530 * 255 &&
        integral[8] == 2295 && squared[8] == 2295 * 255 
        )
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_image1X3_returnRightIntegrals()
{
    // arrange
    initializePreproTest(3);
    image[0] = 255; image[1] = 255; image[2] = 255;

    // act
    computeIntegrals(image, integral, squared, 3, 1);

    // assert
    if(integral[0] == 255 && squared[0] == 255 * 255 &&
        integral[1] == 510 && squared[1] == 510 * 255 &&
        integral[2] == 765 && squared[2] == 765 * 255)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_image3X1_returnRightIntegrals()
{
    // arrange
    initializePreproTest(3);
    image[0] = 255; 
    image[1] = 255; 
    image[2] = 255;

    // act
    computeIntegrals(image, integral, squared, 3, 3);

    // assert
    if(integral[0] == 255 && squared[0] == 255 * 255 &&
        integral[1] == 510 && squared[1] == 510 * 255 &&
        integral[2] == 765 && squared[2] == 765 * 255)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_image2X3_returnRightIntegrals()
{
    // arrange
    initializePreproTest(6);
    image[0] = 255; image[1] = 255;
    image[2] = 255; image[3] = 255;
    image[4] = 255; image[5] = 255;

    // act
    computeIntegrals(image, integral, squared, 6, 2);

    // assert
    if(integral[0] == 255 && squared[0] == 255 * 255 &&
        integral[1] == 510 && squared[1] == 510 * 255 &&
        integral[2] == 510 && squared[2] == 510 * 255 &&
        integral[3] == 1020 && squared[3] == 1020 * 255 &&
        integral[4] == 765 && squared[4] == 765 * 255 &&
        integral[5] == 1530 && squared[5] == 1530 * 255)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_image3X2_returnRightIntegrals()
{
    // arrange
    initializePreproTest(6);
    image[0] = 255; image[1] = 255; image[2] = 255;
    image[3] = 255; image[4] = 255; image[5] = 255;

    // act
    computeIntegrals(image, integral, squared, 6, 3);

    // assert
    if(integral[0] == 255 && squared[0] == 255 * 255 &&
        integral[1] == 510 && squared[1] == 510 * 255 &&
        integral[2] == 765 && squared[2] == 765 * 255 &&
        integral[3] == 510 && squared[3] == 510 * 255 &&
        integral[4] == 1020 && squared[4] == 1020 * 255 &&
        integral[5] == 1530 && squared[5] == 1530 * 255)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_imageMaxResolution_returnRightIntegrals()
{
    // arrange
    int n = MAX_WIDTH_RESOLUTION * MAX_HEIGHT_RESOLUTION;
    initializePreproTest(n);

    for(uint i = 0; i < n; i++)
        image[i] = 255;
    
    // act
    computeIntegrals(image, integral, squared, n, MAX_WIDTH_RESOLUTION);

    // assert
    // check the values in the four corners of the image
    if(integral[0] == 255 && squared[0] == 255 * 255 &&
        integral[239] == 61200 && squared[239] == 61200 * 255 &&
        integral[28560] == 30600 && squared[28560] == 30600 * 255 &&
        integral[28799] == 7344000 && squared[28799] == 7344000 * 255)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_imageMaxResolutionMultipleValues_returnRightIntegrals()
{
    // arrange
    int n = MAX_WIDTH_RESOLUTION * MAX_HEIGHT_RESOLUTION;
    initializePreproTest(n);

    for(uint i = 0; i < n; i++)
        image[i] = i % 255;
    
    // act
    computeIntegrals(image, integral, squared, n, MAX_WIDTH_RESOLUTION);

    // assert
    // check the values in the four corners of the image
    if(integral[0] == 0 && squared[0] == 0 &&
        integral[239] == 28680 && squared[239] == 4579240 &&
        integral[28560] == 14280 && squared[28560] == 2356200 &&
        integral[28799] == 3655800 && squared[28799] == 619980600)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}

int computeIntegrals_imageBiggerThanMaxResolution_returnErrorMessage()
{
    // arrange
    int n = MAX_WIDTH_RESOLUTION * MAX_HEIGHT_RESOLUTION + 1;
    initializePreproTest(n);

    // act
    computeIntegrals(image, integral, squared, n, 1);

    // assert
    if(strcmp(MAX_RESOLUTION, errorMessage) == 0)
    {
        releasePreproTest();
        return EXIT_SUCCESS;
    }

    releasePreproTest();
    return EXIT_FAILURE;
}