/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "data_structures.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	list_graph_t *users = lg_create(520);
	post_t **posts = malloc(sizeof(post_t *) * 500);
		for (int i = 0; i < 500; i++) {
			posts[i] = malloc(sizeof(post_t));
		}
	int nrp = 0;
	while (1) {
		// If fgets returns null, we reached EOF
		if (!fgets(input, MAX_COMMAND_LEN, stdin))
			break;

		#ifdef TASK_1
		handle_input_friends(input, users);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, posts, &nrp);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, posts, users, &nrp);
		#endif
	}
	lg_free(users);
	free(input);
	free_users();
	for (int i = 1; i <= nrp ; i++) {
		if (posts[i]->title)
			free(posts[i]->title);
		ll_free(&posts[i]->likes);
		lg_free(posts[i]->tree);
		free(posts[i]);
	}
	free(posts[0]);
	for (int i = nrp + 1; i < 500; ++i)
		free(posts[i]);
	free(posts);
	return 0;
}
