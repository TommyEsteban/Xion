/**
* NestNet Group
* Xion Object Detection Framework
* integralImage.h
* computation of integral image and squared integral image
* check integral image section on
* http://computervisionwithvaibhav.blogspot.com.co/2015/08/viola-jones-in-nut-shell.html
*/

#ifndef INTEGRALIMAGE_H
#define INTEGRALIMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "xionTypes.h"
#include "xionMessages.h"

// the maximum width and height of the input image to compute the integral images
#define MAX_WIDTH_RESOLUTION 240
#define MAX_HEIGHT_RESOLUTION 120

// compute integral and squared integral images computed, n is the total number of pixels
// the width is the image width; image contains the gray pixels values, the results will be
// loaded in the integral and squaredIntegral pointers
void computeIntegrals(ubyte *image, int *integral, int *squaredIntegral, uint n, uint width);

#endif

