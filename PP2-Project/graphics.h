#include <curses.h> 

//MATRIX PARAMETERS
#define HEIGHT 3//visina jednog polja, 3 reda
#define WIDTH 7//sirina jedndog polja, 7 znakova

//COLORS 2048        RANGE       COLOR
#define FIRST 8      //2		 GRAY
#define SECOND 4     //4-16	     CRIMSON
#define THIRD 12     //32-64	 RED
#define FOURTH 6     //128-256   DYELLOW
#define FIFTH 14     //512-2048  YELLOW
#define WHITE 15

void colorPalette();//paletu boja prikazuje funkcija colorPalette
void intiateColors();//inicijalizuje parove boja
void diplayMatrix();//ne dozvoljava mi da koristim matrix type ovde!!
unsigned char colorOfNumber(unsigned short x);//vraca boju odredjenog broja
void printMenu(WINDOW *menu_win, int highlight);//ispisuje menu
int menu();


