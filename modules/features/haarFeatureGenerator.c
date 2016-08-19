/**
* haarFeatureGenerator.c
* Guardian Kids
* NestNet Group
*/

//#include <cuda.h>
//#include <cuda_runtime.h>
#include "../../include/haarFeatureGenerator.h"

#define TOTAL_EXPECTED_FEATURES 162336

unsigned int featuresCounter;
unsigned int trainedFeaturesCounter = 0;
HaarFeature *generatedFeatures;
HaarFeature *trainedFeatures;
//int indexF = 0;
//char *theFeaturesFolder;
bool useTrainedFeatures = false;

void buildCategoryOfFeatures(int numVerticalRects, int numHorizontalRects, int frameWidth, int frameHeight)
{
	float maxWidth = frameWidth / numHorizontalRects;
	float maxHeight = frameHeight / numVerticalRects;
	int totalRectangles = numVerticalRects * numHorizontalRects;
	int indexR = 0;
	bool type = false;
	
	for(int h = 1; h <= maxHeight; h++)
	{
		for(int w = 1; w <= maxWidth; w++)
		{
			for(int y = 0; y <= frameHeight - h * numVerticalRects; y++)
			{
				for(int x = 0; x <= frameWidth - w * numHorizontalRects; x++)
				{
					HaarRectangle *rectangles = (HaarRectangle *)malloc(sizeof(HaarRectangle) * totalRectangles);
					
					if(rectangles == NULL)
					{
						printf("Error initializing rectangles pointer\n");
						return;
					}
					
					for(int n = 0; n < numVerticalRects; n++)
					{
						for(int m = 0; m < numHorizontalRects; m++)
						{
							rectangles[indexR].x = x + m * w;
							rectangles[indexR].y = y + n * h;
							rectangles[indexR].width = w;
							rectangles[indexR].height = h;
							rectangles[indexR].isBlack = type;
							type = !type;
							indexR++;
						}
					}
					
					indexR = 0;
					
					generatedFeatures[featuresCounter].rectangles = rectangles;
					generatedFeatures[featuresCounter].numberOfRects = totalRectangles;
					featuresCounter++;
					type = false;
				}
			}
		}
	} 
}

void loadTrainedFeatures(char *featuresFolder){
	
	int file_count = 0;
	DIR *dirp;
	struct dirent *entry;

	dirp = opendir(featuresFolder);
	
	if(dirp == NULL){
		printf("error trying to open folder %s\n", featuresFolder);
		return;
	}
	
	while ((entry = readdir(dirp)) != NULL) {
		if (entry->d_type == DT_REG) {
			 file_count++;
		}
	}
	closedir(dirp);
	
	trainedFeatures = (HaarFeature *)malloc(sizeof(HaarFeature) * file_count);
	
	if(trainedFeatures == NULL)
	{
		printf("error initializing trained features pointer\n");
		return;
	}
	
	dirp = opendir(featuresFolder); 
	
	while ((entry = readdir(dirp)) != NULL) {
		if (entry->d_type == DT_REG) { 
			 
			 char subbuff[32];
			 int length = 0;
			 bool lowDash = false;
			 
			 for(int i = 0; i < 32; i++){
				 if(entry->d_name[i] == '.')
					 break;
				
				if(lowDash){
					subbuff[length] = entry->d_name[i];
					length++;
				}
				
				if(entry->d_name[i] == '_')
					lowDash = true;
			 }
			 
			 subbuff[length] = '\0';
			 int index = atoi(subbuff);
			 //printf("feature: %d\n", index);
			 trainedFeatures[trainedFeaturesCounter] = generatedFeatures[index];
			 trainedFeaturesCounter++;
			 
		}
	}
	closedir(dirp);
	//exit(0);
}

void buildFeatures(unsigned int frameWidth, unsigned int frameHeight, bool useOnlyTrainedFeatures, char *featuresFolder)
{
	if(generatedFeatures == NULL)
	{
		//int error = cudaMallocManaged((void **)&generatedFeatures, (sizeof(HaarFeature) * TOTAL_EXPECTED_FEATURES), 0);
		generatedFeatures = (HaarFeature *)malloc(sizeof(HaarFeature) * TOTAL_EXPECTED_FEATURES);
		
		if(generatedFeatures == NULL)
		{
			//printf("%s\n", cudaGetErrorString(error));
			printf("error initializing features pointer\n");
			return;
		}
		
		printf("Generating haar like features...\n");
		buildCategoryOfFeatures(1, 2, frameWidth, frameHeight);
		buildCategoryOfFeatures(1, 3, frameWidth, frameHeight);
		buildCategoryOfFeatures(2, 1, frameWidth, frameHeight);
		buildCategoryOfFeatures(3, 1, frameWidth, frameHeight);
		buildCategoryOfFeatures(2, 2, frameWidth, frameHeight);
		//featuresCounter = indexF;
		//indexF = 0;
		printf("%d features generated\n", featuresCounter);
		
		if(featuresCounter != TOTAL_EXPECTED_FEATURES)
			printf("ERROR the number of features %d does not match with the expected %d", featuresCounter, TOTAL_EXPECTED_FEATURES);
			
		if(useOnlyTrainedFeatures && featuresFolder != NULL){
			useTrainedFeatures = useOnlyTrainedFeatures;
			loadTrainedFeatures(featuresFolder);
			printf("**Working with %d TRAINED features\n", trainedFeaturesCounter);
		}
	}
}

#ifdef __cplusplus
extern "C"
#endif
void getFeatures(HaarFeature **outFeatures)
{
	if(useTrainedFeatures){
		(*outFeatures) = trainedFeatures;
	}
	else{
		(*outFeatures) = generatedFeatures;
	}
}

#ifdef __cplusplus
extern "C"
#endif
unsigned int getFeaturesLength(){
	
	if(useTrainedFeatures){
		return trainedFeaturesCounter;
	}
	else{
		return featuresCounter;
	}
}

void destroyFeatures()
{
	for(unsigned int i = 0; i < featuresCounter; i++){
		free(generatedFeatures[i].rectangles);
	}
	
	free(generatedFeatures);
	
	if(useTrainedFeatures){
		for(unsigned int i = 0; i < trainedFeaturesCounter; i++){
			free(trainedFeatures[i].rectangles);
		}
		free(trainedFeatures);
	}
}