/**
* normalization.c
* Guardian Kids
* NestNet Group
*/

#include "../../include/normalization.h"

/*void normalizeByMedian(BinaryDataset *dataset)
{
	// total of pixels
	int total = dataset->d * dataset->d;
	
	// median, variance and standard deviation variables
	float median = 0.0, variance = 0.0, stdDev = 0.0;
	
	// counter for variance zero images
	int vZeroCounter = 0;
	
	for(unsigned int n = 0; n < dataset->n; n++)
	{
		// the median is the last number in the integral image divided by total
		median = dataset->X1[n][dataset->d - 1][dataset->d - 1] / total;
		
		// the variance is the last number in the squared integral divided by total and minus median
		variance = (dataset->X2[n][dataset->d - 1][dataset->d - 1] / total) - median;
		
		if(variance == 0)
		{
			//printf("variance is zero in item %d, median: %.6f\n", n, median);
			vZeroCounter++;
		}
			
		// the standard deviation is the square root of the variance
		stdDev = sqrt(variance);
		
		for(unsigned int r = 0; r < dataset->d; r++)
		{
			for(unsigned int c = 0; c < dataset->d; c++)
			{
				if(stdDev != 0)
					dataset->XN[n][r][c] = (dataset->X[n][r][c] - median) / stdDev;
				else
					dataset->XN[n][r][c] = 0.0;
				//if(n == 136693 && r == 0 & c == 0) printf("XN[136693][0][0]: %.6f, stdDev: %.6f\n", dataset->XN[n][r][c], stdDev);
			}
		}
	}
	
	printf("%d images with variance zero.\n", vZeroCounter);
}*/

void normalizeByMedian(char *imageName, unsigned char *image, int *integral, int *squaredIntegral, float *normalization, unsigned int n)
{
	// median, variance and standard deviation variables
	float median = 0.0, variance = 0.0, stdDev = 0.0;
	
	// the median is the last number in the integral image divided by total
	median = integral[n - 1] / n;
	
	// the variance is the last number in the squared integral divided by total and minus median
	variance = (squaredIntegral[n - 1] / n) - median;
	
	if(variance == 0)
		printf("image %s is zero variance\n", imageName);
	
	// the standard deviation is the square root of the variance
	stdDev = sqrt(variance);
	
	for(unsigned int i = 0; i < n; i++){
		if(stdDev != 0)
			normalization[i] = (image[i] - median) / stdDev;
		else
			normalization[i] = 0.0;
	}
}