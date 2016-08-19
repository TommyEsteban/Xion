/**
* adaBoostTypes.h
* Guardian Kids
* NestNet Group
* Types used in Ada Boost training
*/

#ifndef ADABOOSTTYPES_H
#define ADABOOSTTYPES_H

#include "haarFeature.h"

#define UPPERLIMITERROR 7.7

typedef char BYTE;

typedef struct WeakClassifier
{
	BYTE polarity;						// polarity value to control the decision
	float threshold;					// the treshold to decide the classification
	HaarFeature feature;				// the selected feature for this weak classifier
} WeakClassifier;

typedef struct StrongClassifier
{
	float *alpha;						// the alpha vector
	float threshold;					// the treshold to decide the classification
	unsigned int count;					// count of weak classifiers
	WeakClassifier *weakClassifiers;	// the weak classifiers for the strong classifier
} StrongClassifier;

#endif