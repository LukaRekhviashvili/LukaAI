#include "Input_data_vect.h"

int number_of_data_vectors, data_vector_size;
struct data_vect * data;


// private function declarations
void initialise_nums();
void initialise_data();

void find_normals();
void normalise_each_vector();

// initialisation
void initialise_input_data_vector()
{
    open_file_stream();

    initialise_nums();
    initialise_data();
}

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


// normalisation
void normilise_input_data_vector()
{
    find_normals();
    normalise_each_vector();
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

void normalise_each_vector()
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
