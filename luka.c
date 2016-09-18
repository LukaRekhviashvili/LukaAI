// header files
#include "File_reader.h"
#include "Neighbour.h"
#include "Randomiser.h"
#include "Best_match_unit.h"

// Structures
//struct data_vect {
//    double * vect;
//    double norm;
//    char * name;
//};

//struct node {
//    double * w;
//    double act;
//    char * label;
//};

// program variables
int number_of_data_vectors, data_vector_size;
struct data_vect * data;
//struct node ** node_map;
//int NODE_MAP_LENGTH = 11, NODE_MAP_HEIGHT = 6;

// function declaration
void initialise_nums();
void initialise_data();
//void initialise_node_map();
//void print_node_map_names();
void print_data_element(struct data_vect data);
double euclid_dist(double *V1, double *V2, int v_size);
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


    intitialise_shuffled_vect(number_of_data_vectors);

    //print_data(data);

    initialise_randomised_data_vect(data, number_of_data_vectors, data_vector_size);

    //print_data(randomised_data);


    initialise_node_map(data_vector_size, data);

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




// others




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
