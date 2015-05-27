//Zamena za argumente funkcija pomeranja
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

//Potrebne konstante
#define TRUE 1
#define FALSE 0
#define UNDO_DEPTH 20

//Matrica
typedef struct matrix
{
	int **set;
	char size;
}matrix;

//Istorija igre
typedef struct history
{
	int **stack[UNDO_DEPTH];
	char latest, oldest;
}history;


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
int snap(unsigned int **table, int table_size, int direction, matrix *M);

//Inicijalizacija istorije igre
history newHistory();

//Brise poslednji podez iz istorije i vraca stanje matrice
int **Pop(history *H, int set_size);

//Pamti potez u istoriju
void Push(history *H, matrix M);

//Oslobadjanje cele matrice
void freeMatrix(matrix *M);

//Oslobadjanje seta
void freeSet(int **set, int size);

//Kopiranje matrice
void copyMatrix(matrix *dest, matrix M);

//Kopira set i vraca pokazivac novog
int **copySet(matrix M);