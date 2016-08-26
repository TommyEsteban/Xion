/**
* attentionalCascade.c
* Guardian Kids
* NestNet Group
* Attentional Cascade as described by Viola Jones
*/

#include "../../include/attentionalCascade.h"

// the final strong classifier
StrongClassifier *adaClassifier;

// the final cascade classifier
StrongClassifier *cascadeClassifier;

bool isParallelMode = false;

// the minumum amount of data for parallelization
unsigned int minDataForParallelization = 3000;

// the number of layers in the cascade classifier
unsigned int layers;

void initializeCascade(unsigned int numOfLayers)
{
    // initialize the cascade classifier which is a collection of strong classifiers
	cascadeClassifier = (StrongClassifier *)malloc(sizeof(StrongClassifier) * numOfLayers);
	
	if(cascadeClassifier == NULL)
	{
		printf("error initializing cascade classifier\n");
		return;
	}
}

void prepareClassifiersFile(char *classifiersFile, char *baseClassifiersFile, unsigned int numLayer)
{
	// this is to convert the layer number into a two char string for the prefix
	// of the classifier file name
	char number[10];
	
	sprintf(number, "%02d", numLayer);
	classifiersFile[0] = '\0';
	strcat(classifiersFile, baseClassifiersFile);
	strcat(classifiersFile, number);
	
    FILE *file = fopen(classifiersFile, "w");
    fclose(file);
	/*FILE *file = fopen(classifiersFile, "r");
	
	if (file != NULL)
    {
        fclose(file);
        remove(classifiersFile);
    }*/
}

void addClassifierToCascade(StrongClassifier *classifier, unsigned int layerIndex, unsigned int fCounter)
{    
    // create a new string classifier
	StrongClassifier c;
    
    // allocate memory for alpha vector
	c.alpha = (float *)malloc(sizeof(float) * fCounter);
	
	if(c.alpha == NULL)
	{
		printf("error initializing finalClassifier->alpha\n");
		return;
	}
	
    // allocate memory for weak classifiers
	c.weakClassifiers = (WeakClassifier *)malloc(sizeof(WeakClassifier) * fCounter);
	
	if(c.weakClassifiers == NULL)
	{
		printf("error initializing finalClassifier->weakClassifiers\n");
		return;
	}
	
	// copy values of the classifier into the new one
	for(unsigned int i = 0; i < fCounter; i++)
	{
		// copy the alpha values
		c.alpha[i] = classifier->alpha[i];
		
		// copy the values of the weak classifiers
		WeakClassifier weakC;
		weakC.polarity = classifier->weakClassifiers[i].polarity;
		weakC.threshold = classifier->weakClassifiers[i].threshold;
		weakC.feature = classifier->weakClassifiers[i].feature;
		c.weakClassifiers[i] = weakC;
	}
	
	// copy the count and the threshold values
	c.count = fCounter;
	c.threshold = classifier->threshold;
	//printf("layerIndex: %d\n", layerIndex);
	// add the strong classifier to the cascade
	cascadeClassifier[layerIndex] = c;
}

UBYTE predictWithCascade(float *X, unsigned int index, unsigned int numOfLayers, StrongClassifier **classifier)
{   
	for(unsigned int i = 0; i < numOfLayers; i++)
	{//printf("c: %p\n", &cascadeClassifier[i]);
		UBYTE prediction = classifyAdaBoost(X, index, &(*classifier)[i]);
        //printf("prediction: %d, layer: %d, count: %d\n", prediction, (i + 1), (*classifier)[i].count);
		if(prediction == 0)
			return 0;
	}
	
	return 1;
}

void computeDetectionRate(BinaryDataset *vDataset, float *d, float *fp)
{
    int falsePositives = 0;
	int truePositives = 0;
    int n = vDataset->n;

	for(int i = 0; i < n; i++)
	{
		UBYTE prediction = predictWithCascade(vDataset->X, i, layers + 1, &cascadeClassifier);
		//printf("%d prediction: %d, reality: %d\n", i, prediction, vDataset->Y[i]);
		if(prediction == vDataset->Y[i] && prediction == 1)
			truePositives++;
		if(prediction != vDataset->Y[i] && prediction == 1)
			falsePositives++;
	}

	*d = (float) truePositives / vDataset->n1;
	*fp = (float) falsePositives / vDataset->n0;
}

bool remakeNegativeSamples(BinaryDataset *tDataset)
{
    // counter for false detections in negative samples
    unsigned int fpCounter = 0;
	UBYTE *predictions = (UBYTE *)malloc(sizeof(UBYTE) * tDataset->n);
	
	// get all predictions
    for(unsigned int i = 0; i < tDataset->n; i++)
	{
		predictions[i] = predictWithCascade(tDataset->X, i, layers, &cascadeClassifier);
		//printf("%d prediction: %d, reality: %d\n", i, prediction, tDataset->Y[i]);
		
		// count false positives
		if(tDataset->Y[i] == 0 && predictions[i] == 1)
		{
			fpCounter++;
		}
	}
	
	if(fpCounter == 0)
		return true;
	
	// the new size of the dataset
	unsigned int newSize = tDataset->n1 + fpCounter;
	
	// extra data for parallel mode
	unsigned int extra = 0;
	
	// if new data set is larger than 2048 the parallelization worth
	if(isParallelMode && newSize > minDataForParallelization)
	{
		int div = ceil((float)newSize / 1024);
		extra = div * 1024 - newSize;
		printf("newSize: %d, div: %d, extra: %d, fp: %d\n", newSize, div, extra, fpCounter);
		newSize = newSize + extra;
	}
	
	// otherwise cancel the parallelization
	else if(isParallelMode && newSize <= minDataForParallelization)
	{
		isParallelMode = false;
	}

    // prepare the new pointers to modify the dataset
	//printf("newSize: %d, p: %d\n", newSize, tDataset->p);
	float *XCopy = (float *)malloc(sizeof(float) * newSize * tDataset->p);
    //float ***XCopy = (float ***)malloc(sizeof(float **) * (newSize));
    UBYTE *YCopy = (UBYTE *)malloc(sizeof(UBYTE) * (newSize));
	
	if(XCopy == NULL || YCopy == NULL)
	{
		fprintf(stderr, "error initializing copy of X, Y matrices \n");
		exit(EXIT_FAILURE);
	}
    
    // the counter data for the new dataset and extra counter for extra data in parallelization
    unsigned int counter = 0;
	unsigned int counterExtra = 0;

	// fill the data
    for(unsigned int i = 0; i < tDataset->n; i++)
	{
		//printf("%d - prediction: %d, reality: %d\n", i, predictions[i], tDataset->Y[i]);
		if(tDataset->Y[i] == 1 || (tDataset->Y[i] == 0 && predictions[i] == 1))
		{
			int stride = tDataset->p * counter;
			//printf("%d stride: %d\n", counter, stride);
			for(unsigned int p = stride; p < stride + tDataset->p; p++)
				XCopy[p] = tDataset->X[p];
			
			YCopy[counter] = tDataset->Y[i];
            counter++;
        }
    }
	
	// fill the extra data only for parallelization mode
	if(isParallelMode && extra > 0)
	{
		for(unsigned int i = 0; i < tDataset->n; i++)
		{
			//printf("%d: reality: %d, prediction: %d \n", i, tDataset->Y[i], predictions[i]);
			if(tDataset->Y[i] == 0 && predictions[i] == 1)
			{
				int stride = tDataset->p * i;
			
				for(unsigned int p = stride; p < stride + tDataset->p; p++)
					XCopy[p] = tDataset->X[p];
				
				YCopy[counter] = tDataset->Y[i];
				counter++;
				counterExtra++;
			}
			
			if(counterExtra == extra)
				break;
			
			if(i == tDataset->n - 1 && counterExtra < extra)
			{
				i = 0;
				//printf("counterExtra: %d, extra: %d, counter: %d\n", counterExtra, extra, counter);
				//char c = getchar(); c++;
			}
		}
	}
    
	free(predictions);
	
	//printf("free memory from old XP data\n");
	
    // free memory resources used in the matrix of samples in the dataset
	//free(tDataset->XP);
	//free(tDataset->Y);
	
    printf("defining new dataset\n");
    // replace the dataset with the new data
    tDataset->X = XCopy;
    tDataset->Y = YCopy;
    tDataset->n0 = fpCounter + extra;
    tDataset->n = counter;
	
    printf("new dataset: %d samples, negative samples: %d, negative misclassified: %d\n", tDataset->n, tDataset->n0, fpCounter);

	return false;
}

void trainCascadeClassifier(BinaryDataset **tDataset, 
							BinaryDataset **vDataset, 
							float fi,
							float di,
							float Ft,
							char *featureFolder, 
							char *baseClassifiersFile,
							bool parallelMode)
{
	// initialize the string to keep the classifiers file names
	int len = strlen(baseClassifiersFile);
	char *classifiersFile = (char *)malloc(len + 3);
	
	if(classifiersFile == NULL)
	{
		printf("error initializing classifiers file string path\n");
		return;
	}
	
	isParallelMode = parallelMode;
	
	// global target detection rate and false positive target rate per layer
	float targetD = 0.0, targetF = 0.0, bestF = 1.0;
	
	// initialize the counter for the cascade layers
    layers = 0;
	
    // number of iterations (number of features)
    unsigned int t = 0;
        
	// current and last false positive rates
	float F[2] = {1.0, 1.0};
	
	// current and last detection rates
	float D[2] = {1.0, 1.0};
	
	// as long as the target overall false positive rate is not reached
	while(F[0] > Ft)
	{
		
        t = 0;					//set the number of features to zero for every new layer
		F[1] = F[0];			// update the last false positve rate
		D[1] = D[0];			// update the last detection rate
        targetF = fi * F[1];	// update the overall false positive target
        
        initializeAdaBoost(MAXFEATURES, *tDataset, isParallelMode);
	
		// as long as the current false positive rate per layer 
		// does not reach the target per layer
		while(F[0] > targetF && F[0] > Ft)
		{
			t++;	// add more features
            
            prepareClassifiersFile(classifiersFile, baseClassifiersFile, layers + 1);
			printf("Cascade in layer %d with %d features...\n", layers + 1, t);
			trainAdaBoost(t, *tDataset, featureFolder, classifiersFile, false);
			getFinalClassifier(&adaClassifier);
			addClassifierToCascade(adaClassifier, layers, t);
			
            // update the overall detection rate target
			targetD = di * D[1];
            
            // compute detection rate and false positive rate for current classifier
			computeDetectionRate(*vDataset, &D[0], &F[0]);
			printf("D: %.6f, targetD: %.6f, F: %.6f, targetF: %.6f\n", D[0], targetD, F[0], (fi * F[1]));

			while(D[0] < targetD)
			{	
				//printf("th: %.6f, ", cascadeClassifier[layers].threshold);
				cascadeClassifier[layers].threshold = cascadeClassifier[layers].threshold - 0.001;
				//printf("*th: %.6f, \n", cascadeClassifier[layers].threshold);
				computeDetectionRate(*vDataset, &D[0], &F[0]);
			}
			
			if(F[0] < bestF)
				bestF = F[0];
			
			if(t == 100)
				break;

			printf("D: %.6f, D: %.6f, F: %.6f, F Target: %.6f, F Global: %.6f, bestF: %.6f \n\n", D[0], targetD, F[0], targetF, Ft, bestF);
		}

        closeAdaBoost();
		layers++;
		bestF = 1.0;
		
		/*if(layers > 0)
			isParallelMode = false;*/

        if(F[0] > Ft)
		{
			bool noFalsePositive = remakeNegativeSamples(*tDataset);
			
			if(noFalsePositive)
			{
				printf("all negative samples has been classified\n");
				break;
			}
		}
		
		if(layers == 10)
			break;
	}

    // displaying final results
    printf("Cascade Classificator done.\n");
    int featCounter = 0;
    
    for(unsigned int i = 0; i < layers; i++)
    {
        printf("layer %d: %d features\n", (i + 1), cascadeClassifier[i].count);
        featCounter += cascadeClassifier[i].count;
    }
    
    free(classifiersFile);
}

void loadCascadeClassifier(char *baseClassifiersFile)
{
    layers = 0;
    int len = strlen(baseClassifiersFile);
	char *classifiersFile = (char *)malloc(len + 3);
    
	if(classifiersFile == NULL)
	{
		printf("error initializing classifiers file string path\n");
		return;
	}
    
    for(int l = 1; l < MAXLAYERS; l++)
    {
        char number[10];
        sprintf(number, "%02d", l);
        classifiersFile[0] = '\0';
        strcat(classifiersFile, baseClassifiersFile);
        strcat(classifiersFile, number);
        //printf("file: %s\n", classifiersFile);
        FILE *file = fopen(classifiersFile, "r");
        
        if (file != NULL)
        {
            fclose(file);
            layers++;
        }
		else{
			free(file);
			break;
		}
        
		//free(file);
    }
    
    initializeCascade(layers);
    
    for(unsigned int l = 1; l <= layers; l++)
    {
        char number[10];
        sprintf(number, "%02d", l);
        classifiersFile[0] = '\0';
        strcat(classifiersFile, baseClassifiersFile);
        strcat(classifiersFile, number);
        //printf("file: %s\n", classifiersFile);
        loadStrongClassifier(classifiersFile, &adaClassifier);
		
        addClassifierToCascade(adaClassifier, (l - 1), adaClassifier->count);
    }
}

void testCascadeClassifier(BinaryDataset **testDataset, char *baseClassifiersFile, unsigned int dimension)
{
	initializeAdaBoostForTest((*testDataset)->n, (*testDataset)->d);
	
	if(cascadeClassifier == NULL)
		loadCascadeClassifier(baseClassifiersFile);
	//printf("**: %p, *:%p, n: %d, n0: %d, n1:%d\n", testDataset, (*testDataset), (*testDataset)->n, (*testDataset)->n0, (*testDataset)->n1);
    int pass = 0, fail = 0, tp = 0, tn = 0, fp = 0, fn = 0;
	float detectionRate = 0.0, falsePositiveRate = 0.0;
	UBYTE prediction = 0;
	
	for(unsigned int i = 0; i < (*testDataset)->n; i++)
	{
        prediction = predictWithCascade((*testDataset)->X, i, layers, &cascadeClassifier);
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
	detectionRate = (float)pass / (*testDataset)->n;
	falsePositiveRate = (float)fp / (*testDataset)->n0;
	printf("%d positive samples, %d negative samples\n", (*testDataset)->n1, (*testDataset)->n0);
	printf("%d correct, %d wrong, tp=%d tn=%d fn=%d fp=%d\n", pass, fail, tp, tn, fn, fp);
	printf("detection rate: %.6f, fp rate: %.12f\n", detectionRate, falsePositiveRate);
}

void closeCascade()
{
    for(unsigned int l = 0; l < layers; l++)
    {
        free(cascadeClassifier[l].alpha);
        free(cascadeClassifier[l].weakClassifiers);
    }
    
    free(cascadeClassifier);
	
}
