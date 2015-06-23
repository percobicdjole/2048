#ifndef AI_H
#define AI_H

#define MAX_DEPTH 5
#define MAX_NUMBER_OF_NODES 48
#define NUMBER_OF_FACTORS 5
#define STACK_SPACE 100000

// konstante za heuristiku // sto manje velikih brojeva
#define MAX_IN_CORNER 256 
#define MAX_NOT_IN_CORNER_PENALTY -16 
#define MONOTONICITY 150 
#define MOVES 5500
#define NO_MONOTONICITY_PENALTY -512
#define NO_MOVES_PENALTY -4096
#define EMPTY_SPACES 4096 
#define BIG_NUMBERS_PENALTY -50 

typedef struct tnode
{
	int **table;
	int table_size;
	int level;
	float weight, possibility;
	struct tnode *next[MAX_NUMBER_OF_NODES];
} T_node;

// vraca najbolji potez
int get_hint(matrix table);

// pravi stablo sa svim mogucim potezima do odredjenog nivoa
void make_tree_iterative(T_node *root, T_node ***stack, int *stack_space);

// trazi najbolji potez
void expectimax_search(T_node *root, T_node ***stack, int *stack_space);

T_node* get_node(int **table, int table_size, int level);
void free_tree(T_node *root, T_node ***stack, int *stack_space);

// za postorder obilazak
void push(T_node ***stack, T_node* elem, int *top, int *stack_space);
T_node* pop(T_node **stack, int *top);

// heuristika
float approximate_position(int **table, int table_size);
void number_of_moves_horizontally(int **table, int table_size, float *score);
void number_of_moves_vertically(int **table, int table_size, float *score);

#endif /* AI_H */