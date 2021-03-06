#include "Neighbour.h"

#include <math.h>

struct neighbour_stats neigbour;

// private function declarations
void find_number_of_neighbours(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, int Time, int TOTAL);
void determine_neighbour_indexes(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, struct bmu *_bmu);


void determine_neighbours(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, struct bmu *_bmu, int Time, int TOTAL)
{
    find_number_of_neighbours(NODE_MAP_HEIGHT, NODE_MAP_LENGTH, Time, TOTAL);
    determine_neighbour_indexes(NODE_MAP_HEIGHT, NODE_MAP_LENGTH, _bmu);
}

void find_number_of_neighbours(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, int Time, int TOTAL)
{
    double time_constant = (double)TOTAL / (double)MAP_RAD;

    neigbour.n_of_rings = (double)MAP_RAD * exp(-(double)Time/time_constant);

    if(neigbour.n_of_rings < 1 && Time > TOTAL / 4)
        neigbour.n_of_rings = 1;
}

void determine_neighbour_indexes(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, struct bmu *_bmu)
{
    int start_i = _bmu->i - neigbour.n_of_rings;
    int start_j = _bmu->j - neigbour.n_of_rings;

    int end_i = _bmu->i + neigbour.n_of_rings + 1;
    int end_j = _bmu->j + neigbour.n_of_rings + 1;

    if (start_i < 0) start_i = 0;
    if (start_j < 0) start_j = 0;

    if (end_j > NODE_MAP_LENGTH) end_j = NODE_MAP_LENGTH;
    if (end_i > NODE_MAP_HEIGHT) end_i = NODE_MAP_HEIGHT;

    // assign indexes
    neigbour.start_i = start_i;
    neigbour.start_j = start_j;

    neigbour.end_i = end_i;
    neigbour.end_j = end_j;

    //printf("%d %d      %d %d\n", neigbour.start_i, neigbour.start_j, neigbour.end_i, neigbour.end_j);
}
