/**
* integralImage.h
* Guardian Kids
* NestNet Group
* Provides computation of integral image and squared integral image
*/

#ifndef INTEGRALIMAGE_H
#define INTEGRALIMAGE_H

#include <stdio.h>
#include <stdlib.h>
//#include "../xionDataset/dataset.h"

// compute integral image and squared integral as exposed by Viola - Jones
// the variables X1 and X2 are used to put the result of computations, in the
// X1 will be the list of integral image data and in the X2 will be the 
// list of squared integral image data
//void computeIntegralImages(BinaryDataset *dataset);

// compute integral and squared integral, n is the length of pixels and dim the dimension
// of the squared images
void computeIntegrals(unsigned char *image, int *integral, int *squaredIntegral, unsigned int n, unsigned int dim);

#endif

