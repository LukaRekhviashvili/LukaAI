#ifndef INPUT_DATA_VECT_H_INCLUDED
#define INPUT_DATA_VECT_H_INCLUDED

#include "File_reader.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>


// structures
struct data_vect {
    double * vect;
    double norm;
    char * name;
};


// variables
extern int number_of_data_vectors, data_vector_size;
extern struct data_vect * data;


// functions
void initialise_input_data_vector();
void normilise_input_data_vector();

void print_data_element(struct data_vect data);
void print_data(struct data_vect *data);


#endif
