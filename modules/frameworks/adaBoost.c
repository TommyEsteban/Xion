/**
* adaBoost.c
* Guardian Kids
* NestNet Group
* Ada Boost classification as described by Viola Jones
*/

#include "../../include/adaBoost.h"
#include "../../include/haarFeatureGenerator.h"
#include "../../include/timeEval.h"

#define BUFFER 20

// the features
HaarFeature *features;

// the final strong classifier
StrongClassifier *finalClassifier;

// number of samples
unsigned int n;

// dimension of images
unsigned int d;

bool parallelMode;				// if this is true parallelization is used in the training phase
float *e;		                // errors
float *b;		                // betas
float *g;		                // feature values
float *w;		                // weights
unsigned int *indexes;	                // indexes of elements
float *bestThresholds;	        // best thresholds per iteration
int *bestFps;	                // best false positives per iteration
int *bestFns;	                // best false negatives per iteration
int *bestFeatures;	            // best feature indexes per iteration
BYTE *bestPolarities;           // best polarities per iteration
unsigned int iterations	= 0;	// the number of iterations expected (maximum features expected)
int lastIteration = 0;	        // this is the last completed iteration in the training phase
float T1 = 0.0, T0 = 0.0;	    // total positive weights per iteration
float totalWeights = 0.0;	    // total sum of weights per iteration

void initializeClassifier(StrongClassifier **classifier)
{
	(*classifier) = (StrongClassifier *)malloc(sizeof(StrongClassifier));
	
	if((*classifier) == NULL)
	{
		printf("error initializing final Classifier\n");
		return;
	}
	
	(*classifier)->alpha = (float *)malloc(sizeof(float) * iterations);
	
	if((*classifier)->alpha == NULL)
	{
		printf("error initializing finalClassifier->alpha\n");
		return;
	}
	
	(*classifier)->weakClassifiers = (WeakClassifier *)malloc(sizeof(WeakClassifier) * iterations);
	
	if((*classifier)->weakClassifiers == NULL)
	{
		printf("error initializing finalClassifier->weakClassifiers\n");
		return;
	}
	
	(*classifier)->count = iterations;
}

void initializeAdaBoost(unsigned int numOfIterations, BinaryDataset *dataset, bool inParallelMode)
{
	parallelMode = inParallelMode;
	iterations = numOfIterations;
	n = dataset->n;
	d = dataset->d;
	//lastIteration = iterations;
	
	//get the generated features
	getFeatures(&features);
	
	e = (float *)malloc(sizeof(float) * iterations);
	b = (float *)malloc(sizeof(float) * iterations);
    w = (float *)malloc(sizeof(float) * dataset->n);
	
	if(!parallelMode)
	{
		g = (float *)malloc(sizeof(float) * dataset->n);
		indexes = (unsigned int *)malloc(sizeof(unsigned int) * dataset->n);
	}
	
    bestThresholds = (float *)malloc(sizeof(float) * iterations);
    bestFps = (int *)malloc(sizeof(int) * iterations);
    bestFns = (int *)malloc(sizeof(int) * iterations);
    bestFeatures = (int *)malloc(sizeof(int) * iterations);
    bestPolarities = (BYTE *)malloc(sizeof(BYTE) * iterations);
	
	if(e == NULL || b == NULL || w == NULL)
	{
		printf("error initializing pointers\n");
		return;
	}
	
	for(unsigned int T = 0; T < iterations; T++)
	{
		/*w[T] = malloc(sizeof(float) * dataset->n);
		
		if(w[T] == NULL)
		{
			printf("error initializing weight pointers\n");
			return;
		}*/
		
		// initialize error in all iterations to a high number
		e[T] = UPPERLIMITERROR;
	}

	float w1 = 1.0 / (2.0 * dataset->n1);
	float w0 = 1.0 / (2.0 * dataset->n0);
	//float w1 = 1.0 / dataset->n;
	//float w0 = 1.0 / dataset->n;

	for(unsigned int i = 0; i < dataset->n; i++)
	{
		// initialize weights
		if(dataset->Y[i] == 1)
			w[i] = w1;
		else
			w[i] = w0;
		
		 // sum weights
		totalWeights += w[i];
		
		if(!parallelMode)
			indexes[i] = i;
	}
	
	for(unsigned int i = 0; i < dataset->n; i++)
	{
		// weights normalization
		w[i] = w[i] / totalWeights;
		
		// sum of positive weights and negative ones
		if(dataset->Y[i] == 1)
			T1 += w[i];
		else
			T0 += w[i];
	}
	
	initializeClassifier(&finalClassifier);
}

void initializeAdaBoostForTest(unsigned int numberOfTestSamples, unsigned int dimension)
{
	n = numberOfTestSamples;
	d = dimension;
	
	if(features == NULL)
		getFeatures(&features);
}

void freeVariables()
{
	/*for(int i = 0; i < iterations; i++)
	{
		free(w[i]);
	}*/
	
	free(e);
	free(b);
	free(w);
	
	if(!parallelMode)
	{
		free(g);
		free(indexes);
	}
    
    free(bestThresholds);
    free(bestFps);
    free(bestFns);
    free(bestFeatures);
    free(bestPolarities);
	
	lastIteration = 0;
	totalWeights = 0.0;
	T1 = 0.0;
	T0 = 0.0;
	iterations = 0.0;
}

void freeClassifier()
{
	free(finalClassifier->alpha);
	free(finalClassifier->weakClassifiers);
	free(finalClassifier);
}

void closeAdaBoost()
{
	freeVariables();
	freeClassifier();
}

UBYTE classifyWeakly(float *X, unsigned int index, WeakClassifier *classifier)
{
	float g = computeFeatureValue(&classifier->feature, X, index, d);
	//printf("g: %.6f\n", g);
	if(classifier->polarity * g < classifier->polarity * classifier->threshold)
		return 1;
	else
		return 0;
}

UBYTE classifyAdaBoost(float *X, unsigned int index, StrongClassifier *classifier)
{
	//printf("*c: %p\n", classifier);
    //return 0;
    float total = 0.0;
    //printf("count: %d\n", classifier->count);
	for(unsigned int i = 0; i < classifier->count; i++)
	{
		total += classifier->alpha[i] * classifyWeakly(X, index, &classifier->weakClassifiers[i]);
	}
	//printf("total: %.6f, threshold: %.6f\n", total, classifier->threshold);
	if(total >= classifier->threshold)
	{
		//printf("prediction: 1\n");
		return 1;
	}
	else
	{
		//printf("prediction: 0\n");
		return 0;
	}
}

UBYTE decisionFunction(float value, float threshold, int polarity)
{
	if(polarity * value < polarity * threshold)
		return 1;
	else
		return 0;
}

void defineStrongClassificator()
{
	finalClassifier->threshold = 0.0;
	
	for(int i = 0; i < lastIteration; i++)
	{
		finalClassifier->alpha[i] = log(1.0 / b[i]);
		finalClassifier->threshold += finalClassifier->alpha[i];
	}
	
	finalClassifier->threshold = 0.5 * finalClassifier->threshold;
}

void mergeForSorting(float *A, unsigned int *I, float *L, unsigned int *Li, int leftCount, float *R, unsigned int *Ri, int rightCount) 
{
	// i - to mark the index of left subarray (L)
	// j - to mark the index of right subarray (R)
	// k - to mark the index of merged subarray (A)
	int i = 0, j = 0, k = 0;

	while(i < leftCount && j < rightCount) 
	{
		if(L[i] < R[j]) 
		{
			A[k] = L[i];
			I[k++] = Li[i++];
		}
		else 
		{
			A[k] = R[j];
			I[k++] = Ri[j++];
		}
	}
	
	while(i < leftCount) 
	{
		A[k] = L[i];
		I[k++] = Li[i++];
	}
	
	while(j < rightCount)
	{
		A[k] = R[j];
		I[k++] = Ri[j++];
	}
}

void sortFeatureValues(float *A, unsigned int *I, float n) 
{
	int mid, i; 
	float *L, *R;
	unsigned int *Li, *Ri;
	
	if(n < 2) return; // base condition. If the array has less than two element, do nothing. 

	mid = n/2;  // find the mid index. 

	// create left and right subarrays
	// mid elements (from index 0 till mid-1) should be part of left sub-array 
	// and (n-mid) elements (from mid to n-1) will be part of right sub-array
	L = (float *)malloc(mid * sizeof(float));
	R = (float *)malloc((n - mid) * sizeof(float));
	Li = (unsigned int *)malloc(mid * sizeof(unsigned int));
	Ri = (unsigned int *)malloc((n - mid) * sizeof(unsigned int)); 
	
	if(L == NULL || R == NULL || Li == NULL || Ri == NULL)
	{
		printf("error initializing pointers for sorting\n");
		return;
	}
	
	for(i = 0; i < mid; i++) 
	{
		L[i] = A[i]; // creating left subarray
		Li[i] = I[i]; // creating left subarray of indexes
	}
	
	for(i = mid; i < n; i++) 
	{
		R[i - mid] = A[i]; // creating right subarray
		Ri[i - mid] = I[i]; // creating right subarray of indexes
	}

	sortFeatureValues(L, Li, mid);  // sorting the left subarray
	sortFeatureValues(R, Ri, n - mid);  // sorting the right subarray
	mergeForSorting(A, I, L, Li, mid, R, Ri, n - mid);  // Merging L and R into A as sorted list.
	free(L);
	free(R);
	free(Li);
	free(Ri);
}

void saveFeature(int featureIndex, int iteration, HaarRectangle *rectangles, unsigned int numberOfRects, char *destinationFolder)
{
	// feature size of 24 and bits per pixel 3
	int totalPixels = 24 * 24 * 3;
	unsigned char *pixels = (unsigned char *)malloc(totalPixels);

	if(pixels == NULL)
	{
		printf("error initializing pixels pointer\n");
		return;
	}
	
	// index for pixel array and rgb initial values
	int index = 0;
	unsigned char r = 120, g = 120, b = 120;
	
	for(int y = 0; y < 24; y++)
	{
		for(int x = 0; x < 24; x++)
		{
			for(unsigned int rIndex = 0; rIndex < numberOfRects; rIndex++)
			{
				// if point collides with rectangle change rgb values
				if(x >= rectangles[rIndex].x && x <= rectangles[rIndex].x + rectangles[rIndex].width
				&& y >= rectangles[rIndex].y && y <= rectangles[rIndex].y + rectangles[rIndex].height)
				{
					if(rectangles[rIndex].isBlack)
					{
						r = 120; g = 60; b = 60;
					}
					else
					{
						r = 160; g = 100; b = 100;
					}
				}
			}
			
			pixels[index] = r;
			pixels[index + 1] = g;
			pixels[index + 2] = b;
			r = 120, g = 120, b = 120;
			index += 3;
		}
	}

	int dflength = strlen(destinationFolder);
	int indexLength = 0;
	
	if(featureIndex < 10)
		indexLength = 1;
	else if(featureIndex < 100)
		indexLength = 2;
	else if(featureIndex < 1000)
		indexLength = 3;
	else if(featureIndex < 10000)
		indexLength = 4;
	else if(featureIndex < 100000)
		indexLength = 5;
	else if(featureIndex < 1000000)
		indexLength = 6;
	else if(featureIndex < 10000000)
		indexLength = 7;
		
	int iterationLength = 0;
	
	if(iterationLength < 10)
		iterationLength = 1;
	else if(iterationLength < 100)
		iterationLength = 2;
	else if(iterationLength < 1000)
		iterationLength = 3;
	else if(iterationLength < 10000)
		iterationLength = 4;
	else if(iterationLength < 100000)
		iterationLength = 5;
	else if(iterationLength < 1000000)
		iterationLength = 6;
	else if(iterationLength < 10000000)
		iterationLength = 7;
	
	char *filePath = (char *)malloc(dflength + indexLength + 4 + 10);
	filePath[0] = '\0';
	
	if(filePath == NULL)
	{
		printf("error initializing filePath pointer\n");
		return;
	}
	
	char fileName[indexLength + iterationLength + 1];
	sprintf(fileName, "%d_%d", iteration, featureIndex);
	strcat(filePath, destinationFolder);
	strcat(filePath, fileName);
	strcat(filePath, ".jpg");

	//printf("writing feature image file to %s\n", filePath);
	writeJpegFile(filePath, pixels, 24, 24, 3);
	
	free(pixels);
}

void updateWeights(unsigned int iterationIndex, WeakClassifier *classifier, BinaryDataset *dataset)
{
	T0 = 0.0, T1 = 0.0;
	//int tp = 0, tn = 0, fp = 0, fn = 0;
	totalWeights = 0.0;

	for(unsigned int i = 0; i < dataset->n; i++)
	{
		UBYTE prediction = classifyWeakly(dataset->XP, i, classifier);
		
		// when the prediction is correct, the weight is decreased by beta factor
		if(prediction == dataset->Y[i])
		{
			w[i] = w[i] * b[iterationIndex];

			//if(prediction == 1) tp++;
			//if(prediction == 0) tn++;
		}
		
		// when the prediction is wrong, the weight is the same but after normalization
		// the weight is increased
		else
		{
			w[i] = w[i];
			
			//if(prediction == 1) fp++;
			//if(prediction == 0) fn++;
		}
		
		totalWeights += w[i];
	}

	for(unsigned int i = 0; i < dataset->n; i++)
	{
		// weights normalization
		w[i] = w[i] / totalWeights;

		// sum positive weights and negative ones
		if(dataset->Y[i] == 1)
			T1 += w[i];
		else
			T0 += w[i];
	}
}

void trainAdaBoostSequential(unsigned int numOfFeatures, BinaryDataset *dataset, char *featureFolder, char *classifiersFile, bool renderFeatures)
{
	struct timeval tvBegin, tvEnd, tvDiff;
    
    // control the number of features
    if(numOfFeatures > iterations)
    {
        printf("error: the number of features for this call is greater than the maximum expected\n");
        return;
    }
    
    // false positives, false negatives and counters for positive and negative samples
	int fp = 0, fn = 0, l1 = 0, l0 = 1;
    
    // variables for error calculation
	float error = 0.0, S1 = 0.0, S0 = 0.0, negBelowPosAbove = 0.0, posBelowNegAbove = 0.0;
	
    // variable to set the polarity per sample
    BYTE p = -1;
	
	printf("training %d features...\n", numOfFeatures);

	// go through the iterations
	for(unsigned int T = 0; T < numOfFeatures; T++)
	{
		if(fabs(e[T] - UPPERLIMITERROR) < 0.00001)
		{
			// initialize error with random large number
			error = 0.0;
			gettimeofday(&tvBegin, NULL);
			
			// evaluate feature values for every feature
			for(unsigned int f = 0; f < getFeaturesLength(); f++)
			{
				//if(T == 14 && f == 133){ for(int i = 0; i < dataset->n; i++) printf("w: %.6f\n", w[i]); };
				p = 0, S1 = 0, S0 = 0, l1 = 0, l0 = 0;
				
				// compute all feature values
				for(unsigned int i = 0; i < dataset->n; i++)
				{
					indexes[i] = i;
					g[i] = computeFeatureValue(&features[f], dataset->XP, i, dataset->d);
				}
				
				// sort the feature values and the indexes
				sortFeatureValues(g, indexes, dataset->n);
				
				/*if(f == 10818)
				{
					for(int i = 5000; i < 5550; i++)
						//printf("i[%d]: %d, g[%d]: %.6f\n", i, indexes[i], i, g[i]);
						printf("%d: %d, g: %.12f\n", i, indexes[i], g[i]);
				}*/
				
				// go through the elements
				for(unsigned int j = 0; j < dataset->n - 1; j++)
				{
					if(dataset->Y[indexes[j]] == 0)
					{
						S0 += w[indexes[j]];
						l0++;
					}
					
					if(dataset->Y[indexes[j]] == 1)
					{
						S1 += w[indexes[j]];
						l1++;
					}
					//if(f == 10818 && j == 5543) printf("j: %d, index: %d, Y; %d, W: %.12f, S1: %.12f, S0: %.12f\n", j, indexes[j], dataset->Y[indexes[j]], w[indexes[j]], S1, S0);
					posBelowNegAbove = (S0 + (T1 - S1));
					negBelowPosAbove = (S1 + (T0 - S0));
					
					if(posBelowNegAbove < negBelowPosAbove)
					{
						error = posBelowNegAbove;
						fp = l0;
						fn = dataset->n1 - l1;
						p = 1;
						//if(f == 10818 && j == 5543){ printf("error-> %.12f = %.12f + (%.12f - %.12f)\n", posBelowNegAbove, S0, T1, S1); };
					}
					else
					{
						error = negBelowPosAbove;
						fp = dataset->n0 - l0;
						fn = l1;
						p = -1;
					}
					
					//if(f == 10818 && j == 5543){ printf("%d, error: %.6f\n", j, error); };
					
					//if(f == 76314)
						//printf("%d: w[T]: %.6f\n", (j - 1), w[T][j - 1]);
						//printf("%d: S1: %.6f, S0: %.6f\n", j, S1, S0);
					
					//if(f == 36564)
						//printf("%d: S1: %.6f, S0: %.6f, T1: %.6f, T0: %.6f, error: %.6f, Y: %d, index: %d, g: %.6f\n", j, S1, S0, T1, T0, error, dataset->Y[indexes[j - 1]], indexes[j - 1], g[j - 1]);
					
					// capture the best values when the error is improved
					if(error > 0 && error < e[T])
					{
						bestFeatures[T] = f;
						bestPolarities[T] = p;
						bestThresholds[T] = g[j + 1];
						bestFps[T] = fp;
						bestFns[T] = fn;
						//bestIndex = j;
						e[T] = error;
					}
				}
				//if(f == 1){
				//printf("f: %d, error: %.6f\n", f, e[T]);char cc = getchar(); cc++;}
			}
			
			if(renderFeatures)
				saveFeature(bestFeatures[T], T, features[bestFeatures[T]].rectangles, features[bestFeatures[T]].numberOfRects, featureFolder);
			
			// after all iterations have been run, the weak classifier is defined
			WeakClassifier classifier;
			classifier.polarity = bestPolarities[T];
			classifier.threshold = bestThresholds[T];
			classifier.feature = features[bestFeatures[T]];
			//serializeValues(bestPolarities[T], bestThresholds[T], bestFeatures[T], classifiersFile);
			
			// add the weak classifier to the final string classifier
			finalClassifier->weakClassifiers[T] = classifier;
			
			// compute the beta values for the current iteration
			b[T] = e[T] / (1.0 - e[T]);
			
			// update weights only if the current iteration is not the last one
			if(T < iterations - 1)
				updateWeights(T, &classifier, dataset);
			
			gettimeofday(&tvEnd, NULL);
			timeEvalSubtract(&tvDiff, &tvEnd, &tvBegin);
			printf("Time: %ld.%06ld, ", tvDiff.tv_sec, tvDiff.tv_usec);
			printf("%d feature=%d error=%.6f beta=%.6f threshold=%.12f polarity=%d \n", (T + 1), bestFeatures[T], e[T], b[T], bestThresholds[T], bestPolarities[T]);
		}
        
		serializeValues(bestPolarities[T], bestThresholds[T], bestFeatures[T], classifiersFile);
		lastIteration++;
		//printf("T=%d e=%.24f beta=%.6f bF=%d th=%.6f p=%d fn=%d, fp=%d ::: w[456]=%.24f\n", T, e[T], b[T], bestFeatureIndex, bestThreshold, bestPolarity, bestFn, bestFp, w[T][456]);
	}
	
	defineStrongClassificator();
	serializeArray(finalClassifier->alpha, numOfFeatures, classifiersFile);
    lastIteration = 0;
}

void trainAdaBoostInParallel(unsigned int numOfFeatures, BinaryDataset *dataset, char *featureFolder, char *classifiersFile, bool renderFeatures)
{
	struct timeval tvBegin, tvEnd, tvDiff;
    
    // control the number of features
    if(numOfFeatures > iterations)
    {
        printf("error: the number of features for this call is greater than the maximum expected\n");
        return;
    }
	
	//allocate necesary memory for parallelization
	
	initParallelization(dataset->XP, dataset->Y, dataset->n, dataset->d);
    
	//unsigned int j = 0;		// the index of the lower error found
	float error = 0.0;		// error calculation
	float threshold = 0.0;	// the best threshold found
    char p = -1;			// polarity

	//printf("features: %d\n", getFeaturesLength());
	printf("training %d features...\n", numOfFeatures);
	
	// go through the iterations
	for(unsigned int T = 0; T < numOfFeatures; T++)
	{//printf("e[%d]: %.24f\n", T, e[T]); char c = getchar(); c++;
		if(fabs(e[T] - UPPERLIMITERROR) < 0.00001)
		{
			// initialize error with random large number
			error = UPPERLIMITERROR;
			gettimeofday(&tvBegin, NULL);
			
			// evaluate feature values for every feature
			for(unsigned int f = 0; f < getFeaturesLength(); f++)
			{
				//gettimeofday(&tvBegin, NULL);
				computeFeatureValuesParallel(f, T);
				sortFeatureValuesParallel();
				computeErrorParallel(&w, T1, T0, &error, &threshold, &p);

				if(error > 0 && error < e[T])
				{
					bestFeatures[T] = f;
					bestPolarities[T] = p;
					bestThresholds[T] = threshold;
					e[T] = error;
				}
				//gettimeofday(&tvEnd, NULL);
				//timeEvalSubtract(&tvDiff, &tvEnd, &tvBegin);
				//printf("Time: %ld.%06ld, \n", tvDiff.tv_sec, tvDiff.tv_usec);
			}
			
			if(renderFeatures)
				saveFeature(bestFeatures[T], T, features[bestFeatures[T]].rectangles, features[bestFeatures[T]].numberOfRects, featureFolder);
			
			// after all iterations have been run, the weak classifier is defined
			WeakClassifier classifier;
			classifier.polarity = bestPolarities[T];
			classifier.threshold = bestThresholds[T];
			classifier.feature = features[bestFeatures[T]];

			// add the weak classifier to the final strong classifier
			finalClassifier->weakClassifiers[T] = classifier;
			
			// compute the beta values for the current iteration
			b[T] = e[T] / (1.0 - e[T]);
			
			// update weights only if the current iteration is not the last one
			if(T < iterations - 1)
				updateWeights(T, &classifier, dataset);
			
			gettimeofday(&tvEnd, NULL);
			timeEvalSubtract(&tvDiff, &tvEnd, &tvBegin);
			printf("Time: %ld.%06ld, ", tvDiff.tv_sec, tvDiff.tv_usec);
			printf("%d feature=%d error=%.6f beta=%.6f threshold=%.12f polarity=%d \n", (T + 1), bestFeatures[T], e[T], b[T], bestThresholds[T], bestPolarities[T]);
		}
        
		serializeValues(bestPolarities[T], bestThresholds[T], bestFeatures[T], classifiersFile);
		lastIteration++;
	
	}
	
	defineStrongClassificator();
	serializeArray(finalClassifier->alpha, numOfFeatures, classifiersFile);
    lastIteration = 0;
	
	// free memory from device used in parallelization
	closeParallelization();
}

void trainAdaBoost(unsigned int numOfFeatures, BinaryDataset *dataset, char *featureFolder, char *classifiersFile, bool renderFeatures)
{
	if(!parallelMode)
		trainAdaBoostSequential(numOfFeatures, dataset, featureFolder, classifiersFile, renderFeatures);
	else
		trainAdaBoostInParallel(numOfFeatures, dataset, featureFolder, classifiersFile, renderFeatures);
}

void getFinalClassifier(StrongClassifier **outClassifier)
{
	(*outClassifier) = finalClassifier;
}

void loadStrongClassifier(char *classifiersFile, StrongClassifier **classifier)
{
	unsigned int linesTotal = 0;
	int c = 0;
	char value[BUFFER];
	unsigned int index = 0, valuesCounter = 0, rowIndex = 0, alphaIndex = 0;
	UBYTE polarity = 0;
	float threshold = 0.0;
	float alpha = 0.0;
	int featureIndex = 0;
	
	// open the file
	FILE *input = fopen(classifiersFile, "r");
	
	// check for the availability of the data file
	if(input == NULL)
	{
		printf("data file could not be open\n");
		return;
	}
	
	printf("accessing classifiers data file: %s\n", classifiersFile);
	
	// count the lines of the file
	while(!feof(input))
	{
	  c = fgetc(input);
	  
	  if(c == '\n')
	  {
		linesTotal++;
	  }
	}
	
	iterations = linesTotal - 1;
	initializeClassifier(classifier);
	rewind(input);
	
	// read the data file char by char
	while ((c = fgetc(input)) != EOF)
    {
		// in case a comma is found is just after a number
		if((char)c == ',')
		{
			// mark the end of the word
			value[index] = '\0';
			
			if(rowIndex < linesTotal - 1)
			{
				if(valuesCounter == 0)
					polarity = atoi(value);
				if(valuesCounter == 1)
					sscanf(value, "%f", &threshold);
				if(valuesCounter == 2)
					featureIndex = atoi(value);

				//printf("value: %s, polarity: %d threshold: %.6f, fIndex: %d\n", value, polarity, threshold, featureIndex);
				//char c = getchar(); c++;
			}
			
			// the last line correspond to the alpha values
			else
			{
				sscanf(value, "%f", &alpha);
				(*classifier)->alpha[alphaIndex] = alpha;
				alphaIndex++;
			}
			
			valuesCounter++;
			index = 0;
		}
		
		// in case a return character is found a new weak classifier data is filled
		else if((char)c == '\n')
		{
			if(rowIndex < linesTotal - 1)
			{
				WeakClassifier wClass;
				wClass.polarity = polarity; 
				wClass.threshold = threshold;
				wClass.feature = features[featureIndex]; 
				(*classifier)->weakClassifiers[rowIndex] = wClass;
			}
			
			value[index] = '\0';
			index = 0;
			rowIndex++;
			valuesCounter = 0;
		}
		
		// otherwise set next character into the auxiliar array
		else
		{
			value[index] = (char)c;
			index++;
		}
	}
	
	fclose(input);
    
    // compute the threshold
    (*classifier)->threshold = 0.0;
	
	for(unsigned int i = 0; i < (*classifier)->count; i++)
	{
		(*classifier)->threshold += (*classifier)->alpha[i];
	}

	(*classifier)->threshold = 0.5 * (*classifier)->threshold;
}

void testAdaBoost(BinaryDataset **testDataset, char *classifiersFile, unsigned int dimension)
{
	n = (*testDataset)->n;
	d = (*testDataset)->d;
	
	//get the generated features
	if(features == NULL)
		getFeatures(&features);
	
	// load the final classifier from the training phase
	if(finalClassifier == NULL)
		loadStrongClassifier(classifiersFile, &finalClassifier);

	/*printf("th: %.6f, alpha[0]: %.6f, count: %d, WP: %d, WTh: %.6f \n", 
	finalClassifier->threshold, finalClassifier->alpha[0], finalClassifier->count,
	finalClassifier->weakClassifiers[0].polarity, finalClassifier->weakClassifiers[0].threshold);*/
	
    int pass = 0, fail = 0, tp = 0, tn = 0, fp = 0, fn = 0;
	float detectionRate = 0.0, falsePositiveRate = 0.0;
	UBYTE prediction = 0;

	for(unsigned int i = 0; i < n; i++)
	{
		prediction = classifyAdaBoost((*testDataset)->XP, i, finalClassifier);
		//prediction = classifyAdaBoost((*testDataset)->XN[i], finalClassifier);
		//printf("%d prediction: %d, reality: %d\n", i, prediction, (*testDataset)->Y[i]);
		if(prediction == (*testDataset)->Y[i])
		{
			pass++;
			
			if((*testDataset)->Y[i] == 1)
				tp++;
			else
				tn++;
		}
		else
		{
			fail++;
			
			if((*testDataset)->Y[i] == 1)
				fn++;
			else
				fp++;
		}
	}
	
    printf("\nResults\n");
	detectionRate = (float)pass / n;
	falsePositiveRate = (float)fp / (*testDataset)->n0;
	printf("%d positive samples, %d negative samples\n", (*testDataset)->n1, (*testDataset)->n0);
	printf("%d correct, %d wrong, tp=%d tn=%d fn=%d fp=%d\n", pass, fail, tp, tn, fn, fp);
	printf("detection rate: %.6f, fp rate: %.6f\n", detectionRate, falsePositiveRate);
	freeClassifier();
}
