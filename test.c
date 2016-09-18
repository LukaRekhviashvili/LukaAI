#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Structures
struct data_vect {
    double * vect;
    double norm;
    char * name;
};

struct node {
    double * w;
    double act;
    char * label;
};

struct bmu {
    int i, j;
    struct bmu * next;
};

struct neighbour_stats{
    int start_i, start_j;
    int end_i, end_j;
    int needed_n_of_neighbours, n_of_rings;
};

// program variables
FILE * fp;
int number_of_data_vectors, data_vector_size;
struct data_vect * data;
struct data_vect * randomised_data;
double * middle_vector;
double RANDOMISER_MAX = 0.02, RANDOMISER_MIN = -0.05;
int * vShuffler;
struct node ** node_map;
int NODE_MAP_LENGTH = 11, NODE_MAP_HEIGHT = 6;
struct bmu *first_node;
struct bmu *last_node;
double bestM_act;
int number_of_bmu_nodes = 0;
struct neighbour_stats neigbour;
struct bmu *best_match_unit;


// function declaration
double * get_random_vector();
void initialise_nums();
void initialise_data();
void initialise_randomised_data();
void initialise_middle_vector();
void initialise_node_map();
double get_random_double(double min, double max);
int get_random_int(int minI, int maxI);
void print_data_element(struct data_vect data);
void open_file_stream();
double euclid_dist(double *V1, double *V2, int v_size);
void shuffle_data();
void add_bmu_node(int i, int j, int act);
struct bmu * get_bmu_node();
void print_bmu(struct bmu *_bmu);
void print_node_map_names();
void print_data(struct data_vect *data);
void normalise_data();
void find_bmu_linked_list();

// neighbour funs
void initialise_neighbour();
void find_number_of_neighbours();
void determine_neighbour_indexes(struct bmu *_bmu);


// MAIN
int main()
{
    srand(time(NULL));

    open_file_stream();

    initialise_nums();

    initialise_data();
    normalise_data();

    //print_data(data);

    initialise_middle_vector();
    initialise_randomised_data();

    //print_data(randomised_data);

    shuffle_data();

    initialise_node_map();
    find_bmu_linked_list();

    best_match_unit = get_bmu_node();

    print_bmu(best_match_unit);

    initialise_neighbour();

    return 0;
}

// FUNCTIONS
// streams
void open_file_stream()
{
    fp = fopen("./iris.data","r");
}

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
            middle_vector[i] = data[j].vect[i] * data[j].vect[i];
        }

        middle_vector[i] /= number_of_data_vectors;
    }
}

void initialise_randomised_data()
{
    randomised_data = (struct data_vect *)malloc(number_of_data_vectors * sizeof(struct data_vect));

    for (int i = 0; i < number_of_data_vectors; ++i) {
        randomised_data[i].vect = get_random_vector();
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

void initialise_neighbour()
{
    find_number_of_neighbours();
    determine_neighbour_indexes(best_match_unit);
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

void normalise_data()
{
    for(int i = 0; i < number_of_data_vectors; i++){
        for (int j = 0; j < data_vector_size; ++j) {
            data[i].norm += data[i].vect[j] * data[i].vect[j];
        }
        data[i].norm = sqrt(data[i].norm);
    }
}


// BMUs
void find_bmu_linked_list()
{
    for (int i = 0; i < NODE_MAP_HEIGHT; ++i) {
        for (int j = 0; j < NODE_MAP_LENGTH; ++j) {
            add_bmu_node(i, j, node_map[i][j].act);
        }
    }
};

void add_bmu_node(int i, int j, int act)
{
    if(first_node == NULL){
        bestM_act = act;

        first_node = malloc(sizeof(struct bmu));

        first_node->i = i;
        first_node->j = j;

        last_node = first_node;

        number_of_bmu_nodes = 1;
    }
    else if(act < bestM_act){
        bestM_act = act;

        first_node->i = i;
        first_node->j = j;

        last_node = first_node;
        first_node->next = NULL;

        number_of_bmu_nodes = 1;
    }
    else if(act == bestM_act){
        last_node->next = malloc(sizeof(struct bmu));

        last_node = last_node->next;

        last_node->i = i;
        last_node->j = j;

        number_of_bmu_nodes++;
    }
}

struct bmu * get_bmu_node()
{
    if(number_of_bmu_nodes == 0) {
        printf("nigga u're fucked!!\n");
        return NULL;
    }

    if(number_of_bmu_nodes == 1) {
        return first_node;
    }

    // get random bmu
    int randN = get_random_int(0, number_of_bmu_nodes);
    struct bmu * iterator = first_node;

    for (int i = 0; i < randN; ++i) {
        iterator = iterator->next;
    }

    //printf("%d %d\n", iterator->i, iterator->j);
    //printf("%d %d\n", first_node[randN].i, first_node[randN].j);

    return iterator;
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

void print_bmu(struct bmu *_bmu)
{
    printf("%d %d  act: %f <-%s",
           _bmu->i, _bmu->j,
           node_map[_bmu->i][_bmu->j].act,
           node_map[_bmu->i][_bmu->j].label);
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

// randomizing
double get_random_double(double minD, double maxD)
{
    double rand_from_0_to_1 = ((double)rand()/(double)RAND_MAX);
    double ans = minD + (rand_from_0_to_1 * (maxD - minD));

    return ans;
}

int get_random_int(int minI, int maxI)
{
    return minI + (rand() % (maxI - minI));
}

double * get_random_vector()
{
    double * ans = (double *)malloc(data_vector_size * sizeof(double));

    for (int i = 0; i < data_vector_size; ++i) {
        ans[i] = get_random_double(middle_vector[i] + RANDOMISER_MIN,
                                   middle_vector[i] + RANDOMISER_MAX);
    }

    return ans;
}

// neighbour
void find_number_of_neighbours()
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

void determine_neighbour_indexes(struct bmu *_bmu)
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






























