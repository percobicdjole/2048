#include <stdlib.h>
#include "logic.h"
#include "ai.h"
#include "math.h"

T_node* get_node(int **table, int table_size, int level)
{
	T_node *node = malloc(sizeof(T_node));

	if (node != NULL)
	{
		int i, number_of_nodes;

		number_of_nodes = table_size * table_size - 2;
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
		for (i = 0; i < number_of_nodes; i++)
			node->next[i] = NULL;
		return node;
	}
	else
		exit(1);
}

void free_stable(T_node *root)
{
	int top = 0, i;
	T_node **stack = malloc(10000*sizeof(T_node*)), *old;
	if (stack == NULL)
		exit(5);
	int number_of_elems = root->table_size*root->table_size;
	push(stack, root, &top);
	while (top != 0)
	{
		root = pop(stack, &top);
		while (root->next[0] != NULL)
		{
			i = 1;
			if (root->level % 2 == 1)
			{
				while (i < 4)
				{
					if (root->next[i] != NULL)
						push(stack, root->next[i], &top);
					i++;
				}
			}
			else
			{
				while (i < number_of_elems && root->next[i] != NULL)
				{
					push(stack, root->next[i], &top);
					i++;
				}
			}
			old = root;
			root = root->next[0];
			free(old);
		}
	}
}

void push(T_node **stack, T_node *elem, int *top)
{
	if ((*top + 1) % 10000 == 0)
	{
		T_node **tmp;
		tmp = realloc(stack, *top*2*sizeof(T_node*));
		if (tmp == NULL)
			exit(2);
		stack = tmp;
	}
	stack[*top] = elem;
	(*top)++;
}

T_node* pop(T_node **stack, int *top)
{
	(*top)--;
	return (stack[*top]);
}

void make_tree_normal_move(T_node *root, int level)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		root->next[i] = get_node(root->table, root->table_size, level);
		if (snap(root->next[i]->table, root->next[i]->table_size, i) == 0)
		{
		 	free(root->next[i]);
		 	root->next[i] = NULL;
		}
		else
		{
			root->next[i]->possibility = 1;
			make_tree_random_move(root->next[i], level + 1);
		}
	}
}

void make_tree_random_move(T_node *root, int level)
{
	if (level < MAX_DEPTH)
	{
		int i , j, counter = 0;
		int number_of_elems = root->table_size*root->table_size; 

		for (i = 0; i < number_of_elems; i++)
		{
			for (j = 0; j < number_of_elems; j++)
			{
				if (root->table[i][j] == 0)
				{
					root->next[counter] = get_node(root->table, root->table_size, level);
					root->next[counter]->possibility = 0.9;
					root->next[counter]->table[i][j] = 2;
					make_tree_normal_move(root->next[counter], level + 1);
					counter++;
					root->next[counter] = get_node(root->table, root->table_size, level);
					root->next[counter]->possibility = 0.1;
					root->next[counter]->table[i][j] = 4;
					make_tree_normal_move(root->next[counter], level + 1);
					counter++;
				}
			}
		}
	}
}

int get_hint(matrix table)
{
	int move = 0;

	T_node *root = get_node(table.set, table.size, 0);
	root->possibility = 1;

	make_tree_normal_move(root, 1);
	expectimax_search(root);

	while (root->next[move]->weight != root->weight)
		move++;
	free_stable(root);
	return move;
}

/*  TO-DO pravljene stabla iterativno, 
		  optimizacija poteza
		  optimizacija procjenjivanja poteza
		  dinamicko provjeravanje, hashing
		 							      */