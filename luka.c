// header files
#include "Neighbour.h"
#include "Randomiser.h"
#include "Best_match_unit.h"
#include "Input_data_vect.h"

int NUMBER_OF_CYCLES = 0;

// MAIN
int main()
{
    srand(time(NULL));

    initialise_input_data_vector();
    normilise_input_data_vector();

    intitialise_shuffled_vect(number_of_data_vectors);

    initialise_randomised_data_vect(data, number_of_data_vectors, data_vector_size);

    initialise_node_map(data_vector_size, data);

    // FUTURE CYCLE || NOT IMPLEMENTED YET
    find_node_map_actiavtions(data, data_vector_size);

    find_bmu_linked_list(node_map, NODE_MAP_HEIGHT, NODE_MAP_LENGTH);

    best_match_unit = get_bmu_node();
    //print_bmu(best_match_unit, node_map);

    determine_neighbours(NODE_MAP_HEIGHT, NODE_MAP_LENGTH, best_match_unit);

    // END OF FUTURE CYCLE

    return 0;
}
