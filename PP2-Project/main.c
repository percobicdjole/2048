//Probna verzija igre
#include "logic.h"
#include "graphics.h"
main()
{
	setSeed();
	matrix m=newMatrix(4);
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	intiateColors();
	mvprintw(0, 45, "2048 Alpha version (19.5.2015.)");
	refresh();
	switch (menu())
	{
	case 1:
		attron(COLOR_PAIR(5));
		mvprintw(1, 45, "Press Q-to quit!");
		attroff(COLOR_PAIR(5));
		displayMatrix(m);
		while (TRUE)
		{
			int c = getch();
			switch (c)
			{
			case KEY_LEFT:moveMatrix(&m, LEFT); break;
			case KEY_RIGHT:moveMatrix(&m, RIGHT); break;
			case KEY_DOWN:moveMatrix(&m, DOWN); break;
			case KEY_UP:moveMatrix(&m, UP); break;
			case 'q':exit(0); break;
			default:continue;
			}
			displayMatrix(m);
		}
		break;
	case 2:
		attron(COLOR_PAIR(5));
		mvprintw(1, 45, "Press Q-to quit!");
		attroff(COLOR_PAIR(5));
		{
			int nmb = randomInt(0, 15);
			m.set[nmb / 4][nmb % 4] = 1;
		}
		displayMatrix(m);
		while (TRUE)
		{
			int c = getch();
			switch (c)
			{
			case KEY_LEFT:moveMatrix(&m, LEFT); break;
			case KEY_RIGHT:moveMatrix(&m, RIGHT); break;
			case KEY_DOWN:moveMatrix(&m, DOWN); break;
			case KEY_UP:moveMatrix(&m, UP); break;
			case 'q':exit(0); break;
			default:continue;
			}
			displayMatrix(m);
		}
		break;
	case 3: exit(0); break;
	}
	


	endwin();
}