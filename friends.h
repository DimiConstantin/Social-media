#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 530
#define MAX_PEOPLE 550
#include "data_structures.h"

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *users);

#endif // FRIENDS_H
