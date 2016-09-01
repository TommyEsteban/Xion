/**
* NestNet Group
* Xion Object Detection Framework
* dataset.c
* generation and loading of datasets
*/

#include "../../include/dataset.h"

#define BUFFER 12

void dat_createInFile(char *sourceFolder, char *destinationFile, uint width, uint height)
{
	struct dirent *ent;
	uint counter = 0;
	int components = 3;
	uint p = width * height;
	
	DIR *dir = opendir (sourceFolder);

	// read files in the directory one by one
	while ((ent = readdir (dir)) != NULL) 
	{
		// skip '.' and '..' directory entries
		if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
		{
			uint len1 = strlen(sourceFolder);
			uint len2 = strlen(ent->d_name);
			
			// variable to concat the directory with the file name
			char *filePath = (char *)malloc(len1 + len2 + 2);
			
			if(filePath == NULL)
				return;
				
			strcpy(filePath, sourceFolder);
			strcat(filePath, "/");
			strcat(filePath, ent->d_name);

			ubyte *imageRawData = NULL;
			ubyte *imageData = (ubyte *)malloc(sizeof(ubyte) * p);
			int *integral = (int *)malloc(sizeof(int) * p);
			int *squaredIntegral = (int *)malloc(sizeof(int) * p);
			float *normalization = (float *)malloc(sizeof(float) * p);
			ser_readJpegFile(filePath, &imageRawData, &components);

			for(uint i = 0, j = 0; i < p * components; i += components, j++)
			{
				imageData[j] = imageRawData[i]; 
			}
			
			computeIntegrals(imageData, integral, squaredIntegral, p, width);
			normalizeByMedian(filePath, imageData, integral, squaredIntegral, normalization, p);
			ser_appendToFile(destinationFile, normalization, p);
			
			free(imageRawData);
			free(imageData);
			free(filePath);
			free(normalization);
			free(squaredIntegral);
			free(integral);
			counter++;
		}
	}
	
	free(ent);
	closedir (dir);
	
	printf("Serialized %d image files.\n", counter);
}

BinaryDataset *dat_loadFromFiles(char *positiveDataFile, char *negativeDataFile, uint width, uint height)
{
	BinaryDataset *dataset = (BinaryDataset *)malloc(sizeof(BinaryDataset));

	// intialialize the dataset structure
	dataset->d = width;
	dataset->w = width;
	dataset->h = height;
	dataset->p = width * height;
	dataset->n = 0;
	dataset->n1 = 0;
	dataset->n0 = 0;
	
	// initialize variables
	char c = 0;
	char value[BUFFER];
	uint valueIndex = 0, index = 0;
	float number = 0;
	
	// open the files
	FILE *inputPos = fopen(positiveDataFile, "r");
	FILE *inputNeg = fopen(negativeDataFile, "r");
	
	// count the values in the positive file and compute the length of samples
	while(!feof(inputPos)) { c = fgetc(inputPos); if(c == ' ') dataset->n1++; }
	dataset->n1 = dataset->n1 / dataset->p;
	printf("number of positive elements: %d\n", dataset->n1);
	
	// count the values in the negative file and compute the length of samples
	while(!feof(inputNeg)) { c = fgetc(inputNeg); if(c == ' ') dataset->n0++; }
	dataset->n0 = dataset->n0 / dataset->p;
	printf("number of negative elements: %d\n", dataset->n0);
	
	// compute the total of samples, positive ones plus negative ones
	dataset->n = dataset->n1 + dataset->n0;

	// allocate memory for X and Y vectors
	dataset->X = (float *)malloc(sizeof(float) * dataset->n * dataset->p);
	dataset->Y = (ubyte *)malloc(sizeof(ubyte) * dataset->n);
	
	//fill the labels
	for(uint i = 0; i < dataset->n1; i++) { dataset->Y[i] = 1; }
	for(uint i = dataset->n1; i < dataset->n; i++) { dataset->Y[i] = 0; }
	
	rewind(inputPos);
	rewind(inputNeg);
	
	// read the data file char by char
	while ((c = fgetc(inputPos)) != EOF)
    {
		// in case a space is found, it is just after a number
		if((char)c == ' ')
		{
			// mark the end of the word (number as string) and convert the string value to float number
			value[valueIndex] = '\0';
			number = atof(value);

			// set the value in the dataset
			dataset->X[index++] = number;
			
			// initialize the value index for a new value
			valueIndex = 0;
		}
		// otherwise set next character into the auxiliar array
		else
			value[valueIndex++] = (char)c;
	}
	
	fclose(inputPos);
	valueIndex = 0;
	
	// read the data file char by char
	while ((c = fgetc(inputNeg)) != EOF)
    {
		// in case a space is found, it is just after a number
		if((char)c == ' ')
		{
			// mark the end of the word (number as string) and convert the string value to float number
			value[valueIndex] = '\0';
			number = atof(value);

			// set the value in the dataset
			dataset->X[index++] = number;
			
			// initialize the value index for a new value
			valueIndex = 0;
		}
		// otherwise set next character into the auxiliar array
		else
			value[valueIndex++] = (char)c;
	}
	
	fclose(inputNeg);
	return dataset;
}

void dat_close(BinaryDataset *dataset)
{
	free(dataset->X);
	free(dataset->Y);
	free(dataset);
}

