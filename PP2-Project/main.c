//Probna verzija igre
#include "logic.h"
#include "graphics.h"
#include "ai.h"
#include <time.h>

void game(matrix *m, int stayInMenu);
void gamex(matrix *m, int stayInMenu);

main()
{
	matrix m;
	int mode=0;
	_Bool stayInMenu = 1;
	setSeed();
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	initiateThemes();
	intiateColors(plava);

	while (stayInMenu)
	{
		mvprintw(0, 45, "2048 Alpha 2 version (23.5.2015.)");
		refresh();
		switch (menu())
		{
			case 1:m = newMatrix(4); game(&m, stayInMenu); break;
			case 2:
			{
				m = newMatrix(4);
				int nmb = randomInt(0, 15); //Za Ivana: napravi f-ju za postavljanje x-tilea od ovoga
				m.set[nmb / 4][nmb % 4] = 1;
				game(&m, stayInMenu);
			}
			break;
			case 3:exit(0); break;
		}
		stayInMenu = 1;
	}
	endwin();
}

void game(matrix *m, int stayInMenu)
{
	int score = 0; 
	short vector[5];
	attron(COLOR_PAIR(INTERFACE));
	mvprintw(1, 45, "Press ESC-to get back to menu!");
	mvprintw(2, 45, "Press h-to get hint!");
	attroff(COLOR_PAIR(INTERFACE));
	displayMatrix(6, 3, *m);
	while (stayInMenu)
	{
		attron(COLOR_PAIR(INTERFACE));
		mvprintw(4 * 3 + 3 + 2, 7, "SCORE: %d", score);
		attroff(COLOR_PAIR(INTERFACE));
		switch (getch())
		{
			case KEY_LEFT:while (moveStep(&*m, LEFT, vector, &score))
						  { 
							  _sleep(100); 
							  displayMatrix(6, 3, *m); 
			}; break;
			case KEY_RIGHT:while (moveStep(&*m, RIGHT, vector, &score))
			{
							  _sleep(100);
							  displayMatrix(6, 3, *m);
			}; break;
			case KEY_UP:while (moveStep(&*m, UP, vector, &score))
			{
							  _sleep(100);
							  displayMatrix(6, 3, *m);
			}; break;
			case KEY_DOWN:while (moveStep(&*m, DOWN, vector, &score))
			{
							  _sleep(100);
							  displayMatrix(6, 3, *m);
			}; break;
			case KEY_ESC:stayInMenu = 0; break;
			case 'h': 
				if (get_hint(*m) == LEFT)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 7, 7, "Najbolji izbor je levo!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (get_hint(*m) == RIGHT)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 7, 7, "Najbolji izbor je desno!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (get_hint(*m) == UP)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 7, 7, "Najbolji izbor je gore!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (get_hint(*m) == DOWN)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 7, 7, "Najbolji izbor je dole!");
					attroff(COLOR_PAIR(INTERFACE));
				}
		}
		spawnNumber(m);
		displayMatrix(6, 3, *m);
	}
	free(*m->set);
	erase();
}
