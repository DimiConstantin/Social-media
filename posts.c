#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"
#include "posts.h"
#include "users.h"
void create_post(int* nrpmain, post_t** posts, char* name, char* title)
{
    int nrp = *nrpmain;
    nrp++;
    posts[nrp]->id = nrp;
    if (title)
        posts[nrp]->title = strdup(title);
    else
        posts[nrp]->title = NULL;
    posts[nrp]->uid = get_user_id(name);
    posts[nrp]->nrlike = 0;
    posts[nrp]->likes = ll_create(sizeof(int));
    posts[nrp]->tree = lg_create(100);
    posts[nrp]->father = 0;
    *nrpmain = nrp;
}
void repost(int* nrpmain, post_t** posts, char* name, int id1, int id2)
{
    create_post(nrpmain, posts, name, NULL);
    int nrp = *nrpmain;
    if (id2 == 0)
        id2 = id1;
    for (int i = id2; i != 0; i = posts[i]->father) {
        lg_add_edge(posts[i]->tree, id2, nrp);
        lg_add_edge(posts[i]->tree, nrp, id2);
    }
    printf("Created Repost #%d for %s", nrp, name);
}

void handle_input_posts(char* input, post_t* posts, int* nrp)
{
    char* commands = strdup(input);
    char* cmd = strtok(commands, "\n ");
    char *name, *title;
    if (!cmd)
        return;
    if (!strcmp(cmd, "create")) {
        name = strtok(NULL, "\n ");
        title = strtok(NULL, "\n");
        create_post(nrp, posts, name, title);
        printf("Created %s for %s", title, name);
    } else if (!strcmp(cmd, "repost")) {
        name = strtok(NULL, "\n ");
        char* get_param;
        int id1, id2 = 0;
        get_param = strtok(NULL, "\n ");
        id1 = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        repost(nrp, posts, name, id1, id2);
    }
    else if (!strcmp(cmd, "common-repost"))
        (void)cmd;
    // TODO: Add function
    else if (!strcmp(cmd, "like"))
        (void)cmd;
    // TODO: Add function
    else if (!strcmp(cmd, "ratio"))
        (void)cmd;
    // TODO: Add function
    else if (!strcmp(cmd, "delete"))
        (void)cmd;
    // TODO: Add function
    else if (!strcmp(cmd, "get-likes"))
        (void)cmd;
    // TODO: Add function
    else if (!strcmp(cmd, "get-reposts"))
        (void)cmd;
    // TODO: Add function
    else if (!strcmp(cmd, "get-likes"))
        (void)cmd;
    // TODO: Add function
}
