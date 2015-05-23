#include <stdlib.h>
#include "ai.h"
#include "logic.h"

int get_hint(matrix table)
{
	int move = 0;

	T_node *root = get_node(table.set, table.size, 0);
	root->possibility = 1;

	make_expectimax_normal_move(root, 1);
	expectimax_search(root);

	while (root->next[move]->weight != root->weight)
		move++;
	free_stable(root);
	return move;
}

void make_tree_normal_move(T_node *root, int level)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		root->next[i] = get_node(root->table, root->table_size, level);
		if (snap(root->next[i]->table, i) == 0) // mora da se prosledi matrica
		{
		 	free(root->next[i]);
		 	root->next[i] = NULL;
		}
		else
		{
			root->next[i]->possibility = 1;
			make_expectimax_random_move(root->next[i], level + 1);
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
					make_expectimax_normal_move(root->next[counter], level + 1);
					counter++;
					root->next[counter] = get_node(root->table, root->table_size, level);
					root->next[counter]->possibility = 0.1;
					root->next[counter]->table[i][j] = 4;
					make_expectimax_normal_move(root->next[counter], level + 1);
					counter++;
				}
			}
		}
	}
}

void expectimax_search(T_node *root)
{
	int top = 0;
	T_node **stack = malloc(10000*sizeof(T_node*));
	if (stack == NULL)
		exit(5);
	int number_of_elems = root->table_size*root->table_size;
	int  i, counter;
	float max;

	while (root != NULL)
	{
		push(stack, root, &top);
		root = root->next[0];
	}
	while (top != 0)
	{
		root = pop(stack, &top);
		if (root->weight == -1)
		{ 
			if (root->level == MAX_DEPTH)
				root->weight = approximate_position(root->table, root->table_size);
			else if (root->level % 2 == 1)
			{
				counter = 0;
				root->weight++; // zato sto je -1
				for (i = 0; i < number_of_elems, root->next[i] != NULL; i++)
				{
					root->next[i]->weight*=root->next[i]->possibility;
					root->weight+= root->next[i]->weight;
					counter++;
				}
				if (i == 0)
					root->weight = approximate_position(root->table, root->table_size);
				root->weight /= counter;
			}
			else
			{
				max = 0;
				counter = 0;
				for (i = 0; i < 4; i++)
				{
					if (root->next[i] == NULL)
						counter++;
					else if (root->next[i]->weight > max)
						max = root->next[i]->weight;
				}
				if (counter == 4)
					root->weight = approximate_position(root->table, root->table_size);
			}
			push(stack, root, &top);
			if (root->level % 2 == 1)
			{
				i = 0;
				while (root->next[i] == NULL && i < number_of_elems-1)
					i++;
				root = root->next[i];
			}
			else
			{
				while (root->next[i] == NULL && i < 3)
					i++;
				root = root->next[i];
			}
			while (root != NULL)
			{
				push(stack, root, &top);
				root = root->next[0];
			}
		}
	}
	free(stack);
}

/*  TO-DO pravljene stabla iterativno, 
		  adaptive/iterative deepening
		  optimizacija poteza
		  optimizacija procjenjivanja poteza
		  dinamicko provjeravanje, hashing
		 							      */