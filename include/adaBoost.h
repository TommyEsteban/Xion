/**
* adaBoost.h
* Guardian Kids
* NestNet Group
* Ada Boost classification as described by Viola Jones
*/

#ifndef ADABOOST_H
#define ADABOOST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adaBoostTypes.h"
#include "cuda/parallelFunctions.h"
#include "dataset.h"
#include "serializer.h"
#include "haarFeature.h"


// initialize of the Ada Boost classifier with a maximum number of features expected to train
void initializeAdaBoost(unsigned int maxNumOfFeatures, BinaryDataset *dataset, bool inParallelMode);

// initialize the Ada Boost classifier only for test stage
void initializeAdaBoostForTest(unsigned int numberOfTestSamples, unsigned int dimension);

// prediction based on weak classificator in flat mode
UBYTE classifyWeakly(float *X, unsigned int index, WeakClassifier *classifier);

// prediction based on strong classificator in flat mode
UBYTE classifyAdaBoost(float *X, unsigned int index, StrongClassifier *classifier);

// train a monolitic classifier using Ada Boost as explaining by Viola Jones
// the number of features specify how many features will be trained for this call
// this number could less thatn the maximum number of features specified in the initialiation
// a dataset previously loaded must be provided
// the feature folder argument is to render the selected features in squared thumbnails
// the classifiers file argument correspond to the file path where the final data of the 
// classifier will be saved, if renderFeatures parameter is true, every feature will be saved as jpg image
void trainAdaBoost(unsigned int numOfFeatures, BinaryDataset *dataset, char *featureFolder, char *classifiersFile, bool renderFeatures);

// get the final classifier after the training phase
void getFinalClassifier(StrongClassifier **outClassifier);

// load a strong classifier from a data file
void loadStrongClassifier(char *classifiersFile, StrongClassifier **classifier);

// after a training a classifier this function test it over a test data set which must 
// provided, the classifiers file must by provided in the second argument and the dimension
// as the third argument correspond to the size of the images of the test data set
void testAdaBoost(BinaryDataset **testDataset, char *classifiersFile, unsigned int dimension);

// free all memory resources from the final classifier
void closeAdaBoost();

#endif
