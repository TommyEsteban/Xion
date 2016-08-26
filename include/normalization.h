/**
* NestNet Group
* Xion Object Detection Framework
* normalization.h
* normalization with integral image and squared integral image
*/

#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "xionTypes.h"
#include "xionMessages.h"

// normalize an image data using media1n and standard deviation ((x - median) / stdDev)
// the result will be put in normalization pointer
void normalizeByMedian(char *imageName, ubyte *image, int *integral, int *squaredIntegral, float *normalization, uint n);


#endif