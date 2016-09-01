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
	float *X;			// this is the data after normalization and flattened in a single array
	ubyte *Y;			// the label of the data, eXected 1 or 0
	uint d;	 			// dimension of the squared matrices
	uint p;				// total points in the matrix (that is d * d)
	uint n;				// the number of elements in the dataset
	uint n1;			// the number of elements with label 1 in the dataset
	uint n0;			// the number of elements with label 0 in the dataset
} BinaryDataset;

// create a dataset in in the destinationFile made from a set of images located in a sourceFolder
// the process includes computation of integrals and normalization, 
// the dimension d is the length in pixels of the squared images;
void dat_createInFile(char *sourceFolder, char *destinationFile, uint width, uint height);

// loads dataset from positive and negative data files
BinaryDataset *dat_loadFromFiles(char *positiveDataFile, char *negativeDataFile, uint dimension);

// free resources of the dataset
void dat_close(BinaryDataset *dataset);

#endif
