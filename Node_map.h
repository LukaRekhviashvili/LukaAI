#ifndef NODE_MAP_H_INCLUDED
#define NODE_MAP_H_INCLUDED

#include <stdio.h>
#include "Randomiser.h"
#include "Utils.h"

struct node {
    double * w;
    double act;
    char * label;
};


extern struct node ** node_map;
extern int NODE_MAP_LENGTH, NODE_MAP_HEIGHT;


void initialise_node_map(int data_vector_size, struct data_vect *data);
void find_node_map_actiavtions(struct data_vect *data, int data_vector_size);

double euclid_dist(double *V1, double *V2, int v_size);

void print_node_map_names();

#endif
