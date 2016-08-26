/**
* NestNet Group
* Xion Object Detection Framework
* xionMessages.h
* contains all messages that the framework throws
*/

#ifndef XION_MESSAGES_H
#define XION_MESSAGES_H

//errors
//dataset related
#define NOT_FOUND_DIRECTORY "Directory does not exists, dataset cannot be created."
#define ARGUMENT_EXCEEDS "Argument exceeds the maximum number of characters."
#define FIRST_ARG_NAME_OF_FOLDER "first argument must be the name of the folder."
#define SECOND_ARG_NAME_OF_FILE "first argument must be the name of the folder."
#define THIRD_ARG_DIMENSION "third argument must be the dimension of the pictures."
#define THIRD_ARG_POSITIVE_NUMBER "third argument must be positive number"

//preprocessing related
#define NULL_IMAGE "the image to compute the integral is null"
#define NULL_IMAGE_NAME "the image name is null"
#define NULL_INTEGRAL_POINTER "the integral pointer is null"
#define NULL_SQUARED_POINTER "the squared integral pointer is null"
#define NULL_DIMENSION "the number of pixels and width of the input image cannot be zero"
#define DIMENSION_LESS_THAN_WIDTH "the dimension of the image data cannot be less than the width of image"
#define MAX_RESOLUTION "the resolution of the gray scale image cannot be greater than 1920 X 1080"
#define NULL_NORMALIZATION "the normalization vector is null"
#define INVALID_DIMENSION "the dimension is invalid, only positive numbers accepted"

//warnings
#define ZERO_VARIANCE "image %s is zero variance\n"

#define MAX_ERROR_LENGTH 1024

char *errorMessage;
char *warningMessage;

#endif