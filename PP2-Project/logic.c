#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Logic.h"

//Postavljanje SEED-a
void setSeed()
{
	srand(time(NULL));
}


//Random broj u opsegu
int randomInt(int low,int high)
{
	return low + rand() / (RAND_MAX / (high - low + 1) + 1);
}

//Kreiranje nove matrice
matrix newMatrix(int size)
{
	matrix N;
	int i;
	int **M = malloc(size*sizeof(int*));
	checkMemError(M);
	for (i = 0; i < size; i++)
	{
		M[i] = calloc(size, sizeof(int));
		checkMemError(M[i]);
	}
	N.set = M;
	N.size = size;
	spawnNumber(&N);
	spawnNumber(&N);
	return N;
}

//Ubacuje broj 2 ili 4 na random izabrano prazno mesto
void spawnNumber(matrix *M)
{
	int i, j, array_index = -1, matrix_index = 0, r;
	int free_tiles[25];
	for (i = 0; i < M->size; i++)
		for (j = 0; j < M->size; j++)
		{
			if (M->set[i][j] == 0)
			{
				array_index++;
				free_tiles[array_index] = matrix_index;
			}
			matrix_index++;
		}

	array_index = randomInt(0, array_index);
	matrix_index = free_tiles[array_index];
	i = matrix_index / M->size;
	j = matrix_index % M->size;
	r = randomInt(1, 10);
	if (r > 8)
		M->set[i][j] = 4;
	else
		M->set[i][j] = 2;
}


//Pomera jedan red za jedan korak i vraca broj promena
int moveRowStep(matrix *M, int row, int direction,int *last_merged,unsigned int *score)
{
	int j, changes = 0;
	if (direction == RIGHT)
	{
		for (j = M->size - 2; j >= 0; j--)
		{
			if (M->set[row][j] != 0)
			{
				if (M->set[row][j + 1] == 0)
				{
					M->set[row][j + 1] = M->set[row][j];
					M->set[row][j] = 0;
					changes++;
				}
				else if (M->set[row][j + 1] == M->set[row][j] && M->set[row][j] != last_merged[row])
				{
					(M->set[row][j + 1]) <<= 1;
					if (score)
						*score += M->set[row][j + 1];
					M->set[row][j] = 0;
					last_merged[row] = M->set[row][j + 1];
					changes++;
				}
			}
		}
	}
	else if (direction == LEFT)
	{
		for (j = 1; j < M->size; j++)
		{
			if (M->set[row][j] != 0)
			{
				if (M->set[row][j - 1] == 0)
				{
					M->set[row][j - 1] = M->set[row][j];
					M->set[row][j] = 0;
					changes++;
				}
				else if (M->set[row][j - 1] == M->set[row][j] && M->set[row][j]!=last_merged[row])
				{
					(M->set[row][j - 1]) <<= 1;
					if (score)
						*score += M->set[row][j - 1];
					M->set[row][j] = 0;
					last_merged[row] = M->set[row][j - 1];
					changes++;
				}
			}
		}
	}
	return changes;
}


//Pomera jednu kolonu za jedan korak i vraca broj promena
int moveColumnStep(matrix *M, int column, int direction,int *last_merged,unsigned int *score)
{
	int i, changes = 0;
	if (direction == UP)
	{
		for (i = 1; i < M->size; i++)
		{
			if (M->set[i][column] != 0)
			{
				if (M->set[i - 1][column] == 0)
				{
					M->set[i - 1][column] = M->set[i][column];
					M->set[i][column] = 0;
					changes++;
				}
				else if (M->set[i - 1][column] == M->set[i][column] && M->set[i][column] != last_merged[column])
				{
					(M->set[i - 1][column]) <<= 1;
					if (score)
						*score += M->set[i - 1][column];
					M->set[i][column] = 0;
					last_merged[column] = M->set[i - 1][column];
					changes++;
				}
			}
		}
	}
	else if (direction == DOWN)
	{
		for (i = M->size - 2; i >= 0; i--)
		{
			if (M->set[i][column] != 0)
			{
				if (M->set[i + 1][column] == 0)
				{
					M->set[i + 1][column] = M->set[i][column];
					M->set[i][column] = 0;
					changes++;
				}
				else if (M->set[i + 1][column] == M->set[i][column] && M->set[i][column] != last_merged[column])
				{
					(M->set[i + 1][column]) <<= 1;
					if (score)
						*score += M->set[i + 1][column];
					M->set[i][column] = 0;
					last_merged[column] = M->set[i + 1][column];
					changes++;
				}
			}
		}
	}
	return changes;
}


int moveStep(matrix *M, int direction,int *last_merged,unsigned int *score)
{
	int i, j, changes = 0;
	if (direction == RIGHT || direction == LEFT)
	{
		for (i = 0; i < M->size; i++)
		{
			changes += moveRowStep(M, i, direction,last_merged,score);
		}
	}

	if (direction == UP || direction == DOWN)
	{
		for (j= 0; j < M->size; j++)
		{
			changes += moveColumnStep(M, j, direction,last_merged,score);
		}
	}
	return changes;
}


int snap(unsigned int **table, int table_size, int direction, matrix *M)
{
	M->set = table;
	M->size = table_size;
	int changes, moved, last_merged[5] = { 0 };
	changes = moved = moveStep(M, direction, last_merged,NULL);
	while (changes)
	{
		changes = moveStep(M, direction, last_merged,NULL);
	}
	if(moved)
		return 1;
	else
		return 0;
}

int **copySet(int **source, char set_size)
{
	int i, **dest_set;
	dest_set = malloc(set_size*sizeof(int*));
	for (i = 0; i < set_size; i++)
	{
		dest_set[i] = malloc(set_size*sizeof(int));
		memcpy(dest_set[i], source[i], set_size*sizeof(int));
	}
	return dest_set;
}

void copyMatrix(matrix *dest, matrix M)
{
	dest->set = copySet(M.set,M.size);
	dest->size = M.size;
}

void freeSet(int **set, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		free(set[i]);
	}
}

void freeMatrix(matrix *M)
{
	freeSet(M->set, M->size);
	free(M->set);
}

state getState(matrix M, unsigned int score)
{
	state S;
	S.score = score;
	S.set = copySet(M.set, M.size);
	return S;
}

void freeState(state *S, unsigned int tile_size)
{
	S->score = 0;
	freeSet(S->set, tile_size);
	free(S->set);
	S->set = NULL;
}

history newHistory(short undo_depth, matrix *M)
{
	history H;
	int i;
	H.latest = 0;
	H.mat = M;
	H.depth = undo_depth;
	H.stack = malloc(undo_depth*sizeof(state));
	for (i = 0; i < H.depth; i++)
	{
		H.stack[i].set = NULL;
	}
	return H;
}

void clearHistory(history *H)
{
	int i;
	for (i = 0; i < H->depth; i++)
	{
		if (H->stack[i].set)
			freeState(&(H->stack[i]), H->mat->size);
	}
}

void destroyHistory(history *H)
{
	clearHistory(H);
	free(H->stack);
}

void pushHistory(history *H,state S)
{
	if (H->stack[H->latest].set)
	{
		freeSet(H->stack[H->latest].set, H->mat->size);
		free(H->stack[H->latest].set);
	}
	H->stack[H->latest].set = copySet(S.set, H->mat->size);
	H->stack[H->latest].score = S.score;
	H->latest = (H->latest + 1) % H->depth;
}

void popHistory(history *H,unsigned int *score)
{
	(H->latest)--;
	if (H->latest < 0)
		H->latest = H->depth - 1;
	if (H->stack[H->latest].set)
	{
		freeSet(H->mat->set, H->mat->size);
		H->mat->set = copySet(H->stack[H->latest].set, H->mat->size);
		*score = H->stack[H->latest].score;
		freeState(&(H->stack[H->latest]), H->mat->size);
	}
}