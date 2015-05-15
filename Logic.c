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


//Ubacuje broja 2 na random izabrano prazno mesto
void spawnNumber(matrix *M)
{
	
	int16 i, j, array_index = -1, matrix_index = 0;
	int16 *free_tiles = malloc((M->size*M->size - M->count)*sizeof(int16));
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
	free(free_tiles);
	i = matrix_index / M->size;
	j = matrix_index % M->size;
	M->set[i][j] = 2;
	M->count++;
}

//Kreiranje nove matrice
matrix newMatrix(int16 size)
{
	matrix N;
	int16 i;
	int16 **M = malloc(size*sizeof(int16*));
	for (i = 0; i < size; i++)
	{
		M[i] = calloc(size, sizeof(int16));
	}
	N.count = 0;
	N.set = M;
	N.size = size;
	spawnNumber(&N);
	spawnNumber(&N);
	return N;
}


//Pomera jedan red za jedno mesto
int16 moveRowStep(matrix M, int16 row, int16 direction)
{
	int16 j, size; 
	size = M.size;
	if (direction == RIGHT)
	{
		for (j = size - 1; j > 0; j--)
		{
			if ((M.set[row][j] == 0 || M.set[row][j] == M.set[row][j-1]))
			{
				M.set[row][j] += M.set[row][j - 1];
				M.set[row][j - 1] = 0;
			}
		}
	}
	else if (direction == LEFT)
	{

		for (j = 0; j < size -1; j++)
		{
			if ((M.set[row][j] == 0 || M.set[row][j] == M.set[row][j + 1]))
			{
				M.set[row][j] += M.set[row][j + 1];
				M.set[row][j + 1] = 0;
			}
		}
	}
}


//Pomera jenu kolonu za jedno mesto
int16 moveColumnStep(matrix M, int16 column, int16 direction)
{
	int16 i, size;
	size = M.size;
	if (direction == DOWN)
	{
		for (i = size - 1; i > 0; i--)
		{
			if ((M.set[i][column] == 0 || M.set[i][column] == M.set[i-1][column]))
			{
				M.set[i][column] += M.set[i-1][column];
				M.set[i-1][column] = 0;
			}
		}
	}
	else if (direction == UP)
	{
		for (i = 0; i < size -1; i++)
		{
			if ((M.set[i][column] == 0 || M.set[i][column] == M.set[i + 1][column]))
			{
				M.set[i][column] += M.set[i + 1][column];
				M.set[i + 1][column] = 0;
			}
		}
	}
}


//Pomera matricu postepeno
void moveStep(matrix M, int16 direction)
{
	int16 i,j;
	if (direction == RIGHT || direction == LEFT)
	{
		for (i = 0; i < M.size; i++)
		{
			moveRowStep(M, i, direction);
		}
	}

	if (direction == UP || direction == DOWN)
	{
		for (j= 0; j < M.size; j++)
		{
			moveColumnStep(M, j, direction);
		}
	}
}


//Pomera matricu u jednom potezu
void snap(matrix M, int16 direction)
{
	int16 i;
	for (i = 0; i < M.size; i++)
	{
		moveStep(M,direction);
	}
}


//Stampa matricu
void printMatrix(matrix M)
{
	int16 i, j;
	for (i = 0; i < M.size; i++)
	{
		for (j = 0; j < M.size; j++)
			printf("%d ", M.set[i][j]);
		printf("\n");
	}
	printf("\n");
}


//Stampa red
void printRow(int16 *R, int16 size)
{
	int16 i, j;
	for (i = 0; i < size; i++)
	{
		printf("%d ", R[i]);
	}
	printf("\n");
}


void main()
{
	/*Demonstracija pomeraja iz jednog poteza:
	Gore, dole, levo desno.
	*/
	int16 i;
	setSeed();
	matrix M = newMatrix(4);
	printMatrix(M);
	snap(M, UP);
	printMatrix(M);
	snap(M, DOWN);
	printMatrix(M);
	snap(M, LEFT);
	printMatrix(M);
	snap(M, RIGHT);
	printMatrix(M);
	system("PAUSE");
}
