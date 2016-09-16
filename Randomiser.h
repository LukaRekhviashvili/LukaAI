#ifndef RANDOMISER_H_INCLUDED
#define RANDOMISER_H_INCLUDED

#include <time.h>
#include <stdlib.h>

extern double RANDOMISER_MAX, RANDOMISER_MIN;

double get_random_double(double min, double max);
int get_random_int(int minI, int maxI);
double * get_random_vector(int data_vector_size, double * middle_vector);

#endif
