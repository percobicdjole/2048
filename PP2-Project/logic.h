#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
	int **set;
	char size;
}matrix;


//Stanje igre
typedef struct state
{
	int **set;
	unsigned int score;
}state;


//Istorija igre
typedef struct history
{
	state *stack;
	matrix *mat;
	int depth,latest;
}history;


//Postavljanje seed-a
void setSeed();

//Nova matrica
matrix newMatrix(int size);

//Unistavanje matrice
void freeMatrix(matrix *M);


//Ubacuje broj 2 ili 4
void spawnNumber(matrix *M);

//Random integer u granicama, ukljucuje i gornju granicu
int randomInt(int low, int high);


//Pomera za jedan korak i vraca broj promena
int moveStep(matrix *M, int direction, int *last_merged, unsigned int *score);


//Igra potez za hint
int snap(unsigned int **table, int table_size, int direction, matrix *M);

//Inicijalizacija istorije igre
history newHistory(short undo_depth, matrix *M);

//Brisanje istorije
void clearHistory(history *H);

//Unistavanje strukture istorije
void destroyHistory(history *H);

//Brise poslednji podez iz istorije i vraca stanje matrice
void popHistory(history *H, unsigned int *score);

//Pamti potez u istoriju
void pushHistory(history *H, state S);

//Oslobadjanje cele matrice
void freeMatrix(matrix *M);

//Oslobadjanje seta
void freeSet(int **set, int size);

//Kopiranje matrice
void copyMatrix(matrix *dest, matrix M);

//Kopira set i vraca pokazivac novog
int **copySet(matrix M);

//Dohvata trenutno stanje i vraca novu strukturu
state getState(matrix M, unsigned int score);

//Oslobadja pokazivace u stanju
void freeState(state *S, unsigned int tile_size);

//Proverava da li je kraj igre
int checkGameOver(matrix M);

//Provera da li je matrica puna
int checkFull(matrix M);