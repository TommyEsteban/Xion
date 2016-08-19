/**
* integralImage.c
* Guardian Kids
* NestNet Group
*/

#include "../../include/integralImage.h"

/*void computeIntegralImages(BinaryDataset *dataset)
{
	if(dataset->X == NULL || dataset->X1 == NULL || dataset->X2 == NULL)
	{
		printf("pointers are not initialized.\n");
		return;
	}
	
	for(unsigned int n = 0; n < dataset->n; n++)
	{
		// initialize the first pixel for integrals
		dataset->X1[n][0][0] = dataset->X[n][0][0];
		dataset->X2[n][0][0] = (dataset->X[n][0][0] * dataset->X[n][0][0]);
		
		for(unsigned int y = 0; y < dataset->d; y++)
		{
			for(unsigned int x = 0; x < dataset->d; x++)
			{
				if(x > 0 && y > 0)
				{
					dataset->X1[n][y][x] = dataset->X[n][y][x] + dataset->X1[n][y][x - 1] + dataset->X1[n][y - 1][x] - dataset->X1[n][y - 1][x - 1];
					dataset->X2[n][y][x] = (dataset->X[n][y][x] * dataset->X[n][y][x]) + dataset->X2[n][y][x - 1] + dataset->X2[n][y - 1][x] - dataset->X2[n][y - 1][x - 1];
				}
				
				if(x == 0 && y > 0)
				{
					dataset->X1[n][y][x] = dataset->X[n][y][x] + dataset->X1[n][y - 1][x];
					dataset->X2[n][y][x] = (dataset->X[n][y][x] * dataset->X[n][y][x]) + dataset->X2[n][y - 1][x];
				}
				
				if(x > 0 && y == 0)
				{
					dataset->X1[n][y][x] = dataset->X[n][y][x] + dataset->X1[n][y][x - 1];
					dataset->X2[n][y][x] = (dataset->X[n][y][x] * dataset->X[n][y][x]) + dataset->X2[n][y][x - 1];
				}
			}
		}
	}
}*/

void computeIntegrals(unsigned char *image, int *integral, int *squaredIntegral, unsigned int n, unsigned int dim)
{
	integral[0] = image[0];
	squaredIntegral[0] = image[0] * image[0];
	
	for(unsigned int i = 1; i < n; i++){
		
		if(i < dim){
			integral[i] = image[i] + integral[i - 1];
			squaredIntegral[i] = (image[i] * image[i]) + squaredIntegral[i - 1];
		}
		
		else if(i % dim == 0){
			integral[i] = image[i] + integral[i - dim];
			squaredIntegral[i] = (image[i] * image[i]) + squaredIntegral[i - dim];
		}
		
		else{
			integral[i] = image[i] + integral[i - 1] + integral[i - dim] - integral[i - dim - 1];
			squaredIntegral[i] = (image[i] * image[i]) + squaredIntegral[i - 1] + squaredIntegral[i - dim] - squaredIntegral[i - dim - 1];
			
			//if((*integral)[i] < 0)
				//printf("%d, int: %d\n", i, (*integral)[i]);
			//printf("i: %d, dim: %d, image: %d, integral[i - 1]: %d, integral[i - dim] : %d, integral[i - dim - 1]: %d\n", 
			//i, dim, (*image[i]), (*integral)[i - 1], (*integral)[i - dim], (*integral)[i - dim - 1] );
			//getchar();
		}
		
		/*if((*integral)[i] < 0)
		{
			printf("i: %d, dim: %d, image: %d, integral[i - 1]: %d, integral[i - dim] : %d, integral[i - dim - 1]: %d\n", 
			i, dim, (*image[i]), (*integral)[i - 1], (*integral)[i - dim], (*integral)[i - dim - 1] );
			getchar();
		}*/
	}
}

/*void computeIntegrals(int **image, int **integral, int **squaredIntegral, int dimension){
	
	for(unsigned int y = 0; y < dimension; y++)
	{
		for(unsigned int x = 0; x < dimension; x++)
		{
			if(x > 0 && y > 0)
			{
				integral[y][x] = image[y][x] + integral[y][x - 1] + integral[y - 1][x] - integral[y - 1][x - 1];
				squaredIntegral[y][x] = (image[y][x] * image[y][x]) + squaredIntegral[y][x - 1] + squaredIntegral[y - 1][x] - squaredIntegral[y - 1][x - 1];
			}
			
			if(x == 0 && y > 0)
			{
				integral[y][x] = image[y][x] + integral[y - 1][x];
				squaredIntegral[y][x] = (image[y][x] * image[y][x]) + squaredIntegral[y - 1][x];
			}
			
			if(x > 0 && y == 0)
			{
				integral[y][x] = image[y][x] + integral[y][x - 1];
				squaredIntegral[y][x] = (image[y][x] * image[y][x]) + squaredIntegral[y][x - 1];
			}
		}
	}
}*/