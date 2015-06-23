#ifndef MENU_H
#define MENU_H 

#include "logic.h"

int menu(char *choices[], int starty, int startx);	/*PRIKAZUJE MENU, NA ZADATIM POCETNIM KOORDINATAMA, POVRATNA VREDNOST POZICIJA STRINGA U MENIJU POCEV OD 1*/
int options(char *menu[]);							/*POSEBNA VERZIJA MENIJA PRIKAZUJE TRENUTNO PODESAVANJE*/
void msgBox(int startx, char text[25]);				/*PRIKAZUJE PORUKU CENTRIRANU PO Y KOORDINATI*/
void errorMsg(char text[25]);						/*PRIKAZUJE PORUKU GRESKE NA SREDINI EKRANA*/

void showHint(matrix *m, int starty, int startx);	/*PRIKAZUJE HINT NA ZADATIM POCETNIM KOORDINATAMA*/

#endif /* MENU_H */
