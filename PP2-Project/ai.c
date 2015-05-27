#include <stdlib.h>
#include "logic.h"
#include "ai.h"
#include "math.h"

T_node* get_node(int **table, int table_size, int level)
{
	T_node *node;
	node = calloc(1, sizeof(T_node));
	
	if (node != NULL)
	{
		int i, number_of_nodes;

		number_of_nodes = 2* (table_size * table_size - 1);
		node->table = malloc(table_size*sizeof(int*));
		if (node->table == NULL)
			exit(3);
		for (i = 0; i < table_size; i++)
		{
			node->table[i] = malloc(table_size*sizeof(int)); 
			if (node->table[i] == NULL)
				exit(4);
			memcpy(node->table[i], table[i], table_size*sizeof(int));
		}
		node->table_size = table_size;
		node->weight = -1;
		node->level = level;
		return node;
	}
	else
		exit(1);
}

void free_tree(T_node *root, T_node ***stack, int *stack_space)
{
	int top = 0, i;
	int number_of_elems = 2 * (root->table_size*root->table_size - 1);
	T_node *old;
	push(stack, root, &top, stack_space);
	while (top != 0)
	{
		root = pop(*stack, &top);
		while (root != NULL)
		{
			i = 1;
			if (root->level % 2 == 0)
			{
				while (i < 4)
				{
					if (root->next[i] != NULL)
						push(stack, root->next[i], &top, stack_space);
					i++;
				}
			}
			else
			{
				while (i < number_of_elems && root->next[i] != NULL)
				{
					push(stack, root->next[i], &top, stack_space);
					i++;
				}
			}
			old = root;
			root = root->next[0];
			for (i = 0; i < old->table_size; i++)
				free(old->table[i]);
			free(old->table);
			free(old);
		}
	}
}

void push(T_node ***stack, T_node *elem, int *top, int *stack_space)
{
	if ((*top+1) % *stack_space == 0 && *top+1 != 0)
	{
		T_node **tmp;
		*stack_space += STACK_SPACE;
		tmp = realloc(*stack, (*stack_space + STACK_SPACE)*sizeof(T_node*)); // zasto?
		if (tmp == NULL)
			exit(2);
		*stack = tmp;
	}
	(*stack)[*top] = elem;
	(*top)++;
}

T_node* pop(T_node **stack, int *top)
{
	(*top)--;
	return (stack[*top]);
}

void make_tree_iterative(T_node *root, T_node ***stack, int *stack_space)
{
	T_node *helping_node;
	matrix *M = malloc(sizeof(matrix));
	int i, j, counter, top = 0;

	helping_node = root;
	push(stack, helping_node, &top, stack_space);
	while (top != 0)
	{
		helping_node = pop(*stack, &top);
	    if (helping_node->level % 2 == 0 && helping_node->level != MAX_DEPTH)
		{
			counter = 0;
			for (i = 0; i < 4; i++)
			{
				helping_node->next[i] = get_node(helping_node->table, helping_node->table_size, helping_node->level+1);
				if (snap(helping_node->next[i]->table, helping_node->next[i]->table_size, i, M) == 0)
				{
					for (j = 0; j < root->table_size; j++)
						free(helping_node->next[i]->table[j]);
					free(helping_node->next[i]->table);
				 	free(helping_node->next[i]); // kako skloniti
				 	helping_node->next[i] = NULL;
					counter++;
				}
				else
				{
					helping_node->next[i]->possibility = 1;
					push(stack, helping_node->next[i], &top, stack_space);
				}
			}
			if (counter == 4)
				helping_node->weight = 0;
		}
		else if (helping_node->level != MAX_DEPTH)
		{
			counter = 0;
			for (i = 0; i < root->table_size; i++)
			{
				for (j = 0; j < root->table_size; j++)
				{
					if (helping_node->table[i][j] == 0)
					{
						helping_node->next[counter] = get_node(helping_node->table, helping_node->table_size, helping_node->level+1);
						helping_node->next[counter]->possibility = 0.9;
						helping_node->next[counter]->table[i][j] = 2;
						push(stack, helping_node->next[counter], &top, stack_space);
						counter++;
						helping_node->next[counter] = get_node(helping_node->table, helping_node->table_size, helping_node->level+1);
						helping_node->next[counter]->possibility = 0.1;
						helping_node->next[counter]->table[i][j] = 4;
						push(stack, helping_node->next[counter], &top, stack_space);
						counter++;
					}
				}
			}
			if (counter == 0)
				helping_node->weight = 0;
		}
		else
			helping_node->weight = approximate_position(helping_node->table, helping_node->table_size);
	}
	free(M);
}

int get_hint(matrix table)
{
	int move = 0;
	int stack_space = STACK_SPACE;
	T_node **stack = malloc(STACK_SPACE*sizeof(T_node*));
	if (stack == NULL)
		exit(7);
	T_node *root = get_node(table.set, table.size, 0);
	root->possibility = 1;

	make_tree_iterative(root, &stack, &stack_space);
	expectimax_search(root, &stack, &stack_space);

	while ((root->next[move] == NULL || root->next[move]->weight != root->weight) && move < 4)
		move++;
	free_tree(root, &stack, &stack_space);
	free(stack);
	return move;
}

/*  TO-DO optimizacija poteza
		  optimizacija procjenjivanja poteza
		  dinamicko provjeravanje, hashing
		 							      */