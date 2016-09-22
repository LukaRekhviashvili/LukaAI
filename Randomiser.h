#ifndef RANDOMISER_H_INCLUDED
#define RANDOMISER_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Input_data_vect.h"

extern double RANDOMISER_MAX, RANDOMISER_MIN;
extern struct data_vect * randomised_data;
extern int * shuffled_vector;

double get_random_double(double min, double max);
int get_random_int(int minI, int maxI);
double * get_random_vector(int data_vector_size, double * middle_vector);
void initialise_randomised_data_vect(struct data_vect *data, int number_of_data_vectors, int data_vector_size);
void intitialise_shuffled_vect(int number_of_data_vectors);

#endif
