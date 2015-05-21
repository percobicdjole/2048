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
	short **set;
	char size;
}matrix;

//Undo sistem
typedef struct history
{
	short **Stack[20];
	short sp;
}history;


//Funkcije za testiranje
void printRow(short *R, short size);
void printMatrix(matrix M);


//Postavljanje seed-a
void setSeed();

//Nova matrica
matrix newMatrix(short size);


//Ubacuje broj 2 ili 4
void spawnNumber(matrix *M);

//Random integer u granicama, ukljucuje i gornju granicu
int randomInt(int low, int high);


//Pomera za jedan korak i vraca broj promena
short moveStep(matrix *M, short direction);

//Igra ceo potez i postavlja novi broj
void moveMatrix(matrix *M, short direction);

//Igra potez za hint
short snap(matrix *M, short direction);