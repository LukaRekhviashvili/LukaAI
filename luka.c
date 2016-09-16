#include <stdlib.h>
#include <math.h>
#include <string.h>

// header files
#include "File_reader.h"
#include "Neighbour.h"
#include "Randomiser.h"
#include "Best_match_unit.h"

// Structures
struct data_vect {
    double * vect;
    double norm;
    char * name;
};

//struct node {
//    double * w;
//    double act;
//    char * label;
//};

// program variables
int number_of_data_vectors, data_vector_size;
struct data_vect * data;
struct data_vect * randomised_data;
double * middle_vector;
int * vShuffler;
struct node ** node_map;
int NODE_MAP_LENGTH = 11, NODE_MAP_HEIGHT = 6;

// function declaration
void initialise_nums();
void initialise_data();
void initialise_randomised_data();
void initialise_middle_vector();
void initialise_node_map();
void print_data_element(struct data_vect data);
double euclid_dist(double *V1, double *V2, int v_size);
void shuffle_data();
void print_node_map_names();
void print_data(struct data_vect *data);
void find_normals();
void normalise_data();


// MAIN
int main()
{
    srand(time(NULL));

    open_file_stream();

    initialise_nums();

    initialise_data();

    find_normals();
    normalise_data();

    //print_data(data);

    initialise_middle_vector();
    initialise_randomised_data();

    //print_data(randomised_data);

    shuffle_data();

    initialise_node_map();
    find_bmu_linked_list(node_map, NODE_MAP_HEIGHT, NODE_MAP_LENGTH);

    best_match_unit = get_bmu_node();

    print_bmu(best_match_unit, node_map);

    initialise_neighbour(
            NODE_MAP_HEIGHT, NODE_MAP_LENGTH, best_match_unit);

    return 0;
}

// FUNCTIONS
// initialisation
void initialise_nums()
{
    char *token;
    char buff[255];

    fgets(buff, 255, (FILE*)fp);

    token = strtok(buff, ",");
    number_of_data_vectors = atoi(token);

    token = strtok(NULL, ",");
    data_vector_size = atoi(token);

    //printf("%d   %d \n\n",number_of_data_vectors, numbersN);
}

void initialise_data()
{
    char *token;
    char buff[255];

    data = (struct data_vect *)malloc(number_of_data_vectors * sizeof(struct data_vect));

    for(int i = 0; i < number_of_data_vectors; i++){
        data[i].vect = (double *)malloc(data_vector_size * sizeof(double));

        fgets(buff, 255, (FILE*)fp);

        // initialise vector
        token = strtok(buff, ",");
        data[i].vect[0] = atof(token);

        for (int j = 1; j <= 3; ++j) {
            token = strtok(NULL, ",");
            data[i].vect[j] = atof(token);
        }

        // initialise name
        token = strtok(NULL, ",");
        data[i].name = malloc(1 + strlen(token));
        strcpy(data[i].name, token);
    }
}

void initialise_middle_vector()
{
    middle_vector = (double *)malloc(data_vector_size * sizeof(double));

    for (int i = 0; i < data_vector_size; ++i) {
        for (int j = 0; j < number_of_data_vectors; ++j) {
            middle_vector[i] += data[j].vect[i];
        }

        middle_vector[i] /= number_of_data_vectors;
    }
}

void initialise_randomised_data()
{
    randomised_data = (struct data_vect *)malloc(number_of_data_vectors * sizeof(struct data_vect));

    for (int i = 0; i < number_of_data_vectors; ++i) {
        randomised_data[i].vect = get_random_vector(data_vector_size, middle_vector);
        randomised_data[i].name = malloc(1 + strlen(data[i].name));
        strcpy(randomised_data[i].name, data[i].name);

        for (int j = 0; j < data_vector_size; ++j) {
            randomised_data[i].norm += randomised_data[i].vect[j] * randomised_data[i].vect[j];
        }
        randomised_data[i].norm = sqrt(randomised_data[i].norm);
    }
}

void initialise_node_map()
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

            // initialise vector
            memcpy(node_map[i][j].w,
                   randomised_data[iterator].vect,
                   data_vector_size * sizeof(double));

            //for (int k = 0; k < data_vector_size; ++k) {
            //    printf("%f\t", node_map[i][j].w[k]);
            //    printf("%f\n", randomised_data[i * NODE_MAP_LENGTH + j].vect[k]);
            //}
            //printf("\n");

            // initialise label with name
            node_map[i][j].label =
                    malloc(1 + strlen(randomised_data[iterator].name));
            strcpy(node_map[i][j].label, randomised_data[iterator].name);

            //printf("%s   %s\n",node_map[i][j].label, randomised_data[iterator].name);

            // find actiavation
            node_map[i][j].act =
                    euclid_dist(node_map[i][j].w,
                                data[vShuffler[iterator]].vect,
                                data_vector_size);

            //printf("%f\n", node_map[i][j].act);

            add_bmu_node(i, j, node_map[i][j].act);
        }
    }
}

// others
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

void shuffle_data()
{
    vShuffler = malloc(number_of_data_vectors * sizeof(int));
    for (int i = 0; i < number_of_data_vectors; ++i) {
        vShuffler[i] = i;
    }

    int temp, rand_n;

    for (int i = 0; i < number_of_data_vectors; ++i) {
        rand_n = get_random_int(0, number_of_data_vectors);

        temp = vShuffler[i];
        vShuffler[i] = vShuffler[rand_n];
        vShuffler[rand_n] = temp;
    }
}

void find_normals()
{
    for(int i = 0; i < number_of_data_vectors; i++){
        for (int j = 0; j < data_vector_size; ++j) {
            data[i].norm += data[i].vect[j] * data[i].vect[j];
        }
        data[i].norm = sqrt(data[i].norm);
    }
}

void normalise_data()
{
    for(int i=0; i < number_of_data_vectors; i++){
        for(int j=0; j < data_vector_size; j++){
            data[i].vect[j] /= data[i].norm;
        }
    }
}

// printing
void print_data_element(struct data_vect data)
{
    printf(" %s", data.name);

    for(int i = 0; i < data_vector_size; i++) {
        printf("%f ",data.vect[i]);
    }

    printf(" normal: %f\n", data.norm);
}

void print_data(struct data_vect *data)
{
    for (int i = 0; i < number_of_data_vectors; ++i) {
        printf("%d: ", i);
        print_data_element(data[i]);
        printf("\n");
    }
}

void print_node_map_names()
{
    for(int i = 0; i < NODE_MAP_HEIGHT; i++){
        for(int j = 0; j < NODE_MAP_LENGTH; j++){
            printf("%c", node_map[i][j].label[5]);
        }
        printf("\n");
    }
}
