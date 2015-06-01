#define MAX_DEPTH 5
#define MAX_NUMBER_OF_NODES 48
#define NUMBER_OF_FACTORS 5
#define MAX_NUMBER_OF_NODES_IN_STABLE 1358954496 // popravi
#define STACK_SPACE 100000

// konstante za heuristiku // sto manje velikih brojeva
#define MAX_IN_CORNER 4096 // 1024 // 4096
#define MAX_NOT_IN_CORNER_PENALTY -128 // 100  // -128
#define MONOTONICITY 1024 // 512 mozda da se smanji  // 1024
#define MOVES 256// 64 // 256
#define NO_MONOTONICITY_PENALTY -128 // nije ga bilo // -256
#define NO_MOVES_PENALTY -2048 // -1024 // -2048
#define EMPTY_SPACES 256 // 8 // 512
#define BIG_NUMBERS_PENALTY -4096 // nije ga bilo

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