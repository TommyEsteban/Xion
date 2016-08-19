/**
* haarFeature.h
* Guardian Kids
* NestNet Group
*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#ifndef HAARFEATURE_H
#define HAARFEATURE_H

typedef struct HaarRectangle
{
    unsigned char x;		// top left x coordinate
    unsigned char y;		// top left y coordinate
    unsigned char width;	// width of the rectangle taken from the x coordinate
    unsigned char height;	// height of the rectangle taken from the y coordinate
	bool isBlack;
}HaarRectangle;

typedef struct HaarFeature
{
	HaarRectangle *rectangles;		// the rectangles of the haar-like feature
	unsigned char numberOfRects;		// the number of rects in the haar-like features
}HaarFeature;

// compute the intensity of one rectangle
float computeIntensity(float **integral, HaarRectangle *rectangle);

// compute the feature value, this is white rectangles minus black rectangles
//float computeFeatureValue(HaarFeature *feature, float **integral);

// compute feature value in flat mode
float computeFeatureValue(HaarFeature *feature, float *X, unsigned int index, unsigned int dim);

#endif
