/**
* timeEval.c
* Guardian Kids
* NestNet Group
* functions for time evaluation 
*/

#include "../../include/timeEval.h"

void timeEvalSubtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
}

void timeEvalPrint(struct timeval *tv)
{
    //char buffer[30];
    //time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    /*curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);*/
}
