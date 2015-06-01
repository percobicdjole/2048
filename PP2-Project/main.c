//Probna verzija igre
#include "logic.h"
#include "graphics.h"
#include "ai.h"

void game(matrix *m, enum modes rezim, int stayInMenu);
int options(char *menu[]);
int swipe(matrix *M, int direction, unsigned int *score);
void swipeNoAnimation(matrix *M, int direction, unsigned int *score);
void showHint(matrix *m, int starty, int startx);
void xTo2048(matrix *m);
void doubleDouble(matrix *m);
void freeTwo(matrix *m);

enum modes {normal, xtile, autoplay, autoplayx};

struct settings
{
	enum rezim mode;
	int size;
	theme theme;
}settings;

main()
{
	matrix m;

	char *mainMenu[] = {
		"Igraj",
		"Rezim",
		"Opcije",
		"Uputstvo",
		"O autorima",
		"Izlaz",
		NULL
	};

	char *modesMenu[] = {
		"Obican",
		"X Plocica",
		"Autoplay",
		"Autoplay X",
		NULL
	};

	char *optionsMenu[] = {
		"Dimenzije",
		"Tema",
	//	"Pozadina",
		"Nazad",
		NULL
	};

	char *sizeMenu[] =
	{
		"4x4",
		"5x5",
		NULL
	};

	char *themesMenu[] =
	{
		"Plava",
		"Crvena",
		"Zelena",
		NULL
	};

	_Bool stayInMenu = 1;
	
	
	//CURSES INCIJALIZACIJA
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	initiateThemes();
	resize_term(40, 80);
		
	splashScreen();  //ETF SPLASH SCREEN
	
	//Podrazumevana settings
	settings.mode = normal;
	settings.size = 4;
	settings.theme = zelena;
	//
	
	intiateColors(settings.theme);

	setSeed();
	resize_term(20, 100);
	bkgd(COLOR_PAIR(INTERFACE)); refresh();
	while (stayInMenu)
	{
		display2048(0, 20);
		switch (menu(mainMenu,0,0))
		{
			case 1: erase(); game(&m, settings.mode, stayInMenu); break;
			case 2: switch (menu(modesMenu, 0, 0))
			{
				case 1:settings.mode = normal; break;
				case 2:settings.mode = xtile; break;
				case 3:settings.mode = autoplay; break;
				case 4:settings.mode = autoplayx; break;
			}
			break;
			case 3: 
			while (stayInMenu)
			{
				switch (options(optionsMenu))
				{
					case 1: switch (menu(sizeMenu, 0, 0))
					{
						case 1:settings.size = 4; break;
						case 2:settings.size = 5; break;
					}break;
					
					case 2: switch (menu(themesMenu, 0, 0))
					{
						case 1:settings.theme = plava; break;
						case 2:settings.theme = crvena; break;
						case 3:settings.theme = zelena; break;
					}intiateColors(settings.theme); break;
					case 3:stayInMenu = 0; break;
			}
			}stayInMenu = 1; break;
			case 6:exit(0); break;
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
		displayMatrix(0, -1, *M);
	}
	if (moved)
	{
		_sleep(25);
		spawnNumber(M);
		displayMatrix(0, -1, *M);
	}
	return moved;
}

void swipeNoAnimation(matrix *M, int direction, unsigned int *score)
{
	int changes, moved, last_merged[5] = { 0 };
	changes = moved = moveStep(M, direction, last_merged, score);
	while (changes)
	{
		changes = moveStep(M, direction, last_merged, score);	
	}
	if (moved)
	{
		spawnNumber(M);
		displayMatrix(0, -1, *M);
	}
}

void game(matrix *m, enum rezim rezim, int stayInMenu)
{
	if (settings.size == 4) resize_term(15, 62);
	else resize_term(18, 75);
	unsigned int score = 0;
	*m = newMatrix(settings.size);
	box(stdscr, 0, 0);
	switch (rezim)
	{
	case xtile:
		{
			int x = randomInt(0, settings.size*settings.size-1);
			m->set[x / settings.size][x%settings.size] = 3;
		}
	case normal:
		{history hist = newHistory(5, m);
		state previous = getState(*m, 0);
		while (stayInMenu)
		{
			mvprintw(2, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "REZULTAT: %d", score);
			mvprintw(4, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni ESC za meni!");
			mvprintw(5, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni h za pomoc!");
			mvprintw(6, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni u za undo!");
			int h, valid_move = 1;
			displayMatrix(0, -1, *m);
			switch (getch())
			{
			case KEY_LEFT:
				valid_move = swipe(m, LEFT, &score);
				if (valid_move)
					pushHistory(&hist, previous);
				previous = getState(*m, score);
				break;
			case KEY_RIGHT:
				valid_move = swipe(m, RIGHT, &score);
				if (valid_move)
					pushHistory(&hist, previous);
				previous = getState(*m, score);
				break;
			case KEY_UP:
				valid_move = swipe(m, UP, &score);
				if (valid_move)
					pushHistory(&hist, previous);
				previous = getState(*m, score);
				break;
			case KEY_DOWN:
				valid_move = swipe(m, DOWN, &score);
				if (valid_move)
					pushHistory(&hist, previous);//Push(previous)
				previous = getState(*m, score);
				break;
			case KEY_ESC:stayInMenu = 0; break;
				case 'u': 
					popHistory(&hist, &score);
					previous = getState(*m, score);
					displayMatrix(0, -1, *m); 
				break;
				case 'h': showHint(m, 8, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0)); break;
				case 'x':if(settings.mode==xtile)xTo2048(m); break;
				case '2':if (settings.mode == normal)doubleDouble(m); score *= 2; break;
				case 'f':if (settings.mode == normal)freeTwo(m); break;
			}
		}
	freeState(&previous, m->size);
	destroyHistory(&hist);
	}
	break;
	case autoplayx:
	{
		int x = randomInt(0, settings.size*settings.size - 1);
		m->set[x / settings.size][x%settings.size] = 3;
	}
	case autoplay:
		halfdelay(1);
		mvprintw(4, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni ESC za meni!");
		displayMatrix(0, -1, *m);
		while (stayInMenu)
		{
			mvprintw(2, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "REZULTAT: %d", score);
			switch (get_hint(*m))
			{
			case LEFT:
				swipeNoAnimation(m, LEFT, &score);
				break;
			case RIGHT:
				swipeNoAnimation(m, RIGHT, &score);
				break;
			case UP:
				swipeNoAnimation(m, UP, &score);
				break;
			case DOWN:
				swipeNoAnimation(m, DOWN, &score);
				break;
			case 4:stayInMenu = 0;
				mvprintw(4, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Kraj, pritsni bilo koje dugme!");
				nocbreak();
				cbreak();
				getch();
				break;
			}
			if (getch() == KEY_ESC)stayInMenu = 0;
		}
	}
	
	freeMatrix(m);
	erase();
	resize_term(20, 100);
	bkgd(COLOR_PAIR(INTERFACE)); refresh();
}

void showHint(matrix *m, int starty, int startx)
{
	int h = get_hint(*m);
	WINDOW *hint;
	hint = newwin(5, 27, starty, startx);
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
}

int options(char *menu[])
{
	WINDOW *menu_win;
	int highlight = 1, choice = 0, c, n_choices;
	for (n_choices = 0; menu[n_choices]; n_choices++);
	cbreak();
	menu_win = newwin(20, 20, 0, 0);
	keypad(menu_win, TRUE);

	int x=3, y=2, i;

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
		wrefresh(menu_win);
		return choice;
}

void xTo2048(matrix *m)
{
	for (int i = 0; i < settings.size; i++)
		for (int j = 0; j < settings.size; j++)
		{
			if (m->set[i][j] == 3)
			{
				m->set[i][j] = 2048;
				break;
			}
		}
}

void doubleDouble(matrix *m)
{
	for (int i = 0; i < settings.size; i++)
		for (int j = 0; j < settings.size; j++)
		{
			m->set[i][j] *= 2;
		}
}

void freeTwo(matrix *m)
{
	int xmin, ymin, min;
	for (int k = 0; k < 2; k++)
	{
		xmin = ymin = 0; min = 65536;
		for (int i = 0; i < settings.size; i++)
			for (int j = 0; j < settings.size; j++)
			{
				if (m->set[i][j]!=0 && m->set[i][j] < min)
				{
					min = m->set[i][j];
					ymin = j;
					xmin = i;
				}
			}
		m->set[xmin][ymin] = 0;
	}
}