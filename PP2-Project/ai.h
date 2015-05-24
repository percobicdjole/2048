#include <stdlib.h>


#define MAX_DEPTH 2
#define MAX_NUMBER_OF_NODES 48
#define NUMBER_OF_FACTORS 5
#define MAX_NUMBER_OF_NODES_IN_STABLE 1358954496 // popravi

// konstante za heuristiku
#define MAX_IN_CORNER 100
#define MAX_NOT_IN_CORNER_PENALTY -100
#define MONOTONICITY 30
#define MOVES 50
#define NO_MOVES_PENALTY -100 // *HIGH
#define EMPTY_SPACES 10

typedef struct tnode
{
	int **table;
	int table_size;
	int level;
	float weight, possibility;
	struct tnode *next[MAX_NUMBER_OF_NODES];
} T_node;

/// \TO-DO promijeni weight u float

// vraca najbolji potez
int get_hint(matrix table);

void make_tree_normal_move(T_node *root, int level);
void make_tree_random_move(T_node *root, int level);

// trazi najbolji potez
void expectimax_search(T_node *root);

T_node* get_node(int **table, int table_size, int level);
void free_stable(T_node *root);

// za postorder obilazak
void push(T_node **stack, T_node* elem, int *top);
T_node* pop(T_node **stack, int *top);

// heuristika
float approximate_position(int **table, int table_size);
void number_of_moves_horizontally(int **table, int table_size, float *score);
void number_of_moves_vertically(int **table, int table_size, float *score);