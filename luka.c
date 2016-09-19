// header files
#include "Neighbour.h"
#include "Randomiser.h"
#include "Best_match_unit.h"
#include "Input_data_vect.h"

int TOTAL_NUMBER_OF_ITERATIONS = 500 * 150; // 500 * n_of_d_vects
int CURRENT_NUMBER_OF_ITERATIONS;
double ALPHA_COEFICIENT = 0.8; // 0,7 - 0,9

//functions
void print_doubles_vector(double * V);
void iterate_through_neighbours();
double get_alpha(int Time);
double get_neibour_coeficient(int Time);
double * get_v_difference(double * V1, double * V2);
double * multiplay_by_scalar(double * V, double scalar);
void vect_sum(double * V1, double * V2);

// MAIN
int main()
{
    srand(time(NULL));

    initialise_input_data_vector();
    normilise_input_data_vector();

    intitialise_shuffled_vect(number_of_data_vectors);

    initialise_randomised_data_vect(data, number_of_data_vectors, data_vector_size);

    initialise_node_map(data_vector_size, data);

    // MAIN LOOP
    // calculate number of first phase iterations
    CURRENT_NUMBER_OF_ITERATIONS = TOTAL_NUMBER_OF_ITERATIONS / 4;

    // First phase
    for(int time = 0; time < CURRENT_NUMBER_OF_ITERATIONS; time++){
        
        find_node_map_actiavtions(data, data_vector_size, number_of_data_vectors, time);

        find_bmu_linked_list(node_map, NODE_MAP_HEIGHT, NODE_MAP_LENGTH);

        best_match_unit = get_bmu_node();


        determine_neighbours(NODE_MAP_HEIGHT, NODE_MAP_LENGTH, best_match_unit);

        // LEARNING
        iterate_through_neighbours(time);

        // printing the output
        printf("N:%d\t |%f%%\n", 
                time, 
                100 * (double)time/(double)CURRENT_NUMBER_OF_ITERATIONS);
        print_node_map_names();
        print_bmu(best_match_unit, node_map);
    }

    // End of First Phase

    return 0;
}


void iterate_through_neighbours(int Time)
{
    int iterator = shuffled_vector[Time % number_of_data_vectors];
    //print_doubles_vector(data[iterator].vect);
    double alpha = get_alpha(Time);
    double neighbour_coeficient = get_neibour_coeficient(Time);

    for(int i = neigbour.start_i; i < neigbour.end_i; i++){
        for(int j = neigbour.start_j; j < neigbour.end_j; j++){
            double * v_difference = 
                get_v_difference(data[iterator].vect, node_map[i][j].w);

            double * v_m_by_scalar =
                multiplay_by_scalar(
                        v_difference, 
                        alpha * neighbour_coeficient);

            vect_sum(node_map[i][j].w, v_m_by_scalar);
        }
    }
}

double get_alpha(int Time)
{
    return ALPHA_COEFICIENT * 
        (1 - ((double)Time / (double)CURRENT_NUMBER_OF_ITERATIONS));
}

double get_neibour_coeficient(int Time)
{
    return neigbour.n_of_rings *
        (1 - ((double)Time / (double)CURRENT_NUMBER_OF_ITERATIONS));
}

double * get_v_difference(double * V1, double * V2)
{
    double * ans_vect = malloc(data_vector_size * sizeof(double));

    for(int i = 0; i < data_vector_size; i++){
        ans_vect[i] = V1[i] - V2[i];
    }

    return ans_vect;
}

void vect_sum(double * V1, double * V2)
{
    for(int i = 0; i < data_vector_size; i++){
        V1[i] += V2[i];
    }
}

double * multiplay_by_scalar(double * V, double scalar)
{
    double * ans_vect = malloc(data_vector_size * sizeof(double));

    for(int i = 0; i < data_vector_size; i++){
        ans_vect[i] = V[i] * scalar;
    }

    return ans_vect;
}

void print_doubles_vector(double * V)
{
    for(int i=0; i < 4; i++){
        printf("%f ", V[i]);
    }
    printf("\n");
}
