/**
* timeEval.h
* Guardian Kids
* NestNet Group
* functions for time evaluation 
*/

#ifndef TIMEEVAL_H
#define TIMEEVAL_H

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// compute the difference between two times
void timeEvalSubtract(struct timeval *result, struct timeval *t2, struct timeval *t1);

// print properly a given time
void timeEvalPrint(struct timeval *tv);

#endif
