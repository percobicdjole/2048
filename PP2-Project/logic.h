#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Zamena za argumente funkcija pomeranja
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

//Potrebne konstante
#define TRUE 1
#define FALSE 0

//Matrica
typedef struct matrix
{
	short **set;//novo ime
	char size;
}matrix;


//Funkcije za testiranje
void printRow(short *R, short size);
void printMatrix(matrix M);


//Funkcije koje se pozivaju na poèetku igre
void setSeed();
matrix newMatrix(short size);


//Pomocne funkcije
void spawnNumber(matrix *M);
int randomInt(int low, int high);


//Funkcije za pomeranje
short moveStep(matrix M, short direction);
void moveMatrix(matrix M, short direction);