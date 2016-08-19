/**
* normalization.h
* Guardian Kids
* NestNet Group
* Provides normalization operations
*/

#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "../xionDataset/dataset.h"

// normalize dataset using median and standard deviation ((x - median) / stdDev)
//void normalizeByMedian(BinaryDataset *dataset);

// normalize an image data using media1n and standard deviation ((x - median) / stdDev)
void normalizeByMedian(char *imageName, unsigned char *image, int *integral, int *squaredIntegral, float *normalization, unsigned int n);


#endif