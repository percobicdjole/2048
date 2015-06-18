#include "logic.h"
#include "graphics.h"
#include "ai.h"
#include "IO.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void game(enum modes rezim, int stayInMenu);
int options(char *menu[]);
int swipe(matrix *M, int direction, unsigned int *score);
void swipeNoAnimation(matrix *M, int direction, unsigned int *score);
void showHint(matrix *m, int starty, int startx);
void getHsc(entry  **score_list, unsigned int *entry_count, unsigned int score);
void msgBox(int startx, char text[25]);

void xTo2048(matrix *m);
void doubleDouble(matrix *m);
void freeTwo(matrix *m);
enum modes {normal, xtile, speed, autoplay, autoplayx};

struct settings
{
	enum rezim mode;
	int size;
	theme theme;
}settings;

main()
{
	char *mainMenu[] = {
		"Igraj",
		"Rezim",
		"Opcije",
		"Uputstvo",
		"Tabela",
		"Izlaz",
		NULL
	};

	char *modesMenu[] = {
		"Obican",
		"X Plocica",
		"Autoplay",
		"Autoplay X",
		"Brzopotezno",
		NULL
	};

	char *optionsMenu[] = {
		"Dimenzije",
		"Tema",
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
		"Plava - SVETLA",
		"Crvena - SVETLA",
		"Zelena - TAMNA",
		"Zuta - TAMNA",
		"BIOS - Sta reci?",
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
	settings.theme = BIOS;
	//
	
	intiateColors(settings.theme);

	setSeed();
	resize_term(20, 100);

	while (stayInMenu)
	{
		
		clear();
		bkgd(COLOR_PAIR(INTERFACE));
		refresh();
		display2048(0, 20);
		switch (menu(mainMenu,0,0))
		{
			case 1: erase(); game(settings.mode, stayInMenu); break;
			case 2: switch (menu(modesMenu, 0, 0))
			{
				case 1:settings.mode = normal; break;
				case 2:settings.mode = xtile; break;
				case 3:settings.mode = autoplay; break;
				case 4:settings.mode = autoplayx; break;
				case 5:settings.mode = speed; break;
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
						case 4:settings.theme = zuta; break;
						case 5:settings.theme = BIOS; break;
					}intiateColors(settings.theme); break;
					case 3:stayInMenu = 0; break;
			}
			}stayInMenu = 1; break;
			case 4:
				break;
			//IZLAZ
			case 6: return(0); break;
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
		displayMatrix(1, 1, *M);
	}
	if (moved)
	{
		_sleep(25);
		spawnNumber(M);
		displayMatrix(1, 1, *M);
	}
	return moved;
}

void swipeSpeed(matrix *M, int direction, unsigned int *score)
{
	int changes, moved, last_merged[5] = { 0 };
	changes = moved = moveStep(M, direction, last_merged, score);
	while (changes)
	{
		changes = moveStep(M, direction, last_merged, score);
		_sleep(75);
		displayMatrix(1, 1, *M);
	}
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
		displayMatrix(1, 1, *M);
	}
}

void DisplayHSC(entry *score_list, unsigned int entry_count)
{
	int i;
	resize_term(13, 60);
	box(stdscr, 0, 0);
	mvprintw(1, getmaxx(stdscr) / 6, "MESTO"); mvprintw(1, getmaxx(stdscr) * 3 / 7, "IME"); mvprintw(1, getmaxx(stdscr)*2/3, "REZULTAT");
	for (i = 0; i < (entry_count>10?10:entry_count); i++)
	{
		for (int i = 0; i < (entry_count>10 ? 10 : entry_count); i++)
		{
			mvprintw(i + 2, getmaxx(stdscr) / 6 + 2, "%d.", i + 1);
			mvprintw(i + 2, getmaxx(stdscr) *3 / 7, "%s", score_list[i].name);
			mvprintw(i + 2, getmaxx(stdscr) * 2 / 3 + 2, "%u", score_list[i].score);
		}
		refresh();
	}
}

void msgBox(int startx, char text[25])
{
	WINDOW *message;
	cbreak();
	int duzina = strlen(text);
	message = newwin(5, duzina+4, (getmaxy(stdscr)-5)/2, startx);
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

void game(enum rezim rezim, int stayInMenu)
{
	if (settings.size == 4) resize_term(15, 62);
	else resize_term(18, 75);
	box(stdscr, 0, 0);

	unsigned int score, entry_count, bit_check;
	
	char status;
	
	matrix *m = malloc(sizeof(matrix));
	if (settings.mode == normal)
	{
		switch (loadGame(m, &score, settings.size))
		{
		case 1:
			msgBox((getmaxx(stdscr)-23) / 2, "Savegame nije ispravan!");
			remove("savegame.dat");
			break;
		case 2:
			mvprintw((getmaxy(stdscr) - 5) / 2, (getmaxx(stdscr) - 23) / 2, "Da li zelite novu igru?");
			_Bool highlight = 0;
			_Bool stay = 1;
			while (stay)
			{
				if (highlight)
				{
					attron(A_REVERSE);
					mvprintw((getmaxy(stdscr) - 5) / 2 + 3, (getmaxx(stdscr) + 11) / 2, "DA");
					attroff(A_REVERSE);
					mvprintw((getmaxy(stdscr) - 5) / 2 + 3, (getmaxx(stdscr) - 11) / 2, "NE");
				}
				else
				{
					mvprintw((getmaxy(stdscr) - 5) / 2 + 3, (getmaxx(stdscr) + 11) / 2, "DA");
					attron(A_REVERSE);
					mvprintw((getmaxy(stdscr) - 5) / 2 + 3, (getmaxx(stdscr) - 11) / 2, "NE");
					attroff(A_REVERSE);
				}
				switch (getch())
				{
				case KEY_LEFT:case KEY_RIGHT: highlight = !highlight; break;
				case 10: stay = 0; break;
				}
			}
			clear(); refresh();
			switch (highlight)
			{
				case 0:msgBox((getmaxx(stdscr)-20) / 2, "Uspesan loadgame!"); break;
				case 1:*m = newMatrix(settings.size); score = 0; break;
			}
			break;
		}

	}
	else{ *m = newMatrix(settings.size); score = 0; }

	box(stdscr, 0, 0);
	
	entry *score_list = loadHsc(&entry_count, &bit_check);
	if (score_list != NULL && bit_check == 0)
	{
		;//Greska (fajl je izmenjen vam programa)
	}
	

	switch (rezim)
	{
	case xtile:
		{
			int x = randomInt(0, settings.size*settings.size-1);
			m->set[x / settings.size][x%settings.size] = 3;
		}
	case normal:
	{
		history hist = newHistory(5, m);
		state previous = getState(*m, 0);
		mvprintw(1, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "REZULTAT:");
		mvprintw(10, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni ESC za meni!");
		mvprintw(11, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni h za pomoc!");
		mvprintw(12, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni u za undo!");
		refresh();
		displayNumber(3, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), score);
		displayMatrix(1, 1, *m);
		while (stayInMenu)
		{
			int valid_move = 1;
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
					pushHistory(&hist, previous);
				previous = getState(*m, score);
				break;
				case KEY_ESC:
					stayInMenu = 0; 
					saveGame(*m, score);
				break;
				case 'u': 
					popHistory(&hist, &score);
					previous = getState(*m, score);
					displayMatrix(1, 1, *m); 
				break;
				case 'h':
					showHint(m, 8, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0)); 
					mvprintw(10, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni ESC za meni!");
					mvprintw(11, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni h za pomoc!");
					mvprintw(12, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni u za undo!");
					refresh();
				break;
				case 'x':if(settings.mode==xtile)xTo2048(m); break;
				case '2':if (settings.mode == normal)doubleDouble(m); score *= 2; break;
				case 'f':if (settings.mode == normal)freeTwo(m); break;
			}
			displayNumber(3, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), score);
			if (!checkGameOver(*m))
			{
				displayGameOver(3,0);
				getHsc(&score_list, &entry_count, score);
				saveHsc(score_list,entry_count);
				stayInMenu = 0;
				clear();
				DisplayHSC(score_list, entry_count);
				getch();
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
		mvprintw(10, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni ESC za meni!");
		mvprintw(1, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "REZULTAT:");
		displayNumber(3, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), score);
		displayMatrix(1, 1, *m);
		nodelay(stdscr, TRUE);
		while (stayInMenu)
		{
			displayNumber(3, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), score);
			if (getch() == KEY_ESC)stayInMenu = 0, nodelay(stdscr, FALSE);
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
				nodelay(stdscr, FALSE);
				msgBox((getmaxx(stdscr)-12) / 7, "Gotova igra!");
				break;
			}
		}
	break;
	case speed:
		nodelay(stdscr, TRUE);
		clock_t start=clock(), end;
		while (stayInMenu)
		{
			mvprintw(10, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "Pritisni ESC za meni!");
			mvprintw(1, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), "REZULTAT:");
			displayNumber(3, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), score);
			displayMatrix(1, 1, *m);
			switch (getch())
			{
			case KEY_LEFT:
				swipeSpeed(m, LEFT, &score);
				break;
			case KEY_RIGHT:
				swipeSpeed(m, RIGHT, &score);
				break;
			case KEY_UP:
				swipeSpeed(m, UP, &score);
				break;
			case KEY_DOWN:
				swipeSpeed(m, DOWN, &score);
				break;
			case KEY_ESC:stayInMenu = 0; break;
			}
			end = clock();
			if (((double)(end - start) / CLOCKS_PER_SEC) > 0.5)
			{
				start = clock();
				spawnNumber(m);
			}
			if (checkFull(*m) || !checkGameOver(*m))
			{
				displayGameOver(3, 0);
				getHsc(&score_list, &entry_count, score);
				saveHsc(score_list, entry_count);
				stayInMenu = 0;
				erase();
				DisplayHSC(score_list, entry_count);
				nodelay(stdscr, FALSE);
				getch();
			}
		}
		break;
	}
	freeMatrix(m);
	free(score_list);
	erase();
	resize_term(20, 100);
	bkgd(COLOR_PAIR(INTERFACE)); refresh();
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
}

void getHsc(entry  **score_list, unsigned int *entry_count, unsigned int score)
{
	WINDOW *highscore;
	entry  player;
	char playerName[50];
	highscore = newwin(10, 25, 1, 25);
	echo();
	wattron(highscore, COLOR_PAIR(INTERFACE));
	wbkgd(highscore, COLOR_PAIR(INTERFACE));
	box(highscore, 0, 0);
	mvwprintw(highscore, 1, 5, "UNOS REZULTATA");
	mvwprintw(highscore, 3, 2, "REZULTAT: %u", score);
	mvwprintw(highscore, 5, 2, "IME:");
	mvwscanw(highscore, 5, 6, "%s", &playerName);
	mvwprintw(highscore, 7, 2, " Pritisnite bilo koje");
	mvwprintw(highscore, 8, 2, " dugme za nastavak!");
	wattroff(highscore, COLOR_PAIR(INTERFACE));
	wrefresh(highscore);
	noecho();
	player = newEntry(playerName, score);
	addEntry(score_list, entry_count, player);
	getch();
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