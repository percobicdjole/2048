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


//Funkcije za testiranje
void printRow(int *R, int size);
void printMatrix(matrix M);


//Postavljanje seed-a
void setSeed();

//Nova matrica
matrix newMatrix(int size);

//Unistavanje matrice
void freeMatrix(matrix *M);

//Kopiranje matrice
void copyMatrix(matrix *dest, matrix *source);

//Ubacuje broj 2 ili 4
void spawnNumber(matrix *M);

//Random integer u granicama, ukljucuje i gornju granicu
int randomInt(int low, int high);


//Pomera za jedan korak i vraca broj promena
int moveStep(matrix *M, int direction, int *last_merged, unsigned int *score);


//Igra potez za hint
int snap(unsigned int **table, int table_size, int direction);