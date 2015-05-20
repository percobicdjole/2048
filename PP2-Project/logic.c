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

//Ubacuje broja 2 na random izabrano prazno mesto
void spawnNumber(matrix *M)
{
	//Problem sa dimnamickom memorijom
	short i, j, array_index = -1, matrix_index = 0;
	short *free_tiles[25];
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
	M->set[i][j] = 2;
}


//Pomera jedan red za jedno mesto
short moveRowStep(matrix *M, short row, short direction)
{
	short j, changes = 0;
	if (direction == RIGHT)
	{
		for (j = M->size - 2; j >= 0; j--)
		{
			if (M->set[row][j] != 0)
			{
				if (M->set[row][j + 1] == 0 || M->set[row][j + 1] == M->set[row][j])
				{
					M->set[row][j + 1] += M->set[row][j];
					M->set[row][j] = 0;
					//M->score += M->set[row][j + 1];
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
				if (M->set[row][j - 1] == 0 || M->set[row][j - 1] == M->set[row][j])
				{
					M->set[row][j - 1] += M->set[row][j];
					M->set[row][j] = 0;
					//M->score += M->set[row][j - 1];
					changes++;
				}
			}
		}
	}
	return changes;
}


//Pomera jednu kolonu za jedno mesto
short moveColumnStep(matrix *M, short column, short direction)
{
	short i, changes = 0;
	if (direction == UP)
	{
		for (i = 1; i < M->size; i++)
		{
			if (M->set[i][column] != 0)
			{
				if (M->set[i - 1][column] == 0 || M->set[i - 1][column] == M->set[i][column])
				{
					M->set[i - 1][column] += M->set[i][column];
					M->set[i][column] = 0;
					//M->score += M->set[i - 1][column];
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
				if (M->set[i + 1][column] == 0 || M->set[i + 1][column] == M->set[i][column])
				{
					M->set[i + 1][column] += M->set[i][column];
					M->set[i][column] = 0;
					//M->score += M->set[i + 1][column];
					changes++;
				}
			}
		}
	}
	return changes;
}


//Pomera matricu postepeno i vraca broj promena
short moveStep(matrix M, short direction)
{
	short i, j, changes = 0;
	if (direction == RIGHT || direction == LEFT)
	{
		for (i = 0; i < M.size; i++)
		{
			changes += moveRowStep(&M, i, direction);
		}
	}

	if (direction == UP || direction == DOWN)
	{
		for (j= 0; j < M.size; j++)
		{
			changes += moveColumnStep(&M, j, direction);
		}
	}
	return changes;
}


//Pomera matricu u jednom potezu
void moveMatrix(matrix M, short direction)
{
	short changes;
	//Detektuje promene u matrici
	changes = moveStep(M, direction);
	while (changes)
	{
		changes = moveStep(M, direction);
	}
	if (changes)
		spawnNumber(&M);
}


//Stampa matricu
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


//Stampa red
void printRow(short *R, short size)
{
	short i, j;
	for (i = 0; i < size; i++)
	{
		printf("%d ", R[i]);
	}
	printf("\n");
}
