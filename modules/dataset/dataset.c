/**
* NestNet Group
* Xion Object Detection Framework
* dataset.c
* generation and load of datasets
*/

#include "../../include/dataset.h"

#define BUFFER 12

void allocMemory(BinaryDataset *dataset)
{
	// memory allocation for the first indirection level for every pointer
	dataset->X = (int ***)malloc(sizeof(int **) * dataset->n);
	dataset->XN = (float ***)malloc(sizeof(float **) * dataset->n);
	dataset->XP = (float *)malloc(sizeof(float) * dataset->n * dataset->d * dataset->d);
	dataset->X1 = (int ***)malloc(sizeof(int **) * dataset->n);
	dataset->X2 = (int ***)malloc(sizeof(int **) * dataset->n);
	dataset->Y = (ubyte *)malloc(sizeof(ubyte) * dataset->n);
	
	// check if some error 
	if(dataset->X == NULL || dataset->XN == NULL || dataset->XP == NULL || dataset->X1 == NULL || dataset->X2 == NULL || dataset->Y == NULL)
	{
		fprintf(stderr, "problem allocating memory in dataset.\n");
		return;
	}
	
	// allocate memory for every indirection level
	for(unsigned int i = 0; i < dataset->n; i++)
	{
		dataset->X[i] = (int **)malloc(sizeof(int *) * dataset->d);
		dataset->XN[i] = (float **)malloc(sizeof(float *) * dataset->d);
		dataset->X1[i] = (int **)malloc(sizeof(int *) * dataset->d);
		dataset->X2[i] = (int **)malloc(sizeof(int *) * dataset->d);
		
		if(dataset->X[i] == NULL || dataset->XN[i] == NULL || 
		   dataset->X1[i] == NULL || dataset->X2[i] == NULL)
		{
			fprintf(stderr, "problem allocating memory in dataset.\n");
			return;
		}
		
		for(unsigned int j = 0; j < dataset->d; j++)
		{
			dataset->X[i][j] = (int *)malloc(sizeof(int) * dataset->d);
			dataset->XN[i][j] = (float *)malloc(sizeof(float) * dataset->d);
			dataset->X1[i][j] = (int *)malloc(sizeof(int) * dataset->d);
			dataset->X2[i][j] = (int *)malloc(sizeof(int) * dataset->d);
			
			if(dataset->X[i][j] == NULL || dataset->XN[i][j] == NULL || 
			   dataset->X1[i][j] == NULL || dataset->X2[i][j] == NULL)
			{
				fprintf(stderr, "problem allocating memory in dataset.\n");
				return;
			}
		}
	}
}

/*void loadBinaryDataset(BinaryDataset **dataset, char *dataFilePath, unsigned int dimension)
{
	// intialialize the dataset structure
	(*dataset)->d = dimension;
	(*dataset)->p = dimension * dimension;
	(*dataset)->n = 0;
	(*dataset)->n1 = 0;
	(*dataset)->n0 = 0;
	
	// initialize variables
	int c = 0;
	char value[BUFFER];
	int index = 0, number = 0;
	int elementIndex = 0, rowIndex = 0, columnIndex = 0;
	
	// open the file
	FILE *input = fopen(dataFilePath, "r");
	
	// check for the availability of the data file
	if(input == NULL)
	{
		printf("data file could not be open\n");
		return;
	}
	
	printf("accessing data file: %s\n", dataFilePath);
	
	// count the lines of the file
	while(!feof(input))
	{
	  c = fgetc(input);
	  
	  if(c == '\n')
	  {
		(*dataset)->n++;
	  }
	}
	
	printf("number of elements in data file: %d\n", (*dataset)->n);
	allocMemory(*dataset);
	rewind(input);
	
	// read the data file char by char
	while ((c = fgetc(input)) != EOF)
    {
		// in case a space is found is just after a number
		if((char)c == ' ')
		{
			// mark the end of the word
			value[index] = '\0';
			
			// convert string to number
			number = atoi(value);
			
			//if(elementIndex == 0)
				//printf("%d ", number);
			
			// set the value in the dataset
			(*dataset)->X[elementIndex][rowIndex][columnIndex] = number;
			columnIndex++;
			
			// initialize the index for a new value
			index = 0;
		}
		
		// in case a return character is found a new sample data is next to begin
		else if((char)c == '\n')
		{
			// get the label value
			value[index] = '\0';
			number = atoi(value);
			(*dataset)->Y[elementIndex] = number;
			
			//count the labels
			if(number == 1)
				(*dataset)->n1++;
			
			if(number == 0)
				(*dataset)->n0++;
			
			// initialize indexes and add one to the elementIndex
			index = 0;
			rowIndex = 0;
			elementIndex++;
		}
		
		// otherwise set next character into the auxiliar array
		else
		{
			value[index] = (char)c;
			index++;
		}
		
		// when scanline of the image ends, start again the counter of columns
		if(columnIndex != 0 && (columnIndex % dimension) == 0)
		{
			columnIndex = 0;
			rowIndex++;
			//printf("row: %d\n", rowIndex);
		}
	}
	
	fclose(input);
}*/

void loadBinaryDataset(BinaryDataset **dataset, char *positiveDataFile, char *negativeDataFile, uint dimension)
{//printf("dim : %d, negFile: %s\n", dimension, negativeDataFile);
	// intialialize the dataset structure
	(*dataset)->d = dimension;
	(*dataset)->p = dimension * dimension;
	(*dataset)->n = 0;
	(*dataset)->n1 = 0;
	(*dataset)->n0 = 0;
	
	// initialize variables
	char c = 0;
	char value[BUFFER];
	uint valueIndex = 0, index = 0;
	float number = 0;
	
	// open the files
	FILE *inputPos = fopen(positiveDataFile, "r");
	FILE *inputNeg = fopen(negativeDataFile, "r");
	
	if(inputPos == NULL) { printf("positive data file could not be open\n"); return; }
	if(inputNeg == NULL) { printf("negative data file could not be open\n"); return; }
	
	printf("accessing positive data file: %s\n", positiveDataFile);
	// count the values in the positive file and compute the length of samples
	while(!feof(inputPos)) { c = fgetc(inputPos); if(c == ' ') (*dataset)->n1++; }
	(*dataset)->n1 = (*dataset)->n1 / (*dataset)->p;
	printf("number of positive elementes in file: %d\n", (*dataset)->n1);
	
	printf("accessing negative data file: %s\n", negativeDataFile);
	// count the values in the negative file and compute the length of samples
	while(!feof(inputNeg)) { c = fgetc(inputNeg); if(c == ' ') (*dataset)->n0++; }
	(*dataset)->n0 = (*dataset)->n0 / (*dataset)->p;
	printf("number of negative elements in file: %d\n", (*dataset)->n0);
	
	// compute the total of samples, positive ones plus negative ones
	(*dataset)->n = (*dataset)->n1 + (*dataset)->n0;
	// allocate memory for XP and Y vectors
	(*dataset)->XP = (float *)malloc(sizeof(float) * (*dataset)->n * (*dataset)->p);
	(*dataset)->Y = (ubyte *)malloc(sizeof(ubyte) * (*dataset)->n);
	
	//fill the labels
	for(uint i = 0; i < (*dataset)->n1; i++) { (*dataset)->Y[i] = 1; }
	for(uint i = (*dataset)->n1; i < (*dataset)->n; i++) { (*dataset)->Y[i] = 0; }
	
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
			(*dataset)->XP[index++] = number;
			
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
			(*dataset)->XP[index++] = number;
			
			// initialize the value index for a new value
			valueIndex = 0;
		}
		// otherwise set next character into the auxiliar array
		else
			value[valueIndex++] = (char)c;
	}
	
	fclose(inputNeg);
}

void dat_createInFile(char *sourceFolder, char *destinationFile, uint d)
{
	struct dirent *ent;
	uint counter = 0;
	int components = 3;
	uint p = d * d;
	//printf("source: %s, dest: %s, dim: %d\n", sourceFolder, destinationFile, d);
	DIR *dir = opendir (sourceFolder);

	// read files in the directory one by one
	while ((ent = readdir (dir)) != NULL) 
	{
		// skip '.' and '..' directory entries
		// printf("%s\n", ent->d_name);
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
			//printf("%d %s\n", counter + 1, filePath);

			ubyte *imageRawData = NULL;
			ubyte *imageData = (ubyte *)malloc(sizeof(ubyte) * p);
			int *integral = (int *)malloc(sizeof(int) * p);
			int *squaredIntegral = (int *)malloc(sizeof(int) * p);
			float *normalization = (float *)malloc(sizeof(float) * p);
			readJpegFile(filePath, &imageRawData, &components);
			//printf("%d - comp: %d\n", imageRawData[0], components);
			for(uint i = 0, j = 0; i < p * components; i += components, j++)
			{
				imageData[j] = imageRawData[i]; 
			}
			
			computeIntegrals(imageData, integral, squaredIntegral, p, d);
			normalizeByMedian(filePath, imageData, integral, squaredIntegral, normalization, p);
			appendToFile(destinationFile, normalization, p);
			
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

void flatData(BinaryDataset **dataset)
{
	uint flatIndex = 0;
	//printf("**X[0]: %.6f\n", (*dataset)->XN[0][0][0]);
	for(uint i = 0; i < (*dataset)->n; i++)
	{
		for(uint r = 0; r < (*dataset)->d; r++)
		{
			for(uint c = 0; c < (*dataset)->d; c++)
			{
				(*dataset)->XP[flatIndex] = (*dataset)->XN[i][r][c];
				flatIndex++;
			}
		}
	}
}

void freeBinaryDataset(BinaryDataset **dataset)
{
	free((*dataset)->XP);
	free((*dataset)->Y);
}

void freeIntermediateData(BinaryDataset **dataset)
{
	for(uint n = 0; n < (*dataset)->n; n++)
	{
		for(uint j = 0; j < (*dataset)->d; j++)
		{
			free((*dataset)->X[n][j]);
			free((*dataset)->X1[n][j]);
			free((*dataset)->X2[n][j]);
			free((*dataset)->XN[n][j]);
		}
		
		free((*dataset)->X[n]);
		free((*dataset)->X1[n]);
		free((*dataset)->X2[n]);
		free((*dataset)->XN[n]);
	}
	
	free((*dataset)->X);
	free((*dataset)->X1); 
	free((*dataset)->X2);
	free((*dataset)->XN);
}

void printIntegerMatrix(uint index, int ***data, uint size)
{
	for(uint i = 0; i < size; i++)
	{
		for(uint j = 0; j < size; j++)
		{
			printf("%d ", data[index][i][j]);
		}
		
		printf("\n");
	}
	
	printf("\n");
}

void printfloatMatrix(uint index, float ***data, uint size)
{
	for(uint i = 0; i < size; i++)
	{
		for(uint j = 0; j < size; j++)
		{
			printf("%.2f ", data[index][i][j]);
		}
		
		printf("\n");
	}
	
	printf("\n");
}
