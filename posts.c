#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"
#include "posts.h"
#include "users.h"

void dfs(int id, post_t** posts)
{
    printf("Repost #%d by %s\n", id, get_user_name(posts[id]->uid));
    ll_node_t* nod = posts[id]->tree->neighbors[id]->head;
    while (nod) {
        dfs(*(int*)nod->data, posts);
        nod = nod->next;
    }
}

void create_post(int* nrpmain, post_t** posts, char* name, char* title)
{
    int nrp = *nrpmain;
    nrp++;
    posts[nrp]->id = nrp;
    if (title) {
        posts[nrp]->title = malloc(strlen(title) + 1);
        DIE(!posts[nrp]->title, "title malloc failed");
        strcpy(posts[nrp]->title, title);
    } else
        posts[nrp]->title = NULL;
    posts[nrp]->uid = get_user_id(name);
    posts[nrp]->father = 0;
    posts[nrp]->nrlike = 0;
    posts[nrp]->likes = ll_create(sizeof(int));
    posts[nrp]->tree = lg_create(NOD_MAX);
    *nrpmain = nrp;
}

void repost(int* nrpmain, post_t** posts, char* name, int id1, int id2)
{
    create_post(nrpmain, posts, name, NULL);
    int nrp = *nrpmain;
    if (id2 == 0)
        id2 = id1;
    posts[nrp]->father = id2;
    for (int i = id2; i != 0; i = posts[i]->father) {
        lg_add_edge(posts[i]->tree, id2, nrp);
        lg_add_edge(posts[i]->tree, nrp, id2);
    }
    printf("Created repost #%d for %s\n", nrp, name);
}

void get_reposts(post_t** posts, int post, int repost)
{
    int id = post;
    if (repost == 0) {
        printf("%s - Post by ", posts[post]->title);
        printf("%s\n", get_user_name(posts[post]->uid));
    } else {
        printf("Repost #%d by %s\n", repost,
            get_user_name(posts[repost]->uid));
        id = repost;
    }
    ll_node_t* nod = posts[id]->tree->neighbors[id]->head;
    while (nod) {
        dfs(*(int*)nod->data, posts);
        nod = nod->next;
    }
}

void delete(post_t** posts, int id1, int id2)
{
    if (id2) {
        printf("Deleted repost #%d of post %s\n", id2, posts[id1]->title);
        for (int i = posts[id2]->father; i != 0; i = posts[i]->father) {
            lg_remove_edge(posts[i]->tree, id2, i);
            lg_remove_edge(posts[i]->tree, i, id2);
        }
    } else
        printf("Deleted %s\n", posts[id1]->title);
}

void common_repost(post_t** posts, int post, int id1, int id2)
{
    int height1 = 0, height2 = 0, copy1 = id1, copy2 = id2;
    for (int i = id1; i != post; i = posts[i]->father)
        height1++;
    for (int i = id2; i != post; i = posts[i]->father)
        height2++;
    if (height1 < height2) {
        for (int i = 0; i < height2 - height1; i++)
            id2 = posts[id2]->father;
    } else
        for (int i = 0; i < height1 - height2; i++)
            id1 = posts[id1]->father;
    while (id1 != id2) {
        id1 = posts[id1]->father;
        id2 = posts[id2]->father;
    }
    printf("The first common repost of %d and %d is %d\n", copy1, copy2, id1);
}

void like_the_post(post_t** posts, int post_id, char* name, int repost_id)
{
    int user_id = get_user_id(name);
    int type = 1; // postare
    int father_post = post_id;
    if (repost_id != 0) {
        post_id = repost_id;
        type = 2; // repostare
    }
    if (posts[post_id]->nrlike == 0) {
        ll_add_nth_node(posts[post_id]->likes, 0, &user_id); // adaug like
        posts[post_id]->nrlike++;
        if (type == 1)
            printf("User %s liked post %s\n", name, posts[father_post]->title);
        else
            printf("User %s liked repost %s\n", name,
                posts[father_post]->title);
        return;
    }
    ll_node_t* curr = posts[post_id]->likes->head;
    int pos = 0;
    while (curr) {
        if (*(int*)curr->data == user_id) {
            ll_node_t* removed = ll_remove_nth_node(posts[post_id]->likes,
                pos);
            free(removed->data);
            free(removed);
            posts[post_id]->nrlike--;
            if (type == 1)
                printf("User %s unliked post %s\n", name,
                    posts[father_post]->title);
            else
                printf("User %s unliked repost %s\n", name,
                    posts[father_post]->title);
            return;
        }
        pos++;
        curr = curr->next;
    }
    ll_add_nth_node(posts[post_id]->likes, 0, &user_id); // adaug like
    posts[post_id]->nrlike++;
    if (type == 1)
        printf("User %s liked post %s\n", name, posts[father_post]->title);
    else
        printf("User %s liked repost %s\n", name, posts[father_post]->title);
}

void get_likes(post_t** posts, int post_id, int repost_id)
{
    if (repost_id == 0)
        printf("Post %s has %d likes\n", posts[post_id]->title,
            posts[post_id]->nrlike);
    else
        printf("Repost #%d has %d likes\n", repost_id,
            posts[repost_id]->nrlike);
}

void ratio(post_t** posts, int post_id, int* nrp)
{
    int max_likes = posts[post_id]->nrlike;
    int max_idx = 0;
    for (int i = 1; i <= *nrp; i++) {
        if (posts[i]->father == post_id) {
            if (posts[i]->nrlike > max_likes) {
                max_likes = posts[i]->nrlike;
                max_idx = i;
            }
        }
    }
    if (max_idx == 0)
        printf("The original post is the highest rated\n");
    else
        printf("Post %d got ratio'd by repost %d\n", post_id, max_idx);
}

void handle_input_posts(char* input, post_t** posts, int* nrp)
{
    char* commands = strdup(input);
    DIE(!commands, "mai bine dadeam comanda pe glovo");
    char* cmd = strtok(commands, "\n ");
    char *name, *title;
    if (!cmd)
        return;
    if (!strcmp(cmd, "create")) {
        name = strtok(NULL, "\n ");
        title = strtok(NULL, "\n");
        create_post(nrp, posts, name, title);
        printf("Created %s for %s\n", title, name);
    } else if (!strcmp(cmd, "repost")) {
        name = strtok(NULL, "\n ");
        char* get_param;
        int id1, id2 = 0;
        get_param = strtok(NULL, "\n ");
        id1 = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        if (get_param)
            id2 = atoi(get_param);
        repost(nrp, posts, name, id1, id2);
    } else if (!strcmp(cmd, "common-repost")) {
        char* get_param = strtok(NULL, "\n ");
        int post = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        int id1 = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        int id2 = atoi(get_param);
        common_repost(posts, post, id1, id2);
    } else if (!strcmp(cmd, "like")) {
        name = strtok(NULL, "\n ");
        char* get_param = strtok(NULL, "\n ");
        int post_id = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        int repost_id = 0;
        if (get_param)
            repost_id = atoi(get_param);
        like_the_post(posts, post_id, name, repost_id);
    }
    else if (!strcmp(cmd, "ratio")) {
        char* get_param = strtok(NULL, "\n ");
        int post_id = atoi(get_param);
        ratio(posts, post_id, nrp);
    } else if (!strcmp(cmd, "delete")) {
        char* get_param;
        int id1, id2 = 0;
        get_param = strtok(NULL, "\n ");
        id1 = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        if (get_param)
            id2 = atoi(get_param);
        delete (posts, id1, id2);
    } else if (!strcmp(cmd, "get-likes")) {
        char* get_param = strtok(NULL, "\n ");
        int post_id = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        int repost_id = 0;
        if (get_param)
            repost_id = atoi(get_param);
        get_likes(posts, post_id, repost_id);
    } else if (!strcmp(cmd, "get-reposts")) {
        char* get_param;
        int id1, id2 = 0;
        get_param = strtok(NULL, "\n ");
        id1 = atoi(get_param);
        get_param = strtok(NULL, "\n ");
        if (get_param)
            id2 = atoi(get_param);
        get_reposts(posts, id1, id2);
    }
    free(commands);
}
