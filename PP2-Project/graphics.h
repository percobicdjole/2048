#ifndef GRAPHICS_H
#define GRAPHICS_H 

#include <curses.h> 
#include "logic.h"

struct colorComposition
{
	unsigned char color, contrast;
};

typedef struct theme
{
	struct colorComposition first, second, third, fourth, fifth, interface;
	char name[10];
}theme;

theme crvena, zelena, plava, zuta, BIOS;

/*PARAMETRI MATRICE*/
#define HEIGHT 3	//visina jednog polja, 3 reda
#define WIDTH 7		//sirina jedndog polja, 7 znakova

/*VREDNOSTI ZNAKOVA SA TASTATURE*/
#define KEY_ESC 27
#define CTRL_Z '\032'
#define ENTER 10

#define BW 0		//CRNA POZADINA / BELA SLOVA
#define FIRST 1
#define SECOND 2
#define THIRD 3
#define FOURTH 4
#define FIFTH 5
#define INTERFACE 6
#define DIGITAL 7
#define ERROR 8
#define WHITE 15

void colorPalette();	/*OVA FUNKCIJA PRIKAZUJE PALETU OD 16 BOJA KOJE MOZE DA PRIKAZE KONZOLA*/

void setTheme(theme tema);	/*INICIJALIZUJE PAROVE BOJA*/

void initThemes(); //inicijalizuje boje za teme

void displayMatrix(int starty, int startx, matrix m);

void displayNumber(int starty, int startx, int number);
void display2048(int starty, int startx);
void displayGameOver(int starty, int startx);
void splashScreen();// ETF LOGO ASCII ART

#endif /* GRAPHICS_H */