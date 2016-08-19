/**
* attentionalCascade.h
* Guardian Kids
* NestNet Group
* Attentional Cascade as described by Viola Jones
*/

#ifndef ATTENTIONALCASCADE_H
#define ATTENTIONALCASCADE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "adaBoost.h"

// the maximum number of layers in the cascade classifier
#define MAXLAYERS 50

// the maximum number of features allowed per layer
#define MAXFEATURES 100000

// initialize the cascade classifier with a number of layers
void initializeCascade(unsigned int numOfLayers);

// prediction based on cascade classificator with a given number of layers
//UBYTE predictWithCascade(float **matrix, unsigned int numOfLayers, StrongClassifier **classifier);

// prediction based on cascade classificator with a given number of layers
UBYTE predictWithCascade(float *X, unsigned int index, unsigned int numOfLayers, StrongClassifier **classifier);

// train a classifier with the attentional cascade method as explained
// by Viola and Jones, two datasets must be provided, one for the training
// and another one for validation, a feature folder could be provided to save the
// selected features in the training process, also a base file name to save the
// classifiers data per stage of the cascade process
// the parameter fi is the maximum acceptable false positive rate per layer
// the parameter di is the minimum acceptable detection rate per layer
// the parameter Ft is the overall false positive rate
void trainCascadeClassifier(BinaryDataset **tDataset, 
							BinaryDataset **vDataset, 
							float fi,
							float di,
							float Ft,
							char *featureFolder, 
							char *baseClassifiersFile,
							bool parallelMode);

// after a training a classifier this function test it over a test data set which must 
// provided, the classifiers file must by provided in the second argument and the dimension
// as the third argument correspond to the size of the squared images of the test data set
void testCascadeClassifier(BinaryDataset **testDataset, char *baseClassifiersFile, unsigned int dimension);

// free memory resources of the cascade classifier
void closeCascade();
#endif
