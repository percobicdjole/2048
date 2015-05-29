//Probna verzija igre
#include "logic.h"
#include "graphics.h"
#include "ai.h"
#include <time.h>

void game(matrix *m, int stayInMenu);
void autoplay(matrix *m, int stayInMenu);
int swipe(matrix *M, int direction, unsigned int *score);

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
			case 3: m = newMatrix(4); autoplay(&m, stayInMenu); break;
			case 4:exit(0); break;
		}
		stayInMenu = 1;
	}
	endwin();
}

int swipe(matrix *M, int direction, unsigned int *score)
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
		displayMatrix(6, 3, *M);
	}
	return moved;
}

void game(matrix *m, int stayInMenu)
{
	unsigned int score = 0;
	int h, valid_move=1;
	history hist = newHistory(5, m);
	state previous = getState(*m, 0);
	attron(COLOR_PAIR(INTERFACE));
	mvprintw(1, 45, "Press ESC-to get back to menu!");
	mvprintw(2, 45, "Press h-to get hint!");
	mvprintw(3, 45, "Press u-for undo");
	attroff(COLOR_PAIR(INTERFACE));
	displayMatrix(6, 3, *m);
	while (stayInMenu)
	{
		attron(COLOR_PAIR(INTERFACE));
		mvprintw(4 * 3 + 3 + 2, 7, "SCORE: %4d", score);
		attroff(COLOR_PAIR(INTERFACE));
		switch (getch())
		{
			case KEY_LEFT:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				valid_move = swipe(m, LEFT, &score);
				if (valid_move)
					pushHistory(&hist,previous);
				previous = getState(*m, score);
				break;
			case KEY_RIGHT:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				
				valid_move = swipe(m, RIGHT, &score);
				if (valid_move)
					pushHistory(&hist,previous);
				previous = getState(*m, score);
				break;
			case KEY_UP:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				
				valid_move = swipe(m, UP, &score);
				if (valid_move)
					pushHistory(&hist,previous);
				previous = getState(*m, score);
				break;
			case KEY_DOWN:
				mvprintw(4 * 3 + 3 + 3, 7, "                                ");
				
				valid_move = swipe(m, DOWN, &score);
				if (valid_move)
					pushHistory(&hist,previous);//Push(previous)
				previous = getState(*m, score);
				break;
			case KEY_ESC:stayInMenu = 0; break;
			case 'u': 
				popHistory(&hist,&score);
				previous = getState(*m, score);
				displayMatrix(6, 3, *m);
				break;
			case 'h': 
				h = get_hint(*m);
				if (h == LEFT)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je LEVO!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (h == RIGHT)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je DESNO!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (h == UP)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je GORE!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				else if (h == DOWN)
				{
					attron(COLOR_PAIR(INTERFACE));
					mvprintw(4 * 3 + 3 + 3, 7, "Najbolji izbor je DOLE!");
					attroff(COLOR_PAIR(INTERFACE));
				}
				refresh();
				break;
		}
	}
	freeState(&previous, m->size);
	freeMatrix(m);
	destroyHistory(&hist);
	erase();
}

void autoplay(matrix *m, int stayInMenu)
{
	unsigned int score = 0;
	attron(COLOR_PAIR(INTERFACE));
	//mvprintw(1, 45, "Press ESC-to get back to menu!");
	attroff(COLOR_PAIR(INTERFACE));
	displayMatrix(6, 3, *m);
	while (stayInMenu)
	{
		attron(COLOR_PAIR(INTERFACE));
		mvprintw(4 * 3 + 3 + 2, 7, "SCORE: %d", score);
		attroff(COLOR_PAIR(INTERFACE));
		switch (get_hint(*m))
		{
		case LEFT:
			swipe(m, LEFT, &score);
			break;
		case RIGHT:
			swipe(m, RIGHT, &score);
			break;
		case UP:
			swipe(m, UP, &score);
			break;
		case DOWN:
			swipe(m, DOWN, &score);
			break;
		case 4:stayInMenu = 0;
			break;
		}
	}
	freeMatrix(m);
	mvprintw(18, 7, "GAME OVER");
	mvprintw(19, 7, "Pritisni bilo koje dugme za povratak u glavni meni!");
	getch();
	erase();
}