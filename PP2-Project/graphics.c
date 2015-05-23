#include "graphics.h"
#include "logic.h"


char *choices[] = {
	"Obicno",
	"X-tile",
	"Izlaz",
};

int n_choices = sizeof(choices) / sizeof(char *);

void displayMatrix(int startx, int starty, matrix m)//dodati opciju za x tile
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
					for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvprintw(starty + k + 2, startx + l + 1, " ");
				}
				mvprintw((i + 1) * HEIGHT + starty, (j + 1) * 4 + 3 * j + startx, "X");
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
						for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvprintw(k + 2 + starty, l + 1 + startx, " ");
					}

					{
						unsigned char y, x;
						y = (i + 1) * HEIGHT;
						if (m.set[i][j] < 100)									x = (j + 1) * 4 + 3 * j;
						else if (m.set[i][j] >= 100 && m.set[i][j] < 10000)		x = (j + 1) * 4 + 3 * j - 1;
						else if (m.set[i][j] >= 10000)							x = (j + 1) * 4 + 3 * j - 2;
						mvprintw(y+starty, x+startx, "%u", m.set[i][j]);
					}

					attroff(COLOR_PAIR(boja));
				}
				else
				{
					attron(COLOR_PAIR(BW));
					for (int k = i*HEIGHT; k < (i + 1) * HEIGHT; k++)
					{
						for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvprintw(k + 2+starty, l + 1+startx, " ");
					}
					mvprintw((i + 1) * HEIGHT+starty, (j + 1) * 4 + 3 * j+startx, ".");
					attroff(COLOR_PAIR(BW));
				}
			}
		}
	}

	refresh();
}

unsigned char colorOfNumber(unsigned short x)
{
	if (x == 2)					    return FIRST;
	else if (x >= 4 && x <= 16)     return SECOND;
	else if (x >= 32 && x <= 64)    return THIRD;
	else if (x >= 128 && x <= 256)  return FOURTH;
	else if (x >= 512)				return FIFTH;
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

void initiateThemes()
{
	//crvena promeniti
	crvena.first = 1;   //2	
	crvena.second = 9;  //4-16	  
	crvena.third = 3;   //32-64
	crvena.fourth = 11; //128-256
	crvena.fifth = 14;  //512-2048
	crvena.interfaceColor = 9;
	//plava
	plava.first = 1;   //2	
	plava.second = 9;  //4-16	  
	plava.third = 3;   //32-64
	plava.fourth = 11; //128-256
	plava.fifth = 14;  //512-2048
	plava.interfaceColor = 9;
}

void intiateColors(theme tema)
{
	init_pair(BW, COLOR_BLACK, WHITE);
	init_pair(FIRST, WHITE, tema.first);
	init_pair(SECOND, WHITE, tema.second);
	init_pair(THIRD, WHITE, tema.third);
	init_pair(FOURTH, WHITE, tema.fourth);
	init_pair(FIFTH, WHITE, tema.fifth);
	init_pair(INTERFACE, COLOR_BLACK, tema.interfaceColor);//za tekst
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
			wattron(menu_win, A_REVERSE | COLOR_PAIR(INTERFACE));
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE | COLOR_PAIR(INTERFACE));
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