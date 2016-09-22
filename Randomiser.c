#include "Randomiser.h"


// private variables
double * middle_vector;

// public variables
double RANDOMISER_MAX = 0.02, RANDOMISER_MIN = -0.05;
struct data_vect * randomised_data;
int * shuffled_vector;

// private functions
void initialise_middle_vector(struct data_vect *data, int number_of_data_vectors, int data_vector_size);

void initialise_middle_vector(struct data_vect *data, int number_of_data_vectors, int data_vector_size)
{
    middle_vector = (double *)malloc(data_vector_size * sizeof(double));

    //printf("printing middle vector\n");
    //for(int i=0;i<data_vector_size; i++){
    //    printf("%f ", middle_vector[i]);
    //}
    //printf("printing middle vector ENDDDDDDD\n");

    for (int i = 0; i < data_vector_size; ++i) {
        for (int j = 0; j < number_of_data_vectors; ++j) {
            middle_vector[i] += data[j].vect[i];
        }

        middle_vector[i] /= number_of_data_vectors;
    }
}

// public functions
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

double * get_random_vector(int data_vector_size, double * middle_vector)
{
    double * ans = (double *)malloc(data_vector_size * sizeof(double));

    for (int i = 0; i < data_vector_size; ++i) {
        //ans[i] = get_random_double(middle_vector[i] + RANDOMISER_MIN, middle_vector[i] + RANDOMISER_MAX);
        ans[i] = get_random_double(0.0, 1.0);
    }

    return ans;
}

void initialise_randomised_data_vect(struct data_vect *data, int number_of_data_vectors, int data_vector_size)
{
    // initialise middle vector
    initialise_middle_vector(data, number_of_data_vectors, data_vector_size);


    // initialise randomised data
    randomised_data = (struct data_vect *)malloc(number_of_data_vectors * sizeof(struct data_vect));

    for (int i = 0; i < number_of_data_vectors; ++i) {
        randomised_data[i].vect = get_random_vector(data_vector_size, middle_vector);

        for (int j = 0; j < data_vector_size; ++j) {
            randomised_data[i].norm += randomised_data[i].vect[j] * randomised_data[i].vect[j];
        }
        randomised_data[i].norm = sqrt(randomised_data[i].norm);
    }
}

void intitialise_shuffled_vect(int number_of_data_vectors)
{
    shuffled_vector = malloc(number_of_data_vectors * sizeof(int));

    shuffle_vect(number_of_data_vectors);
}


void shuffle_vect(int number_of_data_vectors)
{
    for (int i = 0; i < number_of_data_vectors; ++i) {
        shuffled_vector[i] = i;
    }

    int temp, rand_n;

    for (int i = 0; i < number_of_data_vectors; ++i) {
        rand_n = get_random_int(0, number_of_data_vectors);

        temp = shuffled_vector[i];
        shuffled_vector[i] = shuffled_vector[rand_n];
        shuffled_vector[rand_n] = temp;
    }
}
