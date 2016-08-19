/**
* haarFeatureGenerator.h
* Guardian Kids
* NestNet Group
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include "haarFeature.h"

#ifndef HAARFEATUREGENERATOR_H
#define HAARFEATUREGENERATOR_H

//#define TOTALFEATURES 162336

// generates the set of haar like features between a given frame
void buildFeatures(unsigned int frameWidth, unsigned int frameHeight, bool useOnlyTrainedFeatures, char *featuresFolder);

// free all memory resources used
void destroyFeatures();

// get the loaded features;
#ifdef __cplusplus
extern "C"
#endif
void getFeatures(HaarFeature **outFeatures);

// get the total of loaded features
#ifdef __cplusplus
extern "C"
#endif
unsigned int getFeaturesLength();

#endif
