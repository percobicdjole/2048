//Probna verzija igre
#include "logic.h"
#include "graphics.h"
#include "ai.h"
#include <time.h>

void game(matrix *m, int stayInMenu);
void swipe(matrix *M, int direction, unsigned int *score);

main()
{
	matrix m;
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
				int nmb = randomInt(0, 15); 
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

void swipe(matrix *M, int direction, unsigned int *score)
{
	int changes, moved, last_merged[5] = { 0 };
	changes = moved = moveStep(M, direction, last_merged, score);
	while (changes)
	{
		changes = moveStep(M, direction, last_merged, score);
		_sleep(75);
		displayMatrix(6, 3, *M);
	}
	if (moved)
	{
		_sleep(50);
		spawnNumber(M);
	}
		
}

void game(matrix *m, int stayInMenu)
{
	unsigned int score = 0; 
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
			case KEY_LEFT:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				swipe(m, LEFT, &score);
				break;
			case KEY_RIGHT:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				swipe(m, RIGHT, &score);
				break;
			case KEY_UP:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				swipe(m, UP, &score);
				break;
			case KEY_DOWN:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				swipe(m, DOWN, &score);
				break;
			case KEY_ESC:stayInMenu = 0; break;
			case 'h': 
				if (get_hint(*m) == LEFT)//privremeno resenje, u toku nedelje uraditi posebne prozore
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je levo!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (get_hint(*m) == RIGHT)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je desno!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (get_hint(*m) == UP)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je gore!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (get_hint(*m) == DOWN)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je dole!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				break;
		}
		displayMatrix(6, 3, *m);
	}
	free(*m->set);
	erase();
}
