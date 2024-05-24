#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"
#include "feed.h"
#include "users.h"

void get_feed(post_t** posts, list_graph_t* users,
    char* name, int feed_size, int* nrp)
{
    int id = get_user_id(name);
    for (int i = *nrp; i >= 1 && feed_size; --i) {
        if ((lg_has_edge(users, id, posts[i]->uid)
                || id == posts[i]->uid)
            && posts[i]->title) {
            printf("%s: %s\n", get_user_name(posts[i]->uid), posts[i]->title);
            --feed_size;
        }
    }
}

void view_profile(list_graph_t* users, post_t** posts, char* name, int* nrp)
{
    int id = get_user_id(name);
    for (int i = 1; i <= *nrp; ++i) {
        if (posts[i]->uid == id) {
            if (!posts[i]->title)
                printf("Reposted: %s\n", posts[posts[i]->father]->title);
            else
                printf("Posted: %s\n", posts[i]->title);
        }
    }
}

void friends_repost(post_t** posts, list_graph_t* users, char* name,
    int id, int* nrp)
{
    int user_id = get_user_id(name);
    for (int i = 1; i <= *nrp; ++i)
        if (lg_has_edge(users, user_id, posts[i]->uid)
            && posts[i]->father == id)
            printf("%s\n", get_user_name(posts[i]->uid));
}

void printClique(int* Rmain, int rsize)
{
    int R[USR_MAX];
    for (int i = 0; i < rsize; i++)
        R[i] = Rmain[i];
    for (int i = 0; i < rsize - 1; ++i)
        for (int j = i + 1; j < rsize; ++j)
            if (R[i] > R[j]) {
                int aux = R[i];
                R[i] = R[j];
                R[j] = aux;
            }
    for (int i = 0; i < rsize; ++i)
        printf("%s\n", get_user_name(R[i]));
}

void BronKerbosch(list_graph_t* users, int* R, int* P, int* X, int rsize,
    int psize, int xsize, int* rmax, int* rmaxsize)
{
    if (psize == 0 && xsize == 0) {
        if (*rmaxsize < rsize) {
            for (int i = 0; i < rsize; i++)
                rmax[i] = R[i];
            *rmaxsize = rsize;
        }
        return;
    }
    for (int i = 0; i < psize; i++) {
        int nod = P[i];
        R[rsize] = nod;
        int newP[USR_MAX] = { 0 }, newpsize = 0;
        int newX[USR_MAX] = { 0 }, newxsize = 0;
        for (int j = 0; j < psize; j++) {
            if (lg_has_edge(users, nod, P[j])) {
                newP[newpsize] = P[j];
                newpsize++;
            }
        }
        for (int j = 0; j < xsize; j++) {
            if (lg_has_edge(users, nod, X[j])) {
                newX[newxsize] = X[j];
                newxsize++;
            }
        }
        BronKerbosch(users, R, newP, newX, rsize + 1, newpsize, newxsize,
            rmax, rmaxsize);

        P[i] = P[psize - 1];
        psize--;
        X[xsize++] = nod;
    }
}

void common_group(int uid, list_graph_t* users)
{

    int R[USR_MAX], P[USR_MAX] = { 0 }, X[USR_MAX] = { 0 }, i = 1, rmax[USR_MAX], rmaxsize = 0;
    ll_node_t* nod = users->neighbors[uid]->head;
    P[0] = uid;
    while (nod) {
        P[i] = *(int*)nod->data;
        i++;
        nod = nod->next;
    }
    BronKerbosch(users, R, P, X, 0, i, 0, rmax, &rmaxsize);
    if (rmaxsize)
        printClique(rmax, rmaxsize);
    else
        printf("%s\n", get_user_name(uid));
}

void handle_input_feed(char* input, post_t** posts, list_graph_t* users, int* nrp)
{
    char* commands = strdup(input);
	DIE(!commands, "mai bine dadeam comanda pe glovo");
    char* cmd = strtok(commands, "\n ");

    if (!cmd)
        return;

    if (!strcmp(cmd, "feed")) {
        char* name = strtok(NULL, "\n ");
        char* get_param = strtok(NULL, "\n ");
        int feed_size = atoi(get_param);
        get_feed(posts, users, name, feed_size, nrp);
    } else if (!strcmp(cmd, "view-profile")) {
        char* name = strtok(NULL, "\n ");
        view_profile(users, posts, name, nrp);
    } else if (!strcmp(cmd, "friends-repost")) {
        char* name = strtok(NULL, "\n ");
        char* get_param = strtok(NULL, "\n ");
        int id = atoi(get_param);
        friends_repost(posts, users, name, id, nrp);
    } else if (!strcmp(cmd, "common-group")) {
        char* get_param = strtok(NULL, "\n ");
        int uid = get_user_id(get_param);
        printf("The closest friend group of %s is:\n", get_user_name(uid));
        common_group(uid, users);
    }
    free(commands);
}
