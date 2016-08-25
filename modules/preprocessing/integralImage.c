/**
/**
* NestNet Group
* Xion Object Detection Framework
* integralImage.c
* computation of integral image and squared integral image
* check integral image section on
* http://computervisionwithvaibhav.blogspot.com.co/2015/08/viola-jones-in-nut-shell.html
*/

#include "../../include/integralImage.h"

bool parametersValid(ubyte *image, int *integral, int *squaredIntegral, uint n, uint width)
{
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

	if(n == 0 || width == 0)
	{
		strcpy(errorMessage, NULL_DIMENSION);
		return false;
	}

	if(n < width)
	{
		strcpy(errorMessage, DIMENSION_LESS_THAN_WIDTH);
		return false;
	}

	if(n > MAX_WIDTH_RESOLUTION * MAX_HEIGHT_RESOLUTION)
	{
		strcpy(errorMessage, MAX_RESOLUTION);
		return false;
	}

	return true;
}

void computeIntegrals(ubyte *image, int *integral, int *squaredIntegral, uint n, uint width)
{
	if(!parametersValid(image, integral, squaredIntegral, n, width))
	{
		printf("%s\n", errorMessage);
		return;
	}

	integral[0] = image[0];
	squaredIntegral[0] = image[0] * image[0];
	
	for(uint i = 1; i < n; i++){
		
		if(i < width){
			integral[i] = image[i] + integral[i - 1];
			squaredIntegral[i] = (image[i] * image[i]) + squaredIntegral[i - 1];
		}
		
		else if(i % width == 0){
			integral[i] = image[i] + integral[i - width];
			squaredIntegral[i] = (image[i] * image[i]) + squaredIntegral[i - width];
		}
		
		else{
			integral[i] = image[i] + integral[i - 1] + integral[i - width] - integral[i - width - 1];
			squaredIntegral[i] = (image[i] * image[i]) + squaredIntegral[i - 1] + squaredIntegral[i - width] - squaredIntegral[i - width - 1];
		}
	}
}