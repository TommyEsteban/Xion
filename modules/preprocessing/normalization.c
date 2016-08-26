/**
* NestNet Group
* Xion Object Detection Framework
* normalization.h
* normalization with integral image and squared integral image
*/

#include "../../include/normalization.h"

bool normalizationParametersValid(char *imageName, ubyte *image, int *integral, int *squaredIntegral, float *normalization, uint n)
{
	if(imageName == NULL)
	{
		strcpy(errorMessage, NULL_IMAGE_NAME);
		return false;
	}
	
	if(image == NULL)
	{
		strcpy(errorMessage, NULL_IMAGE);
		return false;
	}

	if(integral == NULL)
	{
		strcpy(errorMessage, NULL_INTEGRAL_POINTER);
		return false;
	}

	if(squaredIntegral == NULL)
	{
		strcpy(errorMessage, NULL_SQUARED_POINTER);
		return false;
	}

	if(normalization == NULL)
	{
		strcpy(errorMessage, NULL_NORMALIZATION);
		return false;
	}

	if(n == 0)
	{
		strcpy(errorMessage, NULL_DIMENSION);
		return false;
	}

	return true;
}

void normalizeByMedian(char *imageName, ubyte *image, int *integral, int *squaredIntegral, float *normalization, uint n)
{
	if(!normalizationParametersValid(imageName, image, integral, squaredIntegral, normalization, n))
	{
		printf("%s\n", errorMessage);
		return;
	}

	// median, variance and standard deviation variables
	float median = 0.0, variance = 0.0, stdDev = 0.0;
	
	// the median is the last number in the integral image divided by total
	median = integral[n - 1] / n;
	
	// the variance is the last number in the squared integral divided by total and minus median
	variance = (squaredIntegral[n - 1] / n) - median;
	
	if(variance == 0)
	{
		strcpy(warningMessage, ZERO_VARIANCE);
		printf(warningMessage, imageName);
	}
	
	// the standard deviation is the square root of the variance
	stdDev = sqrt(variance);
	
	for(uint i = 0; i < n; i++){
		if(stdDev > 0)
			normalization[i] = (image[i] - median) / stdDev;
		else
			normalization[i] = 0.0;
	}
}