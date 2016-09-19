#include "Node_map.h"

int NODE_MAP_LENGTH = 10, NODE_MAP_HEIGHT = 8;
struct node ** node_map;

void initialise_node_map(int data_vector_size, struct data_vect *data)
{
    //int number_of_nodes = 5 * sqrt(number_of_data_vectors);
    //double division = 3.0/5.0;

    // allocate memory for node map
    node_map = malloc(NODE_MAP_HEIGHT * sizeof(struct node));
    for (int i = 0; i < NODE_MAP_HEIGHT; ++i) {
        node_map[i] = malloc(NODE_MAP_LENGTH * sizeof(struct node));
    }

    // initialise
    for (int i = 0; i < NODE_MAP_HEIGHT; ++i) {
        for (int j = 0; j < NODE_MAP_LENGTH; ++j) {
            node_map[i][j].w = malloc(data_vector_size * sizeof(double));
            int iterator = i * NODE_MAP_LENGTH + j;

            // Initialise vector
            memcpy(node_map[i][j].w,
                   randomised_data[iterator].vect,
                   data_vector_size * sizeof(double));


            //for (int k = 0; k < data_vector_size; ++k) {
            //    printf("%f\t", node_map[i][j].w[k]);
            //    printf("%f\n", randomised_data[i * NODE_MAP_LENGTH + j].vect[k]);
            //}
            //printf("\n");


            // Initialise label with name
            node_map[i][j].label =
                    malloc(1 + strlen(randomised_data[iterator].name));
            strcpy(node_map[i][j].label, randomised_data[iterator].name);


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
                    node_map[i][j].w,
                    data[shuffled_vector[iterator]].vect,
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

void print_node_map_names()
{
    for(int i = 0; i < NODE_MAP_HEIGHT; i++){
        for(int j = 0; j < NODE_MAP_LENGTH; j++){
            for(int k = 0; k < data_vector_size; k++){
                printf("%.2f ", node_map[i][j].w[k]);
            }
            printf("|");
        }
        printf("\n");
    }
}

