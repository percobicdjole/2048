#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
matrix newMatrix(short size)
{
	matrix N;
	short i;
	short **M = malloc(size*sizeof(short*));
	for (i = 0; i < size; i++)
	{
		M[i] = calloc(size, sizeof(short));
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
	short i, j, array_index = -1, matrix_index = 0, r;
	short free_tiles[25];
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
	r = randomInt(1, 20);
	if (r > 17)
		M->set[i][j] = 4;
	else
		M->set[i][j] = 2;
}


//Pomera jedan red za jedan korak i vraca broj promena
short moveRowStep(matrix *M, short row, short direction,short *last_merged)
{
	short j, changes = 0;
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
short moveColumnStep(matrix *M, short column, short direction,short *last_merged)
{
	short i, changes = 0;
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
					M->set[i][column] = 0;
					last_merged[column] = M->set[i + 1][column];
					changes++;
				}
			}
		}
	}
	return changes;
}



short moveStep(matrix *M, short direction,short *last_merged)
{
	short i, j, changes = 0;
	if (direction == RIGHT || direction == LEFT)
	{
		for (i = 0; i < M->size; i++)
		{
			changes += moveRowStep(M, i, direction,last_merged);
		}
	}

	if (direction == UP || direction == DOWN)
	{
		for (j= 0; j < M->size; j++)
		{
			changes += moveColumnStep(M, j, direction,last_merged);
		}
	}
	return changes;
}


//Ova funkcija ne treba da se koristi!! Treba slicna na se napravi u main()
void moveMatrix(matrix *M, short direction)
{
	short changes, moved, last_merged[5] = { 0 };
	changes = moved = moveStep(M, direction,last_merged);
	while (changes)
	{
		changes = moveStep(M, direction,last_merged);
	}
	if (moved)
		spawnNumber(M);
}

short snap(unsigned int **table, int table_size, short direction)
{
	matrix *M = malloc(sizeof(matrix));
	M->set = table;
	M->size = table_size;
	short changes, moved, last_merged[5] = { 0 };
	changes = moved = moveStep(M, direction, last_merged);
	while (changes)
	{
		changes = moveStep(M, direction, last_merged);
	}
	if(moved)
		return 1;
	else
		return 0;
}


void printMatrix(matrix M)
{
	short i, j;
	for (i = 0; i < M.size; i++)
	{
		for (j = 0; j < M.size; j++)
			printf("%d ", M.set[i][j]);
		printf("\n");
	}
	printf("\n");
}


void printRow(short *R, short size)
{
	short i, j;
	for (i = 0; i < size; i++)
	{
		printf("%d ", R[i]);
	}
	printf("\n");
}
