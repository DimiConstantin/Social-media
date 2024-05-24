#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"
#include "friends.h"
#include "users.h"

void add_friend(list_graph_t* users, char* user1, char* user2)
{
    uint16_t user1_id = get_user_id(user1);
    uint16_t user2_id = get_user_id(user2);

    lg_add_edge(users, user1_id, user2_id);
    lg_add_edge(users, user2_id, user1_id);
    printf("Added connection %s - %s\n", user1, user2);
}

void remove_friend(list_graph_t* users, char* user1, char* user2)
{
    uint16_t user1_id = get_user_id(user1);
    uint16_t user2_id = get_user_id(user2);
    lg_remove_edge(users, user1_id, user2_id);
    lg_remove_edge(users, user2_id, user1_id);
    printf("Removed connection %s - %s\n", user1, user2);
}

void user_suggestions(list_graph_t* users, char* user)
{
    uint16_t user_id = get_user_id(user);
    int* distance = bfs_list_graph(users, user_id);
    int ok = 0;
    for (int i = 0; i < users->nodes && !ok; i++)
        if (distance[i] == 2)
            ok = 1;
    if (ok == 0) {
        printf("There are no suggestions for %s\n", user);
    } else {
        printf("Suggestions for %s:\n", user);
        for (int i = 0; i < users->nodes; i++)
            if (distance[i] == 2) {
                printf("%s\n", get_user_name(i));
                ok = 1;
            }
    }
    free(distance);
}

void find_distance(list_graph_t* users, char* user1, char* user2)
{
    uint16_t user1_id = get_user_id(user1);
    uint16_t user2_id = get_user_id(user2);
    int* distance = bfs_list_graph(users, user1_id);
    if (distance[user2_id] == -1 || distance[user2_id] == 0)
        printf("There is no way to get from %s to %s\n", user1, user2);
    else
        printf("The distance between %s - %s is %d\n", user1,
            user2, distance[user2_id]);
    free(distance);
}

void find_common_friends(list_graph_t* users, char* user1, char* user2)
{
    uint16_t user1_id = get_user_id(user1);
    uint16_t user2_id = get_user_id(user2);
    linked_list_t* friends1 = users->neighbors[user1_id];
    linked_list_t* friends2 = users->neighbors[user2_id];
    ll_node_t* curr1 = friends1->head;
    ll_node_t* curr2 = friends2->head;
    uint16_t* common_friends = malloc(users->nodes * sizeof(uint16_t));
    int ok = 0, ct = 0;

    while (curr1 != NULL) {
        while (curr2 != NULL) {
            if (*((int*)curr1->data) == *((int*)curr2->data)) {
                common_friends[ct++] = *((int*)curr1->data);
                ok = 1;
            }
            curr2 = curr2->next;
        }
        curr1 = curr1->next;
        curr2 = friends2->head;
    }

    for (int i = 0; i < ct - 1; i++) {
        for (int j = 0; j < ct - i - 1; j++) {
            if (common_friends[j] > common_friends[j + 1]) {
                uint16_t temp = common_friends[j];
                common_friends[j] = common_friends[j + 1];
                common_friends[j + 1] = temp;
            }
        }
    }

    if (ok == 1) {
        printf("The common friends between %s and %s are:\n", user1, user2);
        for (int i = 0; i < ct; i++)
            printf("%s\n", get_user_name(common_friends[i]));
    } else if (ok == 0)
        printf("No common friends for %s and %s\n", user1, user2);
    free(common_friends);
}

void find_the_popular_one(list_graph_t* users, char* user)
{
    uint16_t user_id = get_user_id(user);
    linked_list_t* friends = users->neighbors[user_id];
    int max_friends = friends->size;
    uint16_t max_id = user_id;
    ll_node_t* curr = friends->head;
    while (curr) {
        int id = *((int*)curr->data);
        if (users->neighbors[id]->size > max_friends) {
            max_friends = users->neighbors[id]->size;
            max_id = id;
        }
        curr = curr->next;
    }
    if (max_id == user_id)
        printf("%s is the most popular\n", user);
    else
        printf("%s is the most popular friend of %s\n",
            get_user_name(max_id), user);
}

void handle_input_friends(char* input, list_graph_t* users)
{
    char* commands = strdup(input);
    char* cmd = strtok(commands, "\n ");

    if (!cmd)
        return;

    if (!strcmp(cmd, "add")) {
        char* user1 = strtok(NULL, "\n ");
        char* user2 = strtok(NULL, "\n ");
        add_friend(users, user1, user2);
    } else if (!strcmp(cmd, "remove")) {
        char* user1 = strtok(NULL, "\n ");
        char* user2 = strtok(NULL, "\n ");
        remove_friend(users, user1, user2);
    } else if (!strcmp(cmd, "suggestions")) {
        char* user = strtok(NULL, "\n ");
        user_suggestions(users, user);
    } else if (!strcmp(cmd, "distance")) {
        char* user1 = strtok(NULL, "\n ");
        char* user2 = strtok(NULL, "\n ");
        find_distance(users, user1, user2);
    } else if (!strcmp(cmd, "common")) {
        char* user1 = strtok(NULL, "\n ");
        char* user2 = strtok(NULL, "\n ");
        find_common_friends(users, user1, user2);
    } else if (!strcmp(cmd, "friends")) {
        char* user = strtok(NULL, "\n ");
        printf("%s has %d friends\n", user,
            users->neighbors[get_user_id(user)]->size);
    } else if (!strcmp(cmd, "popular")) {
        char* user = strtok(NULL, "\n ");
        find_the_popular_one(users, user);
    }
    free(commands);
}
