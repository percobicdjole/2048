#include "graphics.h"
#include "logic.h"


char *choices[] = {
	"Normal",
	"X-tile",
	"Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void displayMatrix(matrix m)//dodati opciju za x tile
{

	for (int i = 0; i < m.size; i++)
	{
		for (int j = 0; j < m.size; j++)
		{
			
			if (m.set[i][j] == 1)
			{
				attron(COLOR_PAIR(0)|A_REVERSE);
				for (int k = i*HEIGHT; k < (i + 1) * HEIGHT; k++)
				{
					for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvprintw(k + 2, l + 1, " ");
				}
				mvprintw((i + 1) * HEIGHT, (j + 1) * 4 + 3 * j, "X");
				attroff(COLOR_PAIR(0) | A_REVERSE);
			}
			else
			{
				if (m.set[i][j])
				{
					unsigned char boja = colorOfNumber(m.set[i][j]);
					attron(COLOR_PAIR(boja));

					for (int k = i*HEIGHT; k < (i + 1) * HEIGHT; k++)
					{
						for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvprintw(k + 2, l + 1, " ");
					}

					{	//ISPIS ELEMENATA MATRICE, PODESAVANJE PO HORIZONTALI
						unsigned char y, x;
						y = (i + 1) * HEIGHT;
						if (m.set[i][j] < 100)									x = (j + 1) * 4 + 3 * j;
						else if (m.set[i][j] >= 100 && m.set[i][j] < 10000)		x = (j + 1) * 4 + 3 * j - 1;
						else if (m.set[i][j] >= 10000)							x = (j + 1) * 4 + 3 * j - 2;
						mvprintw(y, x, "%u", m.set[i][j]);
					}

					attroff(COLOR_PAIR(boja));
				}
				else
				{
					attron(COLOR_PAIR(0));
					for (int k = i*HEIGHT; k < (i + 1) * HEIGHT; k++)
					{
						for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvprintw(k + 2, l + 1, " ");
					}
					mvprintw((i + 1) * HEIGHT, (j + 1) * 4 + 3 * j, ".");
					attroff(COLOR_PAIR(0));
				}
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
	init_pair(6, COLOR_BLACK, 14);//za tekst
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

int menu()
{
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;
	cbreak(); 
	menu_win = newwin(10, 15, 5, 30);
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
	werase(menu_win);
	wrefresh(menu_win);
	return choice;
}