#define MAX_DEPTH 5
#define MAX_NUMBER_OF_NODES 48
#define NUMBER_OF_FACTORS 5
#define MAX_NUMBER_OF_NODES_IN_STABLE 1358954496 // popravi
#define STACK_SPACE 100000

// konstante za heuristiku
#define MAX_IN_CORNER 1024
#define MAX_NOT_IN_CORNER_PENALTY -100
#define MONOTONICITY 512
#define MOVES 64
#define NO_MONOTONICITY_PENALTY -64
#define NO_MOVES_PENALTY -1024 // *HIGH
#define EMPTY_SPACES 10

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