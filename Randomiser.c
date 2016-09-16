#include "Randomiser.h"

double RANDOMISER_MAX = 0.02, RANDOMISER_MIN = -0.05;

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
        ans[i] = get_random_double(middle_vector[i] + RANDOMISER_MIN,
                                   middle_vector[i] + RANDOMISER_MAX);
    }

    return ans;
}
