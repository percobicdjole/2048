#include <curses.h> 

typedef struct theme
{
	unsigned char first, second, third, fourth, fifth, interfaceColor;
}theme;

theme crvena, zelena, plava, RGB;

//MATRIX PARAMETERS
#define HEIGHT 3//visina jednog polja, 3 reda
#define WIDTH 7//sirina jedndog polja, 7 znakova

#define KEY_ESC 27

#define BW 0
#define FIRST 1
#define SECOND 2
#define THIRD 3
#define FOURTH 4
#define FIFTH 5
#define INTERFACE 6
#define WHITE 15

void colorPalette();//paletu boja prikazuje funkcija colorPalette
void intiateColors();//inicijalizuje parove boja
void initiateThemes(); //inicijalizuje boje za teme
void displayMatrix();//ne dozvoljava mi da koristim matrix type ovde!! nije odgovarajuci opis! ->> void displayMatrix(int startx, int starty, matrix m);
unsigned char colorOfNumber(unsigned short x);//vraca boju odredjenog broja
void printMenu(WINDOW *menu_win, int highlight);//ispisuje menu
int menu();

