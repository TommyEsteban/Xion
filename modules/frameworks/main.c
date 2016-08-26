/**
* NestNet Group
* Xion Object Detection Framework
* main.c
* main entry point for Xion
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/xionMessages.h"
#include "../../include/xionConfig.h"
#include "../../include/dataset.h"
#include "../../include/datasetValidator.h"
#include "../../include/datasetController.h"
#include "../../include/integralImage.h"
#include "../../include/normalization.h"
#include "../../include/haarFeature.h"
#include "../../include/haarFeatureGenerator.h"
#include "../../include/adaBoost.h"
#include "../../include/attentionalCascade.h"

#define MAX_MESSAGE_LENGTH 1024
#define MAX_CHARACTERS 255
#define MIN_ARGTRAINING_ADABOOST 13
#define MIN_ARGTRAINING_CASCADE 19
#define MIN_ARGTEST_ADABOOST 9
#define MIN_ARGTEST_CASCADE 9

int processDataCommand(int argc, char **argv);
void trainingWithAdaBoost(int argc, char **argv);
void testWithAdaBoost(int argc, char **argv);
void trainingWithCascade(int argc, char **argv);
void testWithCascade(int argc, char **argv);

int main (int argc, char **argv)
{
	printf("Xion Object Detection Framework, Version %d.%d\n", xion_VERSION_MAJOR, xion_VERSION_MINOR);

	// command for dataset generation
	if(strcmp(argv[1], "data") == 0)
	{
		processDataCommand(argc, argv);
	}

	if(strcmp(argv[1], "training") == 0)
	{
		if(strcmp(argv[2], "adaboost") == 0 && argc >= MIN_ARGTRAINING_ADABOOST)
		{
			trainingWithAdaBoost(argc, argv);
		}
		
		if(strcmp(argv[2], "cascade") == 0 && argc >= MIN_ARGTRAINING_CASCADE)
		{
			trainingWithCascade(argc, argv);
		}
	}

	else if(strcmp(argv[1], "test") == 0)
	{
		if(strcmp(argv[2], "adaboost") == 0 && argc >= MIN_ARGTEST_ADABOOST)
		{
			testWithAdaBoost(argc, argv);
		}
		
		if(strcmp(argv[2], "cascade") == 0 && argc >= MIN_ARGTEST_CASCADE)
		{
			testWithCascade(argc, argv);
		}
	}
	else
	{
		printf("The following arguments are required.\n");
		printf("\ttraining, test : one of these must be provided as the first argument.\n");
		printf("\tadaboost, cascade : one of these must be provided as the second argument.\n");
		printf("\t-size : the size of the square training or test images.\n");
		printf("\t-data : in training or test mode the correspond file of the dataset.\n");
		printf("\t-cl : in training this is the output classifiers file and in test mode the input classifiers file.\n");
		printf("\t-ff : this is the folder where the features images will be saved (only for training mode).\n");
		printf("\t-features : this is the number of features to generate in the training mode.\n");
	}
	
	printf("Press enter to exit.\n");
	char c = getchar();
	
	if(c == '\n')
		return 0;
}

int processDataCommand(int argc, char **argv)
{
	errorMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);
	warningMessage = (char *)malloc(sizeof(char) * MAX_ERROR_LENGTH);

	if(!dat_isValid(argv))
	{
		printf("%s", errorMessage);
		free(errorMessage);
		free(warningMessage);
		return EXIT_FAILURE;
	}
	
	if(!dat_createDataset(argv))
	{
		printf("%s", errorMessage);
		free(errorMessage);
		free(warningMessage);
		return EXIT_FAILURE;
	}

	free(errorMessage);
	free(warningMessage);
	return EXIT_SUCCESS;
}

void trainingWithAdaBoost(int argc, char **argv)
{
	BinaryDataset *dataset = (BinaryDataset *)malloc(sizeof(BinaryDataset));
				
	if(dataset == NULL)
	{
		printf("error initializing dataset\n");
		return;
	}
	
	char *trainingPosDataFile = (char *)malloc(MAX_CHARACTERS);
	char *trainingNegDataFile = (char *)malloc(MAX_CHARACTERS);
	char *featuresFolder = (char *)malloc(MAX_CHARACTERS);
	char *classifiersFile = (char *)malloc(MAX_CHARACTERS);
	unsigned int size = 0;	// size of the squared training images
	unsigned int numOfFeatures = 0; // number of features to generate in training
	bool mode = false;
	bool useTrainedFeatures = false;
	bool saveFeatures = false;
	
	for(int i = 1; i < argc; i++)
	{//printf("%s\n", argv[i]);
		
		// check for training positive data file path
		if(strcmp(argv[i], "-tposdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(trainingPosDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for training negative data file path
		if(strcmp(argv[i], "-tnegdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(trainingNegDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for size of the training images
		if(strcmp(argv[i], "-size") == 0)
		{
			size = atoi(argv[i + 1]);
		}
		
		// check for size of the training images
		if(strcmp(argv[i], "-features") == 0)
		{
			numOfFeatures = atoi(argv[i + 1]);
		}
		
		// check for the features folder
		if(strcmp(argv[i], "-ff") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(featuresFolder, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for the classifiers file
		if(strcmp(argv[i], "-cl") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(classifiersFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for parallelization mode
		if(strcmp(argv[i], "-p") == 0)
		{
			mode = true;
			printf("GPU parallelization mode activated.\n");
		}
		
		// check for trained features option
		if(strcmp(argv[i], "-tf") == 0)
		{
			useTrainedFeatures = true;
		}
		
		// check for save features option
		if(strcmp(argv[i], "-sf") == 0)
		{
			saveFeatures = true;
			printf("Features will be saved as jpg images in %s.\n", featuresFolder);
		}
	}
	
	//loadBinaryDataset(&dataset, dataFile, size);
	//computeIntegralImages(dataset);
	//normalizeByMedian(dataset);
	//flatData(&dataset);
	//freeIntermediateData(&dataset);
	dat_loadFromFiles(trainingPosDataFile, trainingNegDataFile, size);
	buildFeatures(size, size, useTrainedFeatures, featuresFolder);
	initializeAdaBoost(numOfFeatures, dataset, mode);
	trainAdaBoost(numOfFeatures, dataset, featuresFolder, classifiersFile, saveFeatures);
	
	closeAdaBoost();
	destroyFeatures();
	dat_close(dataset);
	free(dataset);
	free(trainingPosDataFile);
	free(trainingNegDataFile);
	free(featuresFolder);
	free(classifiersFile);
}

void testWithAdaBoost(int argc, char **argv)
{
	BinaryDataset *dataset = (BinaryDataset *)malloc(sizeof(BinaryDataset));
				
	if(dataset == NULL)
	{
		printf("error initializing dataset\n");
		return ;
	}
	
	//char *dataFile = (char *)malloc(MAX_CHARACTERS);
	char *validationPosDataFile = (char *)malloc(MAX_CHARACTERS);
	char *validationNegDataFile = (char *)malloc(MAX_CHARACTERS);
	char *classifiersFile = (char *)malloc(MAX_CHARACTERS);
	unsigned int size = 0;	// size of the squared training images
	
	for(int i = 1; i < argc; i++)
	{
		// check for positive validation data
		if(strcmp(argv[i], "-vposdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(validationPosDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for negative validation data
		if(strcmp(argv[i], "-vnegdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(validationNegDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for the classifiers file
		if(strcmp(argv[i], "-cl") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(classifiersFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for size of the training images
		if(strcmp(argv[i], "-size") == 0)
		{
			size = atoi(argv[i + 1]);
		}
	}
	
	dat_loadFromFiles(validationPosDataFile, validationNegDataFile, size);
	buildFeatures(size, size, false, NULL);
	testAdaBoost(&dataset, classifiersFile, size);
	
	destroyFeatures();
	dat_close(dataset);
    free(dataset);
	free(validationPosDataFile);
	free(validationNegDataFile);
	free(classifiersFile);
}

void trainingWithCascade(int argc, char **argv)
{
	BinaryDataset *tDataset = (BinaryDataset *)malloc(sizeof(BinaryDataset));
	BinaryDataset *vDataset = (BinaryDataset *)malloc(sizeof(BinaryDataset));
	
	if(tDataset == NULL || vDataset == NULL)
	{
		printf("error initializing datasets\n");
		return;
	}
	
	char *trainingPosDataFile = (char *)malloc(MAX_CHARACTERS);
	char *trainingNegDataFile = (char *)malloc(MAX_CHARACTERS);
	char *validationPosDataFile = (char *)malloc(MAX_CHARACTERS);
	char *validationNegDataFile = (char *)malloc(MAX_CHARACTERS);
	char *featuresFolder = (char *)malloc(MAX_CHARACTERS);
	char *baseClassifiersFile = (char *)malloc(MAX_CHARACTERS);
	unsigned int size = 0;	// size of the squared training images
	float fi = 0.0;		// maximum false positive rate per layer
	float di = 0.0;		// minimum detection rate per layer
	float Ft = 0.0;		// target overall false positive rate
	bool mode = false;	// use GPU parallelization
	bool useTrainedFeatures = false;
	
	for(int i = 1; i < argc; i++)
	{
		// check for training positive data file path
		if(strcmp(argv[i], "-tposdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(trainingPosDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for training negative data file path
		if(strcmp(argv[i], "-tnegdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(trainingNegDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for validation positive data file path
		if(strcmp(argv[i], "-vposdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(validationPosDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for validation negative data file path
		if(strcmp(argv[i], "-vnegdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(validationNegDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for the size parameter of the training images
		if(strcmp(argv[i], "-size") == 0)
		{
			size = atoi(argv[i + 1]);
		}
		
		// check for the features folder
		if(strcmp(argv[i], "-ff") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(featuresFolder, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for the classifiers file
		if(strcmp(argv[i], "-cl") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(baseClassifiersFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for the maximum false positive rate per layer argument
		if(strcmp(argv[i], "-fi") == 0)
		{
			sscanf(argv[i + 1], "%f", &fi);
		}
		
		// check for the minimum detection rate per layer argument
		if(strcmp(argv[i], "-di") == 0)
		{
			sscanf(argv[i + 1], "%f", &di);
		}
		
		// check for the target overall false positive rate argument
		if(strcmp(argv[i], "-Ft") == 0)
		{
			sscanf(argv[i + 1], "%f", &Ft);
		}
		
		// check for parallelization mode
		if(strcmp(argv[i], "-p") == 0)
		{
			mode = true;
			printf("GPU parallelization mode activated.\n");
		}
		
		// check for trained features option
		if(strcmp(argv[i], "-tf") == 0)
		{
			useTrainedFeatures = true;
		}
	}
	
	dat_loadFromFiles(trainingPosDataFile, trainingNegDataFile, size);
	dat_loadFromFiles(validationPosDataFile, validationNegDataFile, size);
    buildFeatures(size, size, useTrainedFeatures, featuresFolder);
    initializeCascade(MAXLAYERS);
	trainCascadeClassifier(&tDataset, &vDataset, fi, di, Ft, featuresFolder, baseClassifiersFile, mode);
    
	closeCascade();
    destroyFeatures();
    dat_close(tDataset);
	dat_close(vDataset);
    free(tDataset);
    free(vDataset);
	free(trainingPosDataFile);
	free(trainingNegDataFile);
	free(validationPosDataFile);
	free(validationNegDataFile);
	free(featuresFolder);
	free(baseClassifiersFile);
}

void testWithCascade(int argc, char **argv)
{
	BinaryDataset *dataset = (BinaryDataset *)malloc(sizeof(BinaryDataset));
				
	if(dataset == NULL)
	{
		printf("error initializing dataset\n");
		return ;
	}
	
	char *validationPosDataFile = (char *)malloc(MAX_CHARACTERS);
	char *validationNegDataFile = (char *)malloc(MAX_CHARACTERS);
	char *baseClassifiersFile = (char *)malloc(MAX_CHARACTERS);
	unsigned int size = 0;	// size of the squared training images
	
	for(int i = 1; i < argc; i++)
	{
		// check for positive validation data
		if(strcmp(argv[i], "-vposdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(validationPosDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for negative validation data
		if(strcmp(argv[i], "-vnegdata") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(validationNegDataFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for the classifiers file
		if(strcmp(argv[i], "-cl") == 0)
		{
			if(strlen(argv[i + 1]) <= MAX_CHARACTERS)
				strcpy(baseClassifiersFile, argv[i + 1]);
			else
				printf("argument exceeds 255 characters long.\n");
		}
		
		// check for size of the training images
		if(strcmp(argv[i], "-size") == 0)
		{
			size = atoi(argv[i + 1]);
		}
	}
	
	dat_loadFromFiles(validationPosDataFile, validationNegDataFile, size);
	buildFeatures(size, size, false, NULL);//printf("**: %p, *: %p, n: %d, n0: %d, n1:%d\n", &dataset, dataset, dataset->n, dataset->n0, dataset->n1);
	testCascadeClassifier(&dataset, baseClassifiersFile, size);
	
	closeCascade();
	destroyFeatures();
	dat_close(dataset);
    free(dataset);
	free(validationPosDataFile);
	free(validationNegDataFile);
	free(baseClassifiersFile);
}