#include "data_structures.h"

linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t* ll;
    
    ll = malloc(sizeof(*ll));
    
    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;
    
    return ll;
}

ll_node_t* get_nth_node(linked_list_t* list, unsigned int n)
{
    if (!list) {
        return NULL;
    }
    
    unsigned int len = list->size - 1;
    unsigned int i;
    ll_node_t* node = list->head;
    
    n = MIN(n, len);
    
    for (i = 0; i < n; ++i) {
        node = node->next;
    }
    
    return node;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;
    
    if (!list) {
        return;
    }
    
    /* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
    if (n > list->size) {
        n = list->size;
    }
    
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }
    
    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);
    
    new_node->next = curr;
    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = new_node;
    } else {
        prev->next = new_node;
    }
    
    list->size++;
}

ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;
    
    if (!list || !list->head) {
        return NULL;
    }
    
    /* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
    if (n > list->size - 1) {
        n = list->size - 1;
    }
    
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }
    
    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }
    
    list->size--;
    
    return curr;
}

unsigned int
ll_get_size(linked_list_t* list)
{
    if (!list) {
        return -1;
    }
    
    return list->size;
}


void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;
    
    if (!pp_list || !*pp_list) {
        return;
    }
    
    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }
    
    free(*pp_list);
    *pp_list = NULL;
}

void
ll_print_int(linked_list_t* list)
{
    ll_node_t* curr;
    if (!list) {
        return;
    }
    if (!list->head)
        return;
    curr = list->head;
    while (curr != NULL) {
        printf("%d ", *((int*)curr->data));
        curr = curr->next;
    }
    
    printf("\n");
}

//  ---------GRAPH FUNCTIONS------------  //

list_graph_t*
lg_create(int nodes)
{
	list_graph_t *graph = malloc(sizeof(list_graph_t));
    graph->nodes = nodes;
    graph->neighbors = (linked_list_t **)malloc(nodes * sizeof(linked_list_t));
    for(int i = 0; i < nodes; ++i)
        graph->neighbors[i] = ll_create(sizeof(int));
    return graph;
}

int is_node_in_graph(int n, int nodes)
{
   return n >= 0 && n < nodes;
}

/* Adauga o muchie intre nodurile primite ca parametri */
void
lg_add_edge(list_graph_t* graph, int src, int dest)
{
      if (
       !graph || !graph->neighbors
       || !is_node_in_graph(src, graph->nodes)
       || !is_node_in_graph(dest, graph->nodes)
   )
       return;


   ll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, &dest);
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int
lg_has_edge(list_graph_t* graph, int src, int dest)
{
	if (!graph->neighbors[src]->head)
        return 0;
    for (ll_node_t *curr = graph->neighbors[src]->head; curr != NULL; curr = curr->next)
        if(*((int *)curr->data) == dest)
            return 1;
    return 0;
}

/* Elimina muchia dintre nodurile primite ca parametri */
void
lg_remove_edge(list_graph_t* graph, int src, int dest)
{
    printf("src %d dest %d\n", src, dest);
	if (!lg_has_edge(graph, src, dest))
        return;
    int i = 0;
    for (ll_node_t *current = graph->neighbors[src]->head; current != NULL; current = current->next) {
        if (*((int *)current->data) == dest)
            break;
        i++;
    }
    ll_node_t *removed = ll_remove_nth_node(graph->neighbors[src], i);
    free(removed->data);
    free(removed);
}

/* Elibereaza memoria folosita de lista de adiacenta a grafului */
void
lg_free(list_graph_t* graph)
{
	for (int i = 0; i < graph->nodes; ++i)
        ll_free(&graph->neighbors[i]);
    free(graph->neighbors);
    free(graph);
}

/* Printeaza lista de adiacenta a grafului
 */
void
lg_print_graph(list_graph_t* graph)
{
	for(int i = 0; i < graph->nodes; ++i) {
        if (graph->neighbors[i]->head)
            printf("%d: ", i);
        ll_print_int(graph->neighbors[i]);
    }
}

// ---------QUEUE FUNCTIONS------------  //

queue_t*
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t* q = calloc(1, sizeof(*q));

	q->data_size = data_size;
	q->max_size = max_size;

	q->buff = malloc(max_size * sizeof(*q->buff));

	return q;
}

/*
 * Functia intoarce numarul de elemente din coada al carei pointer este trimis
 * ca parametru.
 */
unsigned int
q_get_size(queue_t *q)
{
    return q->size;
}

/*
 * Functia intoarce 1 daca coada este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
    if (q_get_size(q) == 0)
        return 1;
	return 0;
}

/* 
 * Functia intoarce primul element din coada, fara sa il elimine.
 */
void *
q_front(queue_t *q)
{
	if (q_is_empty(q))
        return NULL;

    return q->buff[q->read_idx];
}

/*
 * Functia scoate un element din coada. Se va intoarce 1 daca operatia s-a
 * efectuat cu succes (exista cel putin un element pentru a fi eliminat) si
 * 0 in caz contrar.
 */
int
q_dequeue(queue_t *q)
{
	if (q->size == 0) {
        return 0;
    }

    free(q->buff[q->read_idx]);

    q->size--;
    q->read_idx = (q->read_idx + 1) % q->max_size;
    return 1;
}

/* 
 * Functia introduce un nou element in coada. Se va intoarce 1 daca
 * operatia s-a efectuat cu succes (nu s-a atins dimensiunea maxima) 
 * si 0 in caz contrar.
 */
int
q_enqueue(queue_t *q, void *new_data)
{
    if (q->write_idx >= q->max_size)
        return 0;
    q->buff[q->write_idx] = malloc(q->data_size);
    memcpy(q->buff[q->write_idx], new_data, q->data_size);
    q->write_idx = (q->write_idx + 1) % q->max_size;
    q->size++;
	return 1;
}

/*
 * Functia elimina toate elementele din coada primita ca parametru.
 */
void
q_clear(queue_t *q)
{
    if (q_get_size(q) == 0)
        return;
    while (q_get_size(q) > 0)
        q_dequeue(q);
}

/*
 * Functia elibereaza toata memoria ocupata de coada.
 */
void
q_free(queue_t *q)
{
    if (!q)
        return;
	q_clear(q);
    free(q->buff);
}

int *bfs_list_graph(list_graph_t* lg, int node) // functie care intoarce un vector de distante de la un nod la toate celelalte noduri
{
	int *colour = calloc(lg->nodes, sizeof(int));
	int *dist = calloc(lg->nodes, sizeof(int));
	memset(dist, 9999, lg->nodes);
	ll_node_t **parents = calloc(lg->nodes, sizeof(ll_node_t *));
	queue_t *q = q_create(sizeof(int), lg->nodes);
	colour[node] = 1;
	dist[node] = 0;
	q_enqueue(q, &node);
	while(!q_is_empty(q)) {
		int nr = *((int *)q_front(q));
		q_dequeue(q);
		if(lg->neighbors[nr]->head) {
			ll_node_t *current = lg->neighbors[nr]->head;
			while(current) {
				int node_idx = *((int *)current->data);
				if (colour[node_idx] == 0) {
					colour[node_idx] = 1;
					parents[node_idx] = current;
					dist[node_idx] = dist[nr] + 1;
					q_enqueue(q, current->data);
				}
				current = current->next;
			}
		}
		colour[nr] = 2;
	}
    free(colour);
    free(parents);
    q_free(q);
    return dist;
}