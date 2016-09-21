// header files
#include "Neighbour.h"
#include "Randomiser.h"
#include "Best_match_unit.h"
#include "Input_data_vect.h"

#include <unistd.h>

// colors
#define RED  "\x1B[31m"
#define NRM  "\x1B[0m"
#define BLU  "\x1B[34m"

int TOTAL_NUMBER_OF_ITERATIONS = 500 * 150; // 500 * n_of_d_vects
int CURRENT_NUMBER_OF_ITERATIONS;
double ALPHA_COEFICIENT = 0.9; // 0,7 - 0,9
int print_coefivcient = 100;

//functions
void print_doubles_vector(double * V);
double get_alpha(int Time);
double get_neibour_coeficient(int Time, int c_i,int c_j,int b_i, int b_j);
double * get_v_difference(double * V1, double * V2);
double * multiplay_by_scalar(double * V, double scalar);
void vect_sum(double * V1, double * V2);
void go_through_phase_1_();
void go_through_phase_2_();
void end_pahse();
void iterate_through_neighbours(int Time, int b_i, int b_j);


// MAIN
int main()
{
    srand(time(NULL));

    initialise_input_data_vector();
    normilise_input_data_vector();

    intitialise_shuffled_vect(number_of_data_vectors);

    //printf("HERE:\n");
    //for(int i=0;i<number_of_data_vectors;i++){
    //    print_data_element(data[shuffled_vector[i]]);
    //}

    initialise_randomised_data_vect(data, number_of_data_vectors, data_vector_size);

    initialise_node_map(data_vector_size, data);

    // MAIN LOOP
    // calculate number of first phase iterations

    // First phase
    go_through_phase_1_();

    // Second Phase
    //go_through_phase_2_();

    // End phase
    end_pahse();

    return 0;
}

// PHASES
void go_through_phase_1_()
{
    CURRENT_NUMBER_OF_ITERATIONS = TOTAL_NUMBER_OF_ITERATIONS / 4;


    for(int time = 0; time < CURRENT_NUMBER_OF_ITERATIONS; time++){
    //for(int time = 0; time < 5000; time++){
        if (time % number_of_data_vectors == 0){
            shuffle_vect(number_of_data_vectors);
        }
        
        find_node_map_actiavtions(data, data_vector_size, number_of_data_vectors, time);

        find_bmu_linked_list(node_map, NODE_MAP_HEIGHT, NODE_MAP_LENGTH);

        best_match_unit = get_bmu_node();


        determine_neighbours(NODE_MAP_HEIGHT, NODE_MAP_LENGTH, best_match_unit, time, CURRENT_NUMBER_OF_ITERATIONS);
        //printf("neibours:%d %d\n", neigbour.start_j, neigbour.end_j);
        

        // LEARNING
        iterate_through_neighbours(time, best_match_unit->i, best_match_unit->j);

        // printing the output
        if (time % print_coefivcient == 0){ 
            printf("Progress=>\t N:%d\t |%f%%\n",
                    time,
                    100 * (double)time/(double)CURRENT_NUMBER_OF_ITERATIONS);
            //print_node_map_names(best_match_unit->i, best_match_unit->j);
            //print_bmu(best_match_unit, node_map);

            //printf("\n");
            //end_pahse();
            //sleep(1);
        }


        // waiting
        //sleep(1);
    }

}

void go_through_phase_2_()
{

    CURRENT_NUMBER_OF_ITERATIONS = 
        TOTAL_NUMBER_OF_ITERATIONS - CURRENT_NUMBER_OF_ITERATIONS;
    ALPHA_COEFICIENT = 0.03;


    for(int time = 0; time < CURRENT_NUMBER_OF_ITERATIONS; time++){
    //for(int time = 0; time < 4; time++){
        if (time % number_of_data_vectors == 0){
            shuffle_vect(number_of_data_vectors);
        }
        
        find_node_map_actiavtions(data, data_vector_size, number_of_data_vectors, time);

        find_bmu_linked_list(node_map, NODE_MAP_HEIGHT, NODE_MAP_LENGTH);

        best_match_unit = get_bmu_node();


        determine_neighbours(NODE_MAP_HEIGHT, NODE_MAP_LENGTH, best_match_unit, time, CURRENT_NUMBER_OF_ITERATIONS);
        //printf("neibours:%d %d\n", neigbour.start_j, neigbour.end_j);
        

        // LEARNING
        iterate_through_neighbours(time, best_match_unit->i, best_match_unit->j);

        // printing the output
        if (time % print_coefivcient == 0){ 
            printf("Progress=>\t N:%d\t |%f%%\n", 
                    time, 
                    100 * (double)time/(double)CURRENT_NUMBER_OF_ITERATIONS);
            //print_node_map_names(best_match_unit->i, best_match_unit->j);
            //print_bmu(best_match_unit, node_map);
        }


        // waiting
        //sleep(1);
    }
}

void end_pahse()
{
    printf("END PHASE:\n");

    for(int i = 0; i < NODE_MAP_HEIGHT; i++){
        for(int j = 0; j < NODE_MAP_LENGTH; j++){
            node_map[i][j].label[0] = '-';
        }
    }

    for(int i = 0; i < number_of_data_vectors; i++){
        find_node_map_actiavtions(data, data_vector_size, number_of_data_vectors, i);

        find_bmu_linked_list(node_map, NODE_MAP_HEIGHT, NODE_MAP_LENGTH);

        best_match_unit = get_bmu_node();

        int x = best_match_unit->i;
        int y = best_match_unit->j;

        if(data[shuffled_vector[i]].name[5] == 's')
            node_map[x][y].label[0] = 's';
        else if(data[shuffled_vector[i]].name[6] == 'e')
            node_map[x][y].label[0] = 'e';
        if(data[shuffled_vector[i]].name[6] == 'i')
            node_map[x][y].label[0] = 'i';
    }

    for(int i = 0; i < NODE_MAP_HEIGHT; i++){
        for(int j = 0; j < NODE_MAP_LENGTH; j++){
            if(node_map[i][j].label[0] == 'i'){
                printf("%s", RED);
            }
            else if(node_map[i][j].label[0] == 'e'){
                printf("%s", BLU);
            }
            else{
                printf("%s", NRM);
            }

            printf(" %c", node_map[i][j].label[0]);
        }
        printf("\n");
    }

    printf("%s", NRM);
}


void iterate_through_neighbours(int Time, int b_i, int b_j)
{
    int iterator = shuffled_vector[Time % number_of_data_vectors];
    double alpha = get_alpha(Time);
    double neighbour_coeficient;



    if (Time % print_coefivcient == 0){ 
        //print_doubles_vector(data[iterator].vect);

        //printf("alpha: %f    ", alpha);
        //printf("neighbour_coeficient: %f\t", neighbour_coeficient);
        //printf("scalar: %f\n", alpha * neighbour_coeficient);
    }

    for(int i = neigbour.start_i; i < neigbour.end_i; i++){
        for(int j = neigbour.start_j; j < neigbour.end_j; j++){
            neighbour_coeficient = get_neibour_coeficient(Time, i, j, b_i, b_j);
            double * v_difference = 
                get_v_difference(data[iterator].vect, node_map[i][j].w);

            double * v_m_by_scalar =
                multiplay_by_scalar(
                        v_difference, 
                        alpha * neighbour_coeficient);

            vect_sum(node_map[i][j].w, v_m_by_scalar);

            free(v_difference);
            free(v_m_by_scalar);
        }
    }
}

double get_alpha(int Time)
{
    double time_constant = (double)CURRENT_NUMBER_OF_ITERATIONS / (double)MAP_RAD;

    double ans = ALPHA_COEFICIENT * exp(-(double)Time/ time_constant);


    //printf("ALPHA_COEFICIENT: %f\t", ALPHA_COEFICIENT);
    //printf("TIME: %d\t", Time);
    //printf("CURRENT_NUMBER_OF_ITERATIONS: %d\n", CURRENT_NUMBER_OF_ITERATIONS);

    return ans;
}

double get_neibour_coeficient(int Time, int c_i, int c_j, int b_i, int b_j)
{
    double dist_q = (c_i - b_i) * (c_i - b_i) + (c_j - b_j) * (c_j - b_j);

    double ans = exp(-dist_q/ 2.0 * (double)neigbour.n_of_rings * (double)neigbour.n_of_rings);

    return ans;
}

double * get_v_difference(double * V1, double * V2)
{
    double * ans_vect = malloc(data_vector_size * sizeof(double));

    //printf("DIFFERENCE:\n");

    for(int i = 0; i < data_vector_size; i++){
        //printf("V1:%f\tV2:%f\t", V1[i], V2[i]);
        ans_vect[i] = V1[i] - V2[i];
        //printf("ANS:%f\n", ans_vect[i]);
    }
    //printf("DIFFERENCE:\n");

    return ans_vect;
}

void vect_sum(double * V1, double * V2)
{
    for(int i = 0; i < data_vector_size; i++){
        //printf("V1:%f\tV2:%f\t", V1[i], V2[i]);
        V1[i] += V2[i];
        //printf("ANS:%f\n", V1[i]);
    }
}

double * multiplay_by_scalar(double * V, double scalar)
{
    double * ans_vect = malloc(data_vector_size * sizeof(double));

    for(int i = 0; i < data_vector_size; i++){
        //printf("V1:%f\tSCALAR:%f\t", V[i], scalar);
        ans_vect[i] = V[i] * scalar;
        //printf("ANS:%f\n", ans_vect[i]);
    }

    return ans_vect;
}

void print_doubles_vector(double * V)
{
    printf("the random vector: ");
    for(int i=0; i < 4; i++){
        printf("%f ", V[i]);
    }
    printf("\n");
}
