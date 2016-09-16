#include "Neighbour.h"

struct neighbour_stats neigbour;

void initialise_neighbour(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, struct bmu *_bmu)
{
    find_number_of_neighbours(NODE_MAP_HEIGHT, NODE_MAP_LENGTH);
    determine_neighbour_indexes(NODE_MAP_HEIGHT, NODE_MAP_LENGTH, _bmu);
}

void find_number_of_neighbours(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH)
{
    int minimum_n_of_neighbours = NODE_MAP_HEIGHT * NODE_MAP_LENGTH * 3 / 10;

    neigbour.needed_n_of_neighbours = 8;
    neigbour.n_of_rings = 1;

    for (int i = 0; neigbour.needed_n_of_neighbours < minimum_n_of_neighbours && i < minimum_n_of_neighbours; ++i) {
        neigbour.needed_n_of_neighbours += (i + 2) * 8;
        neigbour.n_of_rings++;
    }

    //printf("%d    %d\n", needed_n_of_neighbours, minimum_n_of_neighbours);
}

void determine_neighbour_indexes(int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH, struct bmu *_bmu)
{
    int start_i = _bmu->i - neigbour.n_of_rings;
    int start_j = _bmu->j - neigbour.n_of_rings;

    int end_i = _bmu->i + neigbour.n_of_rings;
    int end_j = _bmu->j + neigbour.n_of_rings;

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
