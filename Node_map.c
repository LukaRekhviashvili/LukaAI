#include "Node_map.h"
#include "Neighbour.h"

// colors
#define RED  "\x1B[31m"
#define NRM  "\x1B[0m"
#define BLU  "\x1B[34m"

int NODE_MAP_LENGTH = 10, NODE_MAP_HEIGHT = 6, MAP_RAD = 5;
struct node ** node_map;

void initialise_node_map(int data_vector_size, struct data_vect *data)
{
    // init Map rad
    if(NODE_MAP_HEIGHT > NODE_MAP_LENGTH)
        MAP_RAD = NODE_MAP_HEIGHT / 2;
    else
        MAP_RAD = NODE_MAP_LENGTH / 2;

    // allocate memory for node map
    node_map = malloc(NODE_MAP_HEIGHT * sizeof(struct node*));
    for (int i = 0; i < NODE_MAP_HEIGHT; ++i) {
        node_map[i] = malloc(NODE_MAP_LENGTH * sizeof(struct node));
    }

    // initialise
    for (int i = 0; i < NODE_MAP_HEIGHT; ++i) {
        for (int j = 0; j < NODE_MAP_LENGTH; ++j) {
            node_map[i][j].w = malloc(data_vector_size * sizeof(double));
            int iterator = i * NODE_MAP_LENGTH + j;

            // Initialise vector
            for(int k = 0; k < data_vector_size; k++){
                node_map[i][j].w[k] = randomised_data[iterator].vect[k];
            }

            //for (int k = 0; k < data_vector_size; ++k) {
            //    printf("%f\t", node_map[i][j].w[k]);
            //    printf("%f\n", randomised_data[i * NODE_MAP_LENGTH + j].vect[k]);
            //}
            //printf("\n");


            // Initialise label with name
            node_map[i][j].label = malloc(16 * sizeof(char));

            //printf("%s   %s\n",node_map[i][j].label, randomised_data[iterator].name);
            //printf("%f\n", node_map[i][j].act);
        }
    }
}

void find_node_map_actiavtions(struct data_vect *data, int data_vector_size,int number_of_data_vectors, int Time)
{
    int iterator = Time % number_of_data_vectors;

    for (int i = 0; i < NODE_MAP_HEIGHT; ++i) {
        for (int j = 0; j < NODE_MAP_LENGTH; ++j) {
            node_map[i][j].act =
                euclid_dist(
                    data[shuffled_vector[iterator]].vect,
                    node_map[i][j].w,
                    data_vector_size);
        }
    }
}

double euclid_dist(double *V1, double *V2, int v_size)
{
    double sum = 0;
    double temp;

    for(int i = 0; i < v_size; i++){
        temp = V1[i] - V2[i];
        sum += temp * temp;
    }

    return sqrt(sum);
}

void print_node_map_names(int b_i, int b_j)
{ 
    
    for(int i = 0; i < NODE_MAP_HEIGHT; i++){
        for(int j = 0; j < NODE_MAP_LENGTH; j++){
            // colors
            if(b_i == i && b_j == j){
                printf("%s", BLU);
            }
            else if(i >= neigbour.start_i && i < neigbour.end_i &&
                    j >= neigbour.start_j && j < neigbour.end_j){

                printf("%s", RED);
            }
            else{
                printf("%s", NRM);
            }
            // end colors

            for(int k = 0; k < data_vector_size - 1; k++){
                printf("%.4f ", node_map[i][j].w[k]);
            }

            printf("%.4f", node_map[i][j].w[data_vector_size - 1]);
            printf("|");
        }
        printf("\n");
    }

    printf("%s", NRM);
}

