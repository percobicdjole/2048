#ifndef MENU_H
#define MENU_H 

#include "logic.h"

int options(char *menu[]);
int menu(char *choices[], int starty, int startx);
void errorMsg(char text[25]);
void msgBox(int startx, char text[25]);
void showHint(matrix *m, int starty, int startx);

#endif /* MENU_H */
