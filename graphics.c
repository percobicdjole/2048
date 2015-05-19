#include "graphics.h"

int startx = 50;
int starty = 8;

char *choices[] = {
	"New game",
	"Game modes",
	"Options",
	"Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void matrixDisplay(unsigned short matrica[SIZE][SIZE])
//dodati opciju za x tile
{
	//unsigned short matrica[SIZE][SIZE] = { { 0, 2, 4, 8 }, { 16, 32, 64, 128 }, { 256, 512, 1024, 2048 }, { 4096, 8192, 16384, 32768 } };

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (matrica[i][j])
			{
				unsigned char boja = colorOfNumber(matrica[i][j]);
				attron(COLOR_PAIR(boja));

				for (int k = i*HEIGHT; k < (i + 1) * HEIGHT; k++)
				{
					for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvprintw(k + 2, l + 1, " ");
				}

				{	//ISPIS ELEMENATA MATRICE, PODESAVANJE PO HORIZONTALI
					unsigned char y, x;
					y = (i + 1) * HEIGHT;
					if (matrica[i][j] < 100)									x = (j + 1) * 4 + 3 * j;
					else if (matrica[i][j] >= 100 && matrica[i][j] < 10000)		x = (j + 1) * 4 + 3 * j - 1;
					else if (matrica[i][j] >= 10000)							x = (j + 1) * 4 + 3 * j - 2;
					mvprintw(y, x, "%u", matrica[i][j]);
				}

				attroff(COLOR_PAIR(boja));
			}
			else
			{
				attron(COLOR_PAIR(0));
				mvprintw((i + 1) * HEIGHT, (j + 1) * 4 + 3 * j, ".");
				attroff(COLOR_PAIR(0));
			}

		}
	}

	refresh();
}

unsigned char colorOfNumber(unsigned short x)
{
	if (x == 2)					    return 1;
	else if (x >= 4 && x <= 16)     return 2;
	else if (x >= 32 && x <= 64)    return 3;
	else if (x >= 128 && x <= 256)  return 4;
	else if (x >= 512)				return 5;
	else return 0;
}

void colorPalette()
{
	start_color();
	for (unsigned char i = 0; i < 16; i++)
	{
		init_pair(i, 7, i);
		attron(COLOR_PAIR(i));
		printw("%u\t\n", i);
		attroff(COLOR_PAIR(i));
	}
	refresh();
}

void intiateColors()
{
	init_pair(0, COLOR_BLACK, WHITE);
	init_pair(1, COLOR_BLACK, FIRST);
	init_pair(2, WHITE, SECOND);
	init_pair(3, WHITE, THIRD);
	init_pair(4, WHITE, FOURTH);
	init_pair(5, COLOR_BLACK, FIFTH);
	init_pair(6, COLOR_BLACK, 14);
}

void printMenu(WINDOW *menu_win, int highlight)
{
	int x, y, i;
	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for (i = 0; i < n_choices; ++i)
	{
		if (highlight == i + 1) 
		{
			wattron(menu_win, A_REVERSE | COLOR_PAIR(6));
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE | COLOR_PAIR(6));
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

void menu()
{
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;
	cbreak(); 
	startx = (30);
	starty = (1);
	menu_win = newwin(10, 20, starty, startx);
	keypad(menu_win, TRUE);
	printMenu(menu_win, highlight);
	while (TRUE)
	{
		c = wgetch(menu_win);
		switch (c)
		{
		case KEY_UP:
			if (highlight == 1)
				highlight = n_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case 10:
			choice = highlight;
			break;

		}
		printMenu(menu_win, highlight);
		if (choice != 0) 
			break;
	}
	clrtoeol();
	refresh();


}