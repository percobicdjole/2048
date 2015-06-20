#include "graphics.h"
#include "menu.h"
#include "logic.h"
#include "IO.h"
#include "ai.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void game(enum modes rezim, int stayInMenu);

int swipe(matrix *M, int direction, unsigned int *score);
void swipeNoAnimation(matrix *M, int direction, unsigned int *score);
void showHint(matrix *m, int starty, int startx);
void getHsc(entry  **score_list, unsigned int *entry_count, unsigned int score);
void msgBox(int startx, char text[25]);
void displayHSC(entry *score_list, unsigned int entry_count);

void executeCheat(int code, int *score, matrix *m);
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
		"Automatski",
		"Automatski X",
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
		"Plava-SVETLA",
		"Crvena-SVETLA",
		"Zelena-TAMNA",
		"Zuta-TAMNA",
		"BIOS",
		NULL
	};

	_Bool stayInMenu = 1;
	
	
	//CURSES INCIJALIZACIJA
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	initThemes();
	resize_term(40, 80);
		
	splashScreen();  //ETF SPLASH SCREEN
	
	//Podrazumevana settings
	settings.mode = normal;
	settings.size = 4;
	settings.theme = BIOS;
	//
	
	setTheme(settings.theme);

	setSeed();
	resize_term(20, 100);
	
	clear();
	bkgd(COLOR_PAIR(INTERFACE));
	display2048(0, 20);
	refresh();

	while (stayInMenu)
	{

		switch (menu(mainMenu,0,0))
		{
			case 1: erase(); 
					game(settings.mode, stayInMenu); 
					erase();
					bkgd(COLOR_PAIR(INTERFACE));
					display2048(0, 20);
					refresh();
			break;
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
					case 1: 
						switch (menu(sizeMenu, 0, 0))
						{
							case 1:settings.size = 4; break;
							case 2:settings.size = 5; break;
						}
					break;
					case 2: switch (menu(themesMenu, 0, 0))
					{
						case 1:settings.theme = plava; break;
						case 2:settings.theme = crvena; break;
						case 3:settings.theme = zelena; break;
						case 4:settings.theme = zuta; break;
						case 5:settings.theme = BIOS; break;
					}
						setTheme(settings.theme);
					break;
					case 3:stayInMenu = 0; break;
				}
			}stayInMenu = 1; break;
			case 4:
				break;
			case 5:
			{
				unsigned int score, entry_count, bit_check;
				entry *score_list = loadHsc(&entry_count, &bit_check);
				if (score_list != NULL && bit_check == 0)
				{
					;//Greska (fajl je izmenjen vam programa)
				}
				else
				{
					displayHSC(score_list, entry_count);
					getch();
					resize_term(20, 100);
					erase();
					refresh();
				}
				display2048(0, 20);
			}
			break;
			case 6: return(0); break;
		}
		stayInMenu = 1;
	}
	endwin();
}

//DA SE IZBACI IZ MAIN-A
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
//DA SE IZBACI IZ MAIN-A


void newGame(matrix *m, unsigned int *score)
{
	if (settings.size == 4) resize_term(15, 62);
	else resize_term(18, 75);
	box(stdscr, 0, 0);
	if (settings.mode == normal || settings.mode == xtile || settings.mode == speed)
	{
		int load = loadGame(m, score, &settings.mode, settings.size);
		switch (load)
		{
		case 1:
			msgBox((getmaxx(stdscr) - 23) / 2, "Savegame nije ispravan!");
			remove("savegame.dat");
			break;
		case 2:
			mvprintw((getmaxy(stdscr) - 5) / 2, (getmaxx(stdscr) - 23) / 2, "Da li zelite novu igru?");
			_Bool highlight = 0, stay = 1;
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
			clear(); 
			box(stdscr, 0, 0);
			refresh();
			switch (highlight)
			{
				case 0:
					msgBox((getmaxx(stdscr) - 20) / 2, "Uspesan loadgame!"); 
					if (m->size == 4) resize_term(15, 62);
					else resize_term(18, 75);
				break;
				case 1:*m = newMatrix(settings.size); *score = 0; break;
			}
			break;
		}

	}
	//Ukoliko je autoplay onda se vraca prazna matrica
	else{ *m = newMatrix(settings.size); *score = 0; }
}


void game(enum rezim rezim, int stayInMenu)
{
	unsigned int score, entry_count, bit_check, mode = rezim;
	int code, c, prev_code;
	char *cheats[] = { "leavemealone","abrakadabra","jeltozelite" ,"robot",NULL};
	char buffer[20] = "";
	matrix *m = malloc(sizeof(matrix));

	newGame(m, &score);
	
	entry *score_list = loadHsc(&entry_count, &bit_check);
	if (score_list != NULL && bit_check == 0)
	{
		;//Greska (fajl je izmenjen vam programa)
	}
	
	switch (rezim)
	{
	case xtile:
		if (score==0)//DA NE BI POSTAVLJAO X-TILE KAD SE LOADUJE
		{
			int x = randomInt(0, settings.size*settings.size - 1);
			m->set[x / settings.size][x%settings.size] = 3;
		}
	case normal:
	{
		history hist = newHistory(5, m);
		state previous = getState(*m, 0);
		prev_code = code = -1;

		mvprintw(1, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "REZULTAT:");
		mvprintw(10, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni ESC za meni!");
		mvprintw(11, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni h za pomoc!");
		mvprintw(12, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni CTRL + Z za undo!");
		refresh();
		displayNumber(3, 4 * WIDTH + 2 + (settings.size == 5 ? 10 : 0), score);
		displayMatrix(1, 1, *m);
		while (stayInMenu)
		{
			int valid_move = 1;
			c = getch();
			switch (c)
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
				if (score!=0) saveGame(*m, score, mode);
				break;
			case CTRL_Z:
					popHistory(&hist, &score);
					previous = getState(*m, score);
					displayMatrix(1, 1, *m); 
				break;
			case 'h':
					showHint(m, 9, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0));
					bkgd(COLOR_PAIR(INTERFACE));
					mvprintw(10, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni ESC za meni!");
					mvprintw(11, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni h za pomoc!");
					mvprintw(12, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni CTRL + Z za undo!");
					refresh();
					displayMatrix(1, 1, *m);
				break;
				
			}
			displayNumber(3, 4 * WIDTH + 2 + (m->size == 5 ? 10 : 0), score);
			
			if (!checkGameOver(*m))
			{
				displayGameOver(3, 0);
				getHsc(&score_list, &entry_count, score);
				saveHsc(score_list, entry_count);
				stayInMenu = 0;
				clear();
				displayHSC(score_list, entry_count);
				getch();
			}

			if (isalpha((char)c))
			{
				strcat(buffer, &c);
				code = findCode(cheats, buffer, prev_code);
				if (code == -1)
					strcpy(buffer, "");
				if (code == -2)
				{
					strcpy(buffer, "");
					executeCheat(prev_code, &score, m);
				}
				prev_code = code;
			}
		}
		freeState(&previous, m->size);
		destroyHistory(&hist);
	}break;
	
	case autoplayx:
	{
		int x = randomInt(0, settings.size*settings.size - 1);
		m->set[x / settings.size][x%settings.size] = 3;
	}
	case autoplay:
		mvprintw(10, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni ESC za meni!");
		mvprintw(1, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "REZULTAT:");
		refresh();
		displayNumber(3, 4 * WIDTH + 2 + (m->size == 5 ? 10 : 0), score);
		displayMatrix(1, 1, *m);
		nodelay(stdscr, TRUE);
		while (stayInMenu)
		{
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
				msgBox((getmaxx(stdscr)-16) / 7, "Gotova igra!");
				break;
			}
			displayNumber(3, 4 * WIDTH + 2 + (m->size == 5 ? 10 : 0), score);
			if (getch() == KEY_ESC)
			{
				stayInMenu = 0;
				nodelay(stdscr, FALSE);
			}
		}
		nodelay(stdscr, FALSE);
	break;
	
	case speed:
		nodelay(stdscr, TRUE);
		clock_t start=clock(), end;
		while (stayInMenu)
		{
			mvprintw(10, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "Pritisni ESC za meni!");
			mvprintw(1, 4 * WIDTH + 3 + (m->size == 5 ? 10 : 0), "REZULTAT:");
			displayNumber(3, 4 * WIDTH + 2 + (m->size == 5 ? 10 : 0), score);
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
			case KEY_ESC:
				stayInMenu = 0; 
				if(score!=0)saveGame(*m, score, mode);
			break;
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
				displayHSC(score_list, entry_count);
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

/*FUNKCIJE ZA HIGHSCORE*/

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
	
	//mvwscanw(highscore, 5, 6, "%s", &playerName); //DA MOGU DA BUDU IMENA IZ VISE DELOVA!
	
	mvwgetstr(highscore, 5, 6, &playerName);
	mvwprintw(highscore, 7, 2, " Pritisnite bilo koje");
	mvwprintw(highscore, 8, 2, " dugme za nastavak!");
	wattroff(highscore, COLOR_PAIR(INTERFACE));
	wrefresh(highscore);
	noecho();
	player = newEntry(playerName, score);
	addEntry(score_list, entry_count, player);
	getch();
}

void displayHSC(entry *score_list, unsigned int entry_count)
{
	int i;
	resize_term(13, 60);
	erase();
	refresh();
	box(stdscr, 0, 0);
	mvprintw(1, getmaxx(stdscr) / 6, "MESTO"); mvprintw(1, getmaxx(stdscr) * 3 / 7, "IME"); mvprintw(1, getmaxx(stdscr) * 2 / 3, "REZULTAT");
	for (i = 0; i < (entry_count>10 ? 10 : entry_count); i++)
	{
		for (int i = 0; i < (entry_count>10 ? 10 : entry_count); i++)
		{
			mvprintw(i + 2, getmaxx(stdscr) / 6 + 2, "%d.", i + 1);
			mvprintw(i + 2, getmaxx(stdscr) * 3 / 7, "%s", score_list[i].name);
			mvprintw(i + 2, getmaxx(stdscr) * 2 / 3 + 2, "%u", score_list[i].score);
		}
		refresh();
	}
}


/*FUNKCIJE ZA CHEAT-OVE*/
void xTo2048(matrix *m)
{
	int i, j;
	for (i = 0; i < settings.size; i++)
		for (j = 0; j < settings.size; j++)
		{
			if (m->set[i][j] == 3)
			{
				m->set[i][j] = 2048;
				return;
			}
		}
}

void doubleDouble(matrix *m)
{
	int i, j;
	for (i = 0; i < settings.size; i++)
		for (j = 0; j < settings.size; j++)
		{
			if (m->set[i][j]!=0)
				m->set[i][j] <<= 1;
		}
}

void freeTwo(matrix *m)
{
	int xmin, ymin, min,i,j,k;
	for (k = 0; k < 2; k++)
	{
		xmin = ymin = 0;
		min = 65536;
		for (i = 0; i < settings.size; i++)
			for (j = 0; j < settings.size; j++)
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

void timedAutoplay(matrix *m, unsigned int *score)
{
	clock_t start = clock(), end=0;
	while (((double)(end - start) / CLOCKS_PER_SEC) < 13)
	{
		switch (get_hint(*m))
		{
		case LEFT:
			swipeNoAnimation(m, LEFT, score);
			break;
		case RIGHT:
			swipeNoAnimation(m, RIGHT, score);
			break;
		case UP:
			swipeNoAnimation(m, UP, score);
			break;
		case DOWN:
			swipeNoAnimation(m, DOWN, score);
			break;
		}
		displayNumber(3, 4 * WIDTH + 2 + (m->size == 5 ? 10 : 0), *score);
		end = clock();
		if (!checkGameOver(*m))return;
	}
}

void executeCheat(int code, int *score, matrix *m)
{
	switch (code)
	{
	case 0:
		if (settings.mode == xtile)
			xTo2048(m);
		break;
	case 1:
		if (settings.mode == normal)
		{
			doubleDouble(m);
			if (*score == 0)
				(*score)++;
			*score *= 2;
		}
		break;
	case 2:
		if (settings.mode == normal)
			freeTwo(m);
		break;
	case 3:
		if (settings.mode == normal)
			timedAutoplay(m, score);
		break;
	}
}