#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"
#include "posts.h"
#include "users.h"
void create_post(int* nrpmain, post_t* posts, char* name, char* title)
{
    int nrp = *nrpmain;
    nrp++;
    posts[nrp].id = nrp;
    strcpy(posts[nrp].title, title);
    posts[nrp].uid = get_user_id(name);
    posts[nrp].nrlike = 0;
    posts[nrp].likes = ll_create(sizeof(int));
    posts[nrp].tree = lg_create(100);
    printf("Created %s for %s", title, name);
    *nrpmain = nrp;
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
    } else if (!strcmp(cmd, "repost"))
        (void)cmd;
    // TODO: Add function
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
