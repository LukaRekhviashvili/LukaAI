#ifndef BEST_MATCH_UNIT_H_INCLUDED
#define BEST_MATCH_UNIT_H_INCLUDED

#include "Randomiser.h"
#include <stdio.h>

// struct
struct bmu {
    int i, j;
    struct bmu * next;
};

struct node {
    double * w;
    double act;
    char * label;
};

// variables
extern struct bmu *best_match_unit;
extern struct bmu *first_node;
extern struct bmu *last_node;
extern double bestM_act;
extern int number_of_bmu_nodes;

// function declarations
void print_bmu(struct bmu *_bmu, struct node ** node_map);

struct bmu * get_bmu_node();

void add_bmu_node(int i, int j, int act);
void find_bmu_linked_list(struct node ** node_map, 
	int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH);


#endif
