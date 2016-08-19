/**
* haarFeature.c
* Guardian Kids
* NestNet Group
*/

#include "../../include/haarFeature.h"

float computeIntensity(float **integral, HaarRectangle *rectangle)
{
    // variables to represent vertices of the rectangle
	float d = 0.0, a = 0.0, b = 0.0, c = 0.0;
	
	if(rectangle->x - 1 >= 0 && rectangle->y - 1 >= 0)
		d = integral[rectangle->y - 1][rectangle->x - 1];
	
	if(rectangle->y - 1 >= 0)
		b = integral[rectangle->y - 1][rectangle->x + rectangle->width - 1];
	
	if(rectangle->x - 1 >= 0)
		c = integral[rectangle->y + rectangle->height - 1][rectangle->x - 1];
	
	a = integral[rectangle->y + rectangle->height - 1][rectangle->x + rectangle->width - 1];
	
	return (a - b - c + d);
}

/*float computeFeatureValue(HaarFeature *feature, float **integral)
{
    float intensityWhite = 0.0;
	float intensityBlack = 0.0;
	
	for(unsigned int i = 0; i < feature->numberOfRects; i++)
	{
		if(feature->rectangles[i].isBlack == true)
			intensityBlack += computeIntensity(integral, &feature->rectangles[i]);
		else
			intensityWhite += computeIntensity(integral, &feature->rectangles[i]);
	}

	return (intensityWhite - intensityBlack);
}*/


float computeFeatureValue(HaarFeature *feature, float *X, unsigned int index, unsigned int dim)
{
	float intensityWhite = 0.0;
	float intensityBlack = 0.0;
	
	long unsigned int stride = dim * dim * index;
	unsigned int count = feature->numberOfRects;

	for(unsigned int i = 0; i < count; i++)
	{
		HaarRectangle r = feature->rectangles[i];
		
		float d = 0.0, a = 0.0, b = 0.0, c = 0.0;

		if(r.x - 1 >= 0 && r.y - 1 >= 0)
			d = X[stride + (dim * (r.y - 1)) + (r.x - 1)];
		
		if(r.y - 1 >= 0)
			b = X[stride + (dim * (r.y - 1)) + (r.x + r.width - 1)];
		
		if(r.x - 1 >= 0)
			c = X[stride + (dim * (r.y + r.height - 1)) + (r.x - 1)];

		a = X[stride + (dim * (r.y + r.height - 1)) + (r.x + r.width - 1)];
	
		if(r.isBlack)
			intensityBlack += (a - b - c + d);
		else
			intensityWhite += (a - b - c + d);
	}
	
	return (intensityWhite - intensityBlack);
}