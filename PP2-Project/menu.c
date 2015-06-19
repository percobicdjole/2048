#include <curses.h>
#include "graphics.h"
#include "logic.h"

struct settings
{
	enum rezim mode;
	int size;
	theme theme;
}settings;


int menu(char *choices[], int starty, int startx)
{
	WINDOW *menu_win;
	int highlight = 1, choice = 0, c, n_choices;
	for (n_choices = 0; choices[n_choices]; n_choices++);
	cbreak();
	menu_win = newwin(20, 20, starty, startx);
	werase(menu_win);
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
	delwin(menu_win);
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

void msgBox(int startx, char text[25])
{
	WINDOW *message;
	cbreak();
	int duzina = strlen(text);
	message = newwin(5, duzina + 4, (getmaxy(stdscr) - 5) / 2, startx);
	keypad(message, TRUE);
	wbkgd(message, COLOR_PAIR(INTERFACE));
	box(message, 0, 0);
	mvwaddstr(message, 2, 2, text);
	wrefresh(message);
	wgetch(message);
	werase(message);
	wrefresh(message);
	delwin(message);
}

void showHint(matrix *m, int starty, int startx)
{
	int h = get_hint(*m);
	WINDOW *hint;
	cbreak();
	hint = newwin(5, 27, starty, startx);
	keypad(hint, TRUE);
	wbkgd(hint, COLOR_PAIR(INTERFACE));
	box(hint, 0, 0);
	switch (h)
	{
	case LEFT: mvwprintw(hint, 2, 2, "Najbolji izbor je LEVO!"); break;
	case RIGHT: mvwprintw(hint, 2, 2, "Najbolji izbor je DESNO!"); break;
	case UP:mvwprintw(hint, 2, 2, "Najbolji izbor je GORE!"); break;
	case DOWN:mvwprintw(hint, 2, 2, "Najbolji izbor je DOLE!"); break;
	case 4:mvwprintw(hint, 2, 6, "IGRA JE GOTOVA!"); break;
	}
	wrefresh(hint);
	wgetch(hint);
	werase(hint);
	wrefresh(hint);
	delwin(hint);
}

int options(char *menu[])
{
	WINDOW *menu_win;
	int highlight = 1, choice = 0, c, n_choices;
	for (n_choices = 0; menu[n_choices]; n_choices++);
	cbreak();
	menu_win = newwin(20, 20, 0, 0);
	keypad(menu_win, TRUE);

	int x = 3, y = 2, i;

	wbkgd(menu_win, COLOR_PAIR(INTERFACE));
	box(menu_win, 0, 0);
	for (i = 0; i < n_choices; ++i)
	{

		switch (i + 1)
		{
		case 1:
			if (highlight == i + 1)wattron(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
			mvwprintw(menu_win, y + i, x, "%s", menu[i]);
			if (highlight == i + 1)wattroff(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
			wattron(menu_win, A_NORMAL);
			wprintw(menu_win, "%s", settings.size == 4 ? ": 4x4" : ": 5x5");
			wattroff(menu_win, A_NORMAL);

			break;
		case 2:
			if (highlight == i + 1)wattron(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
			mvwprintw(menu_win, y + i, x, "%s", menu[i]);
			if (highlight == i + 1)wattroff(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
			wattron(menu_win, A_NORMAL);
			wprintw(menu_win, ": %s", settings.theme.name);
			wattroff(menu_win, A_NORMAL);
			break;
		case 3:
			if (highlight == i + 1)wattron(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
			mvwprintw(menu_win, y + i, x, "%s", menu[i]);
			if (highlight == i + 1)wattroff(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
			break;
		}
		++y;
	}
	wrefresh(menu_win);

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
		y = 2;
		wbkgd(menu_win, COLOR_PAIR(INTERFACE));
		box(menu_win, 0, 0);
		for (i = 0; i < n_choices; ++i)
		{

			switch (i + 1)
			{
			case 1:
				if (highlight == i + 1)wattron(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
				mvwprintw(menu_win, y + i, x, "%s", menu[i]);
				if (highlight == i + 1)wattroff(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
				wattron(menu_win, A_NORMAL);
				wprintw(menu_win, "%s", settings.size == 4 ? ": 4x4" : ": 5x5");
				wattroff(menu_win, A_NORMAL);

				break;
			case 2:
				if (highlight == i + 1)wattron(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
				mvwprintw(menu_win, y + i, x, "%s", menu[i]);
				if (highlight == i + 1)wattroff(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
				wattron(menu_win, A_NORMAL);
				wprintw(menu_win, ": %s", settings.theme.name);
				wattroff(menu_win, A_NORMAL);
				break;
			case 3:
				if (highlight == i + 1)wattron(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
				mvwprintw(menu_win, y + i, x, "%s", menu[i]);
				if (highlight == i + 1)wattroff(menu_win, COLOR_PAIR(INTERFACE) | A_REVERSE);
				break;
			}
			++y;
		}
		wrefresh(menu_win);

		if (choice != 0)
			break;
	}
	werase(menu_win);
	delwin(menu_win);
	return choice;
}
