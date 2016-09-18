#ifndef NEIGHBOUR_H_INCLUDED
#define NEIGHBOUR_H_INCLUDED

#include "Best_match_unit.h"

struct neighbour_stats{
    int start_i, start_j;
    int end_i, end_j;
    int needed_n_of_neighbours, n_of_rings;
};

//struct bmu {
//    int i, j;
//    struct bmu * next;
//};

// neighbour funs
void determine_neighbours(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, struct bmu *_bmu);
void find_number_of_neighbours(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH);
void determine_neighbour_indexes(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, struct bmu *_bmu);

// variables
extern struct neighbour_stats neigbour;

#endif
