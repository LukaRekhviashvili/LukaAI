#include "Best_match_unit.h"

struct bmu *best_match_unit;
struct bmu *first_node;
struct bmu *last_node;
double bestM_act;
int number_of_bmu_nodes = 0;

void find_bmu_linked_list(struct node ** node_map, int NODE_MAP_HEIGHT, int NODE_MAP_LENGTH)
{
    first_node = NULL;
    last_node = NULL;

    for (int i = 0; i < NODE_MAP_HEIGHT; ++i) {
        for (int j = 0; j < NODE_MAP_LENGTH; ++j) {
            add_bmu_node(i, j, node_map[i][j].act);
        }
    }
};

void add_bmu_node(int i, int j, double act)
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

void print_bmu(struct bmu *_bmu, struct node ** node_map)
{
    printf("%d %d  act: %f <-%s \n",
           _bmu->i, _bmu->j,
           node_map[_bmu->i][_bmu->j].act,
           node_map[_bmu->i][_bmu->j].label);
}
