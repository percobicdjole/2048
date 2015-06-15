#include "graphics.h"
#include "logic.h"

void displayMatrix(int startx, int starty, matrix m)//dodati opciju za x tile
{
	for (int i = 0; i < m.size; i++)
	{
		for (int j = 0; j < m.size; j++)
		{
			
			if (m.set[i][j] == 3)
			{
				attron(COLOR_PAIR(0) | A_REVERSE);
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
						else if (m.set[i][j] >= 100 && m.set[i][j] < 1000)		x = (j + 1) * 4 + 3 * j - 1;
						else if (m.set[i][j] >= 1000)							x = (j + 1) * 4 + 3 * j - 2;
						mvprintw(y+starty, x+startx, "%d", m.set[i][j]);
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

mystrcpy(char dest[], char src[])
{
	int i = 0;

	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}

	dest[i] = '\0';
}

void initiateThemes()
{
	//crvena
	crvena.first.color = 4;   //2	
	crvena.first.contrast = 15;
	
	crvena.second.color = 12;  //4-16
	crvena.second.contrast = 15;

	crvena.third.color = 5;   //32-64
	crvena.third.contrast = 14;

	crvena.fourth.color = 13; //128-256
	crvena.fourth.contrast = 14;

	crvena.fifth.color = 12;  //512-2048
	crvena.fifth.contrast = 11;

	crvena.interface.color = 4;
	crvena.interface.contrast = 7;

	mystrcpy(crvena.name, "crvena");
	
	//plava
	plava.first.color = 1;   //2	
	plava.first.contrast = 15;

	plava.second.color = 9;  //4-16
	plava.second.contrast = 15;

	plava.third.color = 3;   //32-64
	plava.third.contrast = 13;

	plava.fourth.color = 11; //128-256
	plava.fourth.contrast = 0;

	plava.fifth.color = 10;  //512-2048
	plava.fifth.contrast = 14;

	plava.interface.color = 9;
	plava.interface.contrast = 7;

	mystrcpy(plava.name, "plava");

	//zelena
	zelena.first.color = 2;   //2	
	zelena.first.contrast = 0;

	zelena.second.color = 10;  //4-16
	zelena.second.contrast = 0;

	zelena.third.color = 3;   //32-64
	zelena.third.contrast = 0;

	zelena.fourth.color = 11; //128-256
	zelena.fourth.contrast = 0;

	zelena.fifth.color = 10;  //512-2048
	zelena.fifth.contrast = 12;

	zelena.interface.color = 10;
	zelena.interface.contrast = 0;

	mystrcpy(zelena.name, "zelena");

	//zuta
	zuta.first.color = 6;   //2	
	zuta.first.contrast = 0;

	zuta.second.color = 14;  //4-16
	zuta.second.contrast = 0;

	zuta.third.color = 4;   //32-64
	zuta.third.contrast = 14;

	zuta.fourth.color = 12; //128-256
	zuta.fourth.contrast = 0;

	zuta.fifth.color = 14;  //512-2048
	zuta.fifth.contrast = 9;

	zuta.interface.color = 14;
	zuta.interface.contrast = 0;

	mystrcpy(zuta.name, "zuta");

	//bios
	BIOS.first.color = 6;   //2	
	BIOS.first.contrast = 9;

	BIOS.second.color = 14;  //4-16
	BIOS.second.contrast = 9;

	BIOS.third.color = 14;   //32-64
	BIOS.third.contrast = 4;

	BIOS.fourth.color = 14; //128-256
	BIOS.fourth.contrast = 12;

	BIOS.fifth.color = 11;  //512-2048
	BIOS.fifth.contrast = 12;

	BIOS.interface.color = 14;
	BIOS.interface.contrast = 9;

	mystrcpy(BIOS.name, "BIOS");
}

void intiateColors(theme tema)
{
	init_pair(BW, COLOR_BLACK, WHITE);
	init_pair(FIRST, tema.first.contrast, tema.first.color);
	init_pair(SECOND, tema.second.contrast, tema.second.color);
	init_pair(THIRD, tema.third.contrast, tema.third.color);
	init_pair(FOURTH, tema.fourth.contrast, tema.fourth.color);
	init_pair(FIFTH, tema.fifth.contrast, tema.fifth.color);
	init_pair(INTERFACE, tema.interface.color, tema.interface.contrast);//za tekst

	//init_pair(INTERFACE, 3, 7);//za tekst

	init_pair(8, tema.interface.color, tema.interface.color);//za digitalni displej
}

int menu(char *choices[], int starty, int startx)
{
	WINDOW *menu_win;
	int highlight = 1, choice = 0, c, n_choices;
	for (n_choices = 0; choices[n_choices]; n_choices++);
	cbreak(); 
	menu_win = newwin(20, 20, starty, startx);
	keypad(menu_win, TRUE);
	printMenu(menu_win, choices, n_choices, highlight);
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
		printMenu(menu_win, choices, n_choices, highlight);
		if (choice != 0) 
			break;
	}
	werase(menu_win);
	wrefresh(menu_win);
	return choice;
}

void printMenu(WINDOW *menu_win, char *choices[], int n_choices, int highlight)
{
	int x, y, i;
	x = 3;
	y = 2;
	wbkgd(menu_win, COLOR_PAIR(INTERFACE));
	box(menu_win, 0, 0);
	for (i = 0; i < n_choices; ++i)
	{
		if (highlight == i + 1)
		{
			wattron(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
			mvwprintw(menu_win, y + i, x, "%s", choices[i]);
			wattroff(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
		}
		else mvwprintw(menu_win, y + i, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

void displayNumber(int starty, int startx, int number)
{	
	int endx = 30;
	WINDOW *score;
	score = newwin(5, 30, starty, startx);
	werase(score);
	wbkgd(score, COLOR_PAIR(INTERFACE));
	int space = -5;
	wattron(score,COLOR_PAIR(8));
	while (number)
	{
		switch (number % 10)
		{
			case 0:
				mvwprintw(score,0, endx + space, "xxxx");
				for (int i = 1; i <= 3; i++)mvwprintw(score,i, endx + space, "x");
				for (int i = 1; i <= 3; i++)mvwprintw(score,i, 3 + endx + space, "x");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
			case 1:
				mvwprintw(score,0, endx + space, "xxx");
				for (int i = 1; i <= 3; i++)mvwprintw(score,i, 1 + endx + space, "xx");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
			case 2:
				mvwprintw(score,0, endx + space,"xxxx");
				mvwprintw(score,1, 3 + endx + space, "x");
				mvwprintw(score,2, endx + space, "xxxx");
				mvwprintw(score,3, endx + space, "x");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
			case 3:
				mvwprintw(score,0, endx + space,     "xxxx");
				mvwprintw(score,1, 3 + endx + space, "x");
				mvwprintw(score,2, endx + space, "xxxx");
				mvwprintw(score,3, 3 + endx + space, "x");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
			case 4:
				for (int i = 0; i <= 4; i++)mvwprintw(score,i, 3 + endx + space, "x");
				for (int i = 0; i <= 1; i++)mvwprintw(score,i, endx + space, "x");
				mvwprintw(score,2, endx + space, "xxx");
			break;
			case 5:
				mvwprintw(score,0, endx + space,     "xxxx");
				mvwprintw(score,1, endx + space, "x");
				mvwprintw(score,2, endx + space, "xxxx");
				mvwprintw(score,3, 3 + endx + space, "x");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
			case 6:
				mvwprintw(score,0, endx + space,   "xxxx");
				mvwprintw(score,1, endx + space, "x");
				mvwprintw(score,2, endx + space, "xxxx");
				mvwprintw(score,3, endx + space, "x");
				mvwprintw(score,3, 3 + endx + space, "x");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
			case 7:
				mvwprintw(score,0, endx + space,     "xxxx");
				mvwprintw(score,1, 3 + endx + space, "x");
				mvwprintw(score,2, 1 + endx + space, "xxx");
				mvwprintw(score,3, 2 + endx + space, "x");
				mvwprintw(score,4, 2 + endx + space, "x");		
			break;
			case 8:
				mvwprintw(score,0, endx + space,     "xxxx");
				mvwprintw(score,1, endx + space, "x");
				mvwprintw(score,1, 3 + endx + space, "x");
				mvwprintw(score,2, endx + space, "xxxx");
				mvwprintw(score,3, endx + space, "x");
				mvwprintw(score,3, 3 + endx + space, "x");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
			case 9:
				mvwprintw(score,0, endx + space,     "xxxx");
				mvwprintw(score,1, endx + space, "x");
				mvwprintw(score,1, 3 + endx + space, "x");
				mvwprintw(score,2, endx + space, "xxxx");
				mvwprintw(score,3, 3 + endx + space, "x");
				mvwprintw(score,4, endx + space, "xxxx");
			break;
		}
		number /= 10;
		space -= 5;
	}
	wrefresh(score);
	wattroff(score,COLOR_PAIR(8));
}
