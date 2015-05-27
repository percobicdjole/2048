#include <stdlib.h>
#include "logic.h"
#include "ai.h"

void expectimax_search(T_node *root, T_node ***stack, int *stack_space)
{
	int top = 0, top2 = 1, begin = 0, end = 1;
	int number_of_elems = 2 * (root->table_size*root->table_size - 1);
	int  i, counter;
	float max;

	push(stack, root, &top, stack_space);
	while (root->level != MAX_DEPTH - 1)
	{
		if (top2 == begin)
		{
			begin = end;
			top2 = top;
			end = top;
		}
		root = pop(*stack, &top2);
		if (root->level % 2 == 1 && root->level != MAX_DEPTH - 1)
		{
			i = 0;
			while (1)
			{
				while (i < number_of_elems && root->next[i] != NULL && root->next[i]->weight != -1)
					i++;
				if (i == number_of_elems || root->next[i] == NULL)
					break;
				push(stack, root->next[i], &top, stack_space);
				i++;
			}
		}
		else if (root->level != MAX_DEPTH - 1)
		{
			i = 0;
			while (1)
			{
				while (i < 4 && ((root->next[i] == NULL) ? 1 : ((root->next[i]->weight != -1) ? 1 : 0)))
					i++;      //  ako nije jednak jedinici njegovi sinovi su vec popunjeni
				if (i == 4)
					break;
				push(stack, root->next[i], &top, stack_space);
				i++;
			}
		}
		if (top == 1 && end == 1)
			break; // kada nema vise poteza
	}
	while (top != 0)
	{
		root = pop(*stack, &top);
		if (root->weight == -1)
		{
			if (root->level % 2 == 1)
			{
				counter = 0;
				(root->weight)++; // zato sto je -1
				for (i = 0; i < number_of_elems; i++)
				{
					if (root->next[i] == NULL)
						break;
					root->next[i]->weight *= root->next[i]->possibility;
					root->weight += root->next[i]->weight;
					counter++;
				}
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
					root->weight = max;
				}
			}
		}
	}
}

float approximate_position(int **table, int table_size)
{
	int i, j, empty_spaces_counter = 0;
	int max_number = 1;
	int x, y;
	float score = 0, check_moves;

	for (i = 0; i < table_size; i++)
	{
		for (j = 0; j < table_size; j++)
		{
			if (table[i][j] > max_number)
			{
				max_number = table[i][j]; 
				x = i;
				y = j;
			}
			else if (table[i][j] == max_number && !((x == 0 && (y == 0 || y == table_size-1)) // provjeri
						|| (y == 0 && x == table_size-1)))
			{
				if ((i == 0 && (j == 0 || j == table_size-1)) || (j == 0 && i == table_size-1))
				{
					x = i;
					y = j;
				}
			}
			if (table[i][j] == 0)
				empty_spaces_counter++;
		}
	}
	if (x == 0 && y == 0)
	{
		score+= max_number*MAX_IN_CORNER;
		// monotonost
		for (i = 1; i < table_size; i++)
		{
			if (table[0][i] <= table[0][i-1])
				score+= table[0][i]*MONOTONICITY;
			else
				break;
		}
		for (j = 1; j < table_size; j++)
		{
			if (table[j][0] <= table[j-1][0])
				score+= table[j][0]*MONOTONICITY;
			else
				break;
		}
		// monotonost u sredini
		int rows = 1, columns = 1;
		while (rows < i)
		{
			while (columns < j)
			{
				if (table[rows-1] >= table[rows] && table[columns-1] >= table[columns])
				{
					score+= table[rows][columns]*MONOTONICITY;
					columns++;
				}
				else
				{
					j = columns;
					break;
				}
			}
			columns = 1;
			rows++;
		}
		// provjeri broj poteza
		check_moves = score;
		number_of_moves_horizontally(table, table_size, &score);
		number_of_moves_vertically(table, table_size, &score);
		if (check_moves == score)
			score+= max_number*NO_MOVES_PENALTY;
		// ostalo
		score+= empty_spaces_counter*EMPTY_SPACES;
		score+= max_number;
	}
	else if (x == 0 && y == table_size-1)
	{
		score+= max_number*MAX_IN_CORNER;
		// monotonost
		for (i = table_size-1; i > 0; i--)
		{
			if (table[0][i-1] <= table[0][i])
				score+= table[0][i-1]*MONOTONICITY;
			else
				break;
		}
		for (j = 1; j < table_size; j++)
		{
			if (table[j][table_size-1] <= table[j-1][table_size-1])
				score+= table[j][table_size-1]*MONOTONICITY;
			else
				break;
		}
		//provjeri monotonost u sredini
		int rows = table_size-2, columns = 1;
		while (rows > i)
		{
			while (columns < j)
			{
				if (table[rows-1] <= table[rows] && table[columns-1] >= table[columns])
				{
					score+= table[rows-1][columns]*MONOTONICITY;
					columns++;
				}
				else
				{
					j = columns;
					break;
				}
			}
			columns = 1;
			rows--;
		}
		// provjeri broj poteza
		check_moves = score;
		number_of_moves_horizontally(table, table_size, &score);
		number_of_moves_vertically(table, table_size, &score);
		if (check_moves == score)
			score+= max_number*NO_MOVES_PENALTY;
		// ostalo
		score+= empty_spaces_counter*EMPTY_SPACES;
		score+= max_number;	
	}
	else if (x == table_size-1 && y == 0)
	{
		score+= max_number*MAX_IN_CORNER;
		// monotonost
		for (i = 1; i < table_size; i++)
		{
			if (table[table_size-1][i] <= table[table_size-1][i-1])
				score+= table[table_size-1][i]*MONOTONICITY;
			else
				break;
		}
		for (j = table_size-1; j > 0; j--)
		{
			if (table[j-1][0] <= table[j][0])
				score+= table[j-1][0]*MONOTONICITY;
			else
				break;
		}
		//provjeri monotonost u sredini
		int rows = 1, columns = table_size-2;
		while (rows < i)
		{
			while (columns > j)
			{
				if (table[rows-1] >= table[rows] && table[columns-1] <= table[columns])
				{
					score+= table[rows][columns-1]*MONOTONICITY;
					columns--;
				}
				else
				{
					j = columns;
					break;
				}
			}
			columns = table_size-2;
			rows++;
		}
		// provjeri broj poteza
		check_moves = score;
		number_of_moves_horizontally(table, table_size, &score);
		number_of_moves_vertically(table, table_size, &score);
		if (check_moves == score)
			score+= max_number*NO_MOVES_PENALTY;
		// ostalo
		score+= empty_spaces_counter*EMPTY_SPACES;
		score+= max_number;
	}
	else if (x == table_size-1 && y == table_size-1)
	{
		score+= max_number*MAX_IN_CORNER;
		// monotonost
		for (i = table_size-1; i > 0; i--)
		{
			if (table[table_size-1][i-1] <= table[table_size-1][i])
				score+= table[table_size-1][i-1]*MONOTONICITY;
			else
				break;
		}
		for (j = table_size-1; j > 0; j--)
		{
			if (table[j-1][table_size-1] <= table[j][table_size-1])
				score+= table[j-1][table_size-1]*MONOTONICITY;
			else
				break;
		}
		//provjeri monotonost u sredini
		int rows = table_size-2, columns = table_size-2;
		while (rows > i)
		{
			while (columns > j)
			{
				if (table[rows-1] <= table[rows] && table[columns-1] <= table[columns])
				{
					score+= table[rows-1][columns-1]*MONOTONICITY;
					columns--;
				}
				else
				{
					j = columns;
					break;
				}
			}
			columns = table_size-2;
			rows--;
		}
		// provjeri broj poteza
		check_moves = score;
		number_of_moves_horizontally(table, table_size, &score);
		number_of_moves_vertically(table, table_size, &score);
		if (check_moves == score)
			score+= max_number*NO_MOVES_PENALTY;
		// ostalo
		score+= empty_spaces_counter*EMPTY_SPACES;
		score+= max_number;
	}
	else
	{
		score+= max_number*MAX_NOT_IN_CORNER_PENALTY;
		// provjeri broj poteza
		check_moves = score;
		number_of_moves_horizontally(table, table_size, &score);
		number_of_moves_vertically(table, table_size, &score);
		if (check_moves == score)
			score+= max_number*NO_MOVES_PENALTY;
		// ostalo
		score+= empty_spaces_counter*EMPTY_SPACES;
		score+= max_number;
	}
	if (score < 0)
		return 0;
	else 
		return score;
}

void number_of_moves_horizontally(int **table, int table_size, float *score)
{
	int i, j;
	int flag = 0;
	for (i = 0; i < table_size; i++)
	{
		for (j = 0; j < table_size-1; j++)
		{
			if (table[i][j] == table[i][j+1])
			{
				if (flag == 0)
				{
					*score+= table[i][j]*MOVES;
					flag = 1;
				}
				else
					flag = 0;
			}
		}
		flag = 0;
	}
}

void number_of_moves_vertically(int **table, int table_size, float *score)
{
	int i, j;
	int flag = 0;
	for (j = 0; j < table_size; j++)
	{
		for (i = 0; i < table_size-1; i++)
		{
			if (table[i][j] == table[i+1][j])
			{
				if (flag == 0)
				{
					*score+= table[i][j]*MOVES;
					flag = 1;
				}
				else
					flag = 0;
			}
		}
		flag = 0;
	}
}