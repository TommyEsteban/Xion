/**
* NestNet Group
* Xion Object Detection Framework
* dataset.h
* generation and load of datasets
*/

#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "xionTypes.h"
#include "serializer.h"
#include "integralImage.h"
#include "normalization.h"

typedef struct BinaryDataset
{
	int ***X;			// this is the raw data comming from a list of images
	float ***XN;		// this is the data after normalization
	float *XP;			// this is the data after normalization and flattened in a single array
	int ***X1;			// optional transformed data after a preprocessing
	int ***X2;			// optional second transformed data after preprocessing
	ubyte *Y;			// the label of the data, expected 1 or 0
	uint d;	 			// dimension of the squared matrices
	uint p;				// total points in the matrix (that is d * d)
	uint n;				// the number of elements in the dataset
	uint n1;			// the number of elements with label 1 in the dataset
	uint n0;			// the number of elements with label 0 in the dataset
} BinaryDataset;

// load in memory a binary dataset from a data file
// the dimension is the width/height of the squared images
//void loadBinaryDataset(BinaryDataset **dataset, char *dataFilePath, unsigned int dimension);

void loadBinaryDataset(BinaryDataset **dataset, char *positiveDataFile, char *negativeDataFile, uint dimension);

// create a dataset in in the destinationFile made from a set of images located in a sourceFolder
// the process includes computation of integrals and normalization, 
// the dimension d is the length in pixels of the squared images;
void dat_createInFile(char *sourceFolder, char *destinationFile, uint d);

// free memory used in a dataset
void freeBinaryDataset(BinaryDataset **dataset);

// free some intermediate data, use it after apply transformations
void freeIntermediateData(BinaryDataset **dataset);

// fill XP array of the dataset provided by flattening the data of XN matrix
void flatData(BinaryDataset **dataset);

// print a matrix of integers from a dataset, the zero-based index is provided
// to choose which matrix to print, the data pointer is the data to print
// from the dataset and the size is the dimension of the squared matrix
void printIntegerMatrix(uint index, int ***data, uint size);

// print a matrix of floats from a dataset, the zero-based index is provided
// to choose which matrix to print, the data pointer is the data to print
// from the dataset and the size is the dimension of the squared matrix
void printfloatMatrix(uint index, float ***data, uint size);

#endif
