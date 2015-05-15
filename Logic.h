#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Zamena za argumente funkcija pomeranja
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

//Konstante
#define MAXINT16 65535
#define MAXINT32 4294967295

//Celi neoznaceni do 65,535
typedef unsigned short int  int16;

//Celi neoznaceni do 4,294,967,295
typedef unsigned int  int32;

//Matrica
typedef struct matrix
{
	int16 **set;//novo ime
	unsigned int size, count;
}matrix;


//Funkcije za testiranje
void printRow(int16 *R, int16 size);
void printMatrix(matrix M);


//Funkcije koje se pozivaju na poèetku igre
void setSeed();
matrix newMatrix(int16 size);


//Pomocne funkcije
void spawnNumber(matrix *M);
int randomInt(int low, int high);


//Funkcije za pomeranje
void moveStep(matrix M, int16 direction);
void snap(matrix M, int16 direction);