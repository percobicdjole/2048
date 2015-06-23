#include "graphics.h"
#include "logic.h"
#include <string.h>

unsigned char colorOfNumber(unsigned short x)
{
	if (x < 4)					    return FIRST;
	else if (x >= 4 && x <= 16)     return SECOND;
	else if (x >= 32 && x <= 64)    return THIRD;
	else if (x >= 128 && x <= 256)  return FOURTH;
	else if (x >= 512)				return FIFTH;
	else return 0;
}

void displayMatrix(int starty, int startx, matrix m)
{
	WINDOW *matrica = newwin(m.size*HEIGHT, m.size*WIDTH, starty, startx);
	for (int i = 0; i < m.size; i++)
	{
		for (int j = 0; j < m.size; j++)
		{	
			if (m.set[i][j])
			{
				unsigned char boja = colorOfNumber(m.set[i][j]);
				wattron(matrica, COLOR_PAIR(boja));
				if (m.set[i][j] == 3)wattron(matrica, getattrs(matrica) | A_REVERSE);

				//FARBA PLOCICU
				for (int k = i*HEIGHT; k < (i+1) * HEIGHT; k++)
				{
					for (int l = j*WIDTH; l < (j+1) * WIDTH; l++)mvwprintw(matrica, k, l, " ");
				}
				//

				//ISPISUJE BROJ ILI X
				if (m.set[i][j] == 3) mvwprintw(matrica, i * HEIGHT + HEIGHT/2, j * WIDTH + WIDTH/2, "X");
				else
				{
					int x = j * WIDTH + WIDTH / 2;
					if (m.set[i][j] >= 100 && m.set[i][j] < 1000)			x -= 1;
					else if (m.set[i][j] >= 1000)							x -= 2;
					mvwprintw(matrica, i * HEIGHT + HEIGHT / 2, x, "%d", m.set[i][j]);
				}
				//
				wattroff(matrica, getattrs(matrica));
			}
			else
			{
				wattron(matrica, COLOR_PAIR(INTERFACE));
				for (int k = i*HEIGHT; k < (i + 1) * HEIGHT; k++)
				{
					for (int l = j*WIDTH; l < (j + 1) * WIDTH; l++)mvwprintw(matrica, k, l, " ");
				}
				mvwprintw(matrica, i * HEIGHT + HEIGHT / 2, j * WIDTH + WIDTH / 2, ".");
				wattroff(matrica, COLOR_PAIR(INTERFACE));
			}
		}
	}
	wrefresh(matrica);
	delwin(matrica);
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

void initThemes()
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

	strcpy(crvena.name, "crvena");
	
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

	strcpy(plava.name, "plava");

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

	strcpy(zelena.name, "zelena");

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

	strcpy(zuta.name, "zuta");

	//bios
	BIOS.first.color = 6;   //2	
	BIOS.first.contrast = 14;

	BIOS.second.color = 1;  //4-16
	BIOS.second.contrast = 6;

	BIOS.third.color = 4;   //32-64
	BIOS.third.contrast = 14;

	BIOS.fourth.color = 14; //128-256
	BIOS.fourth.contrast = 12;

	BIOS.fifth.color = 11;  //512-2048
	BIOS.fifth.contrast = 12;

	BIOS.interface.color = 14;
	BIOS.interface.contrast = 9;

	strcpy(BIOS.name, "BIOS");
}

void setTheme(theme tema)
{
	init_pair(BW, COLOR_BLACK, WHITE);
	init_pair(FIRST, tema.first.contrast, tema.first.color);
	init_pair(SECOND, tema.second.contrast, tema.second.color);
	init_pair(THIRD, tema.third.contrast, tema.third.color);
	init_pair(FOURTH, tema.fourth.contrast, tema.fourth.color);
	init_pair(FIFTH, tema.fifth.contrast, tema.fifth.color);
	init_pair(INTERFACE, tema.interface.color, tema.interface.contrast);//za tekst
	init_pair(DIGITAL, tema.interface.color, tema.interface.color);//za digitalni displej
	init_pair(ERROR, 14, 12);//za error
}

void displayNumber(int starty, int startx, int number)
{	
	int endx = 30;
	WINDOW *score;
	score = newwin(5, 30, starty, startx);
	werase(score);
	wbkgd(score, COLOR_PAIR(INTERFACE));
	int space = -5;
	wattron(score,COLOR_PAIR(DIGITAL));
	if (number == 0)
	{
		mvwprintw(score, 0, endx + space, "xxxx");
		for (int i = 1; i <= 3; i++)mvwprintw(score, i, endx + space, "x");
		for (int i = 1; i <= 3; i++)mvwprintw(score, i, 3 + endx + space, "x");
		mvwprintw(score, 4, endx + space, "xxxx");
	}
	else
	{
		while (number)
		{
			switch (number % 10)
			{
			case 0:
				mvwprintw(score, 0, endx + space, "xxxx");
				for (int i = 1; i <= 3; i++)mvwprintw(score, i, endx + space, "x");
				for (int i = 1; i <= 3; i++)mvwprintw(score, i, 3 + endx + space, "x");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			case 1:
				mvwprintw(score, 0, endx + space, "xxx");
				for (int i = 1; i <= 3; i++)mvwprintw(score, i, 1 + endx + space, "xx");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			case 2:
				mvwprintw(score, 0, endx + space, "xxxx");
				mvwprintw(score, 1, 3 + endx + space, "x");
				mvwprintw(score, 2, endx + space, "xxxx");
				mvwprintw(score, 3, endx + space, "x");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			case 3:
				mvwprintw(score, 0, endx + space, "xxxx");
				mvwprintw(score, 1, 3 + endx + space, "x");
				mvwprintw(score, 2, endx + space, "xxxx");
				mvwprintw(score, 3, 3 + endx + space, "x");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			case 4:
				for (int i = 0; i <= 4; i++)mvwprintw(score, i, 3 + endx + space, "x");
				for (int i = 0; i <= 1; i++)mvwprintw(score, i, endx + space, "x");
				mvwprintw(score, 2, endx + space, "xxx");
				break;
			case 5:
				mvwprintw(score, 0, endx + space, "xxxx");
				mvwprintw(score, 1, endx + space, "x");
				mvwprintw(score, 2, endx + space, "xxxx");
				mvwprintw(score, 3, 3 + endx + space, "x");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			case 6:
				mvwprintw(score, 0, endx + space, "xxxx");
				mvwprintw(score, 1, endx + space, "x");
				mvwprintw(score, 2, endx + space, "xxxx");
				mvwprintw(score, 3, endx + space, "x");
				mvwprintw(score, 3, 3 + endx + space, "x");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			case 7:
				mvwprintw(score, 0, endx + space, "xxxx");
				mvwprintw(score, 1, 3 + endx + space, "x");
				mvwprintw(score, 2, 1 + endx + space, "xxx");
				mvwprintw(score, 3, 2 + endx + space, "x");
				mvwprintw(score, 4, 2 + endx + space, "x");
				break;
			case 8:
				mvwprintw(score, 0, endx + space, "xxxx");
				mvwprintw(score, 1, endx + space, "x");
				mvwprintw(score, 1, 3 + endx + space, "x");
				mvwprintw(score, 2, endx + space, "xxxx");
				mvwprintw(score, 3, endx + space, "x");
				mvwprintw(score, 3, 3 + endx + space, "x");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			case 9:
				mvwprintw(score, 0, endx + space, "xxxx");
				mvwprintw(score, 1, endx + space, "x");
				mvwprintw(score, 1, 3 + endx + space, "x");
				mvwprintw(score, 2, endx + space, "xxxx");
				mvwprintw(score, 3, 3 + endx + space, "x");
				mvwprintw(score, 4, endx + space, "xxxx");
				break;
			}
			number /= 10;
			space -= 5;
		}
	}
	wrefresh(score);
	wattroff(score,DIGITAL);
	delwin(score);
}

void display2048(int starty, int startx)
{
	mvprintw(0 + starty, startx, ".----------------.  .----------------.  .----------------.  .----------------. ");
	mvprintw(1 + starty, startx, "| .--------------. || .--------------. || .--------------. || .--------------. |");
	mvprintw(2 + starty, startx, "| |    _____     | || |     ____     | || |   _    _     | || |     ____     | |");
	mvprintw(3 + starty, startx, "| |   / ___ `.   | || |   .'    '.   | || |  | |  | |    | || |   .' __ '.   | |");
	mvprintw(4 + starty, startx, "| |  |_/___) |   | || |  |  .--.  |  | || |  | |__| |_   | || |   | (__) |   | |");
	mvprintw(5 + starty, startx, "| |   .'____.'   | || |  | |    | |  | || |  |____   _|  | || |   .`____'.   | |");
	mvprintw(6 + starty, startx, "| |  / /____     | || |  |  `--'  |  | || |      _| |_   | || |  | (____) |  | |");
	mvprintw(7 + starty, startx, "| |  |_______|   | || |   '.____.'   | || |     |_____|  | || |  `.______.'  | |");
	mvprintw(8 + starty, startx, "| |              | || |              | || |              | || |              | |");
	mvprintw(9 + starty, startx, "| '--------------' || '--------------' || '--------------' || '--------------' |");
	mvprintw(10 + starty, startx, " '----------------'  '----------------'  '----------------'  '----------------'");
	attron(COLOR_PAIR(INTERFACE) | A_REVERSE);
	mvprintw(12 + starty, 10 + startx, "Autori: Ivan Blazic - logika");
	mvprintw(14 + starty, 25 + startx, "Jovan Malovic - grafika");
	mvprintw(16 + starty, 35 + startx, "Djordje Percobic - AI");
	attroff(COLOR_PAIR(INTERFACE) | A_REVERSE);
	mvprintw(18 + starty, 67 + startx, "V 1.0 (2015)");
	refresh();
}

void displayGameOver(int starty, int startx)
{
	resize_term(12, 81);
	erase();
	mvprintw(0 + starty, startx, "___oooo______ooo____ooo_____ooo_ooooooo_______oooo____oo____oo_ooooooo_ooooooo___");
	mvprintw(1 + starty, startx, "_oo____oo__oo___oo__oooo___oooo_oo__________oo____oo__oo____oo_oo______oo____oo__");
	mvprintw(2 + starty, startx, "oo________oo_____oo_oo_oo_oo_oo_oooo_______oo______oo_oo____oo_oooo____oo____oo__");
	mvprintw(3 + starty, startx, "oo____ooo_ooooooooo_oo__ooo__oo_oo_________oo______oo__oo__oo__oo______ooooooo___");
	mvprintw(4 + starty, startx, "_oo____oo_oo_____oo_oo_______oo_oo__________oo____oo____oooo___oo______oo____oo__");
	mvprintw(5 + starty, startx, "___oooo___oo_____oo_oo_______oo_ooooooo_______oooo_______oo____ooooooo_oo_____oo_");
	refresh();
	_sleep(1000);
}

void splashScreen()
{
	init_pair(7, 15, 4);
	attron(COLOR_PAIR(7));
	bkgd(COLOR_PAIR(7));
	mvprintw(0, 0, ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(1, 0, ",,,,,,*/(%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(2, 0, ",,,,,/@@@@@@@@@@@@@@@@&%#((//***,,,,,,,,,**//(%&@@@@@@@@@@,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(3, 0, ",,,,,,&@@@@@/,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,#@@@,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(4, 0, ",,,,,,*@@@@@@,,,,,,,,,,,,**/(#&&@@@@@@@@@@@@@@@#,,,,,#@@@,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(5, 0, ",,,,,,,#@@@@@&,,,,,,,,,,,&@@@@@@@@@@@@@@@@@@@@@@&,,,,,,&@@@,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(6, 0, ",,,,,,,,@@@@@@/,,,,,,,,,,,@@@@@@&(#@@@@%,,,,,,,,,,,,,,,,*#@@@,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(7, 0, ",,,,,,,,/@@@@@@,,,,,,,,,,,,,,,,,,,,/&@@@@@@@@@&(,,,,,,,,,,#@@,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(8, 0, ",,,,,,,,,&@@@@@%,,,,,,,,,,,,,,,,(@@@@@@@@@@@@@@@@@@(,,,,,,,@@@@,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(9, 0, ",,,,,,,,,*@@@@@@*,,,,,,,,,,,,,(@@@@@&*/@@@@@&,,,*#@@@@/,,,,,%@@*,,,,,,,,,,,,,,,,,,,,");
	mvprintw(10, 0, ",,,,,,,,,,%@@@@@&,,,,,,,,,,,,@@@@@/,,,,#@@@@@#,,,,,,(@@@/,,,,%@@/,,,,,,,,,,,,,,,,,,,");
	mvprintw(11, 0, ",,,,,,,,,,*@@@@@@(,,,,,,,,,,@@@@&,,,,,,,&@@@@@/,,,,,,,/@@@*,,,%@@(,,,,,,,,,,,,,,,,,,");
	mvprintw(12, 0, ",,,,,,,,,,,#@@@@@@,,,,,,,,,%@@@&,,,,,,,,*@@@@@@*,,,,,,,,&@@/,,,&@@/,,,,,,,,,,,,,,,,,");
	mvprintw(13, 0, ",,,,,,,,,,,*@@@@@@#,,,,,,,,@@@@*,,,,,,,,,(@@@@@&,,,,**(#&@@@@@@@@@@*,,,,,,,,,,,,,,,,");
	mvprintw(14, 0, ",,,,,,,,,,,,#@@@@@@*,,,,,,*@@@@,,,,,,,,,,,&@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,");
	mvprintw(15, 0, ",,,,,,,,,,,,,@@@@@@%,,,,,,*@@@@*/(%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&,,,,,,,,,,,,,,,");
	mvprintw(16, 0, ",,,,,,,,,,,,,#@@@@@@*,,,*(%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%@@/,,,&@@#,,,,,,,,,,,,,,,,");
	mvprintw(17, 0, ",,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@(,,,,,,,,/@@#,,,*@@@*,,,,,,,,,,,,,");
	mvprintw(18, 0, ",,,,,,,,,,,,,,#@@@@@@@@@@@@@@@@@@@@@@@@%(*,,(@@@@@&,,,,,,,,(@@#,,,,#@@&,,,,,,,,,,,,,");
	mvprintw(19, 0, ",,,,,,,,,,,,,,*@@@@@@@@@@@@@&@@@@*,,,,,,,,,,,@@@@@@/,,,,,,,@@@/,,,,,@@@*,,,,,,,,,,,,");
	mvprintw(20, 0, ",,,,,,,,,,,,,,,#@@@@@@*,,,,,,#@@@@/,,,,,,,,,,(@@@@@@,,,,,,%@@@,,,,,,((@,,,,,,,,,,,,,");
	mvprintw(21, 0, ",,,,,,,,,,,,,,,*@@@@@@%,,,,,,,(@@@@&,,,,,,,,,,@@@@@@/,,,,&@@@*,,,,,,,(@*,,,,,,,,,,,,");
	mvprintw(22, 0, ",,,,,,,,,,,,,,,,%@@@@@@*,,,,,,,*@@@@@&*,,,,,,,(@@@@@&,,(@@@@*,,,,,,,,,@*,,,,,,,,,,,,");
	mvprintw(23, 0, ",,,,,,,,,,,,,,,,*@@@@@@#,,,,,,,,,/@@@@@@&(,,,,,@@@@@@@@@@@&,,,,,,,,,,,@@(,,,,,,,,,,,");
	mvprintw(24, 0, ",,,,,,,,,,,,,,,,,%@@@@@@,,,,,,,,,,,*&@@@@@@@@@@@@@@@@@@@&,,,,,,,,,,,,*@@@,,,,,,,,,,,");
	mvprintw(25, 0, ",,,,,,,,,,,,,,,,,*@@@@@@(,,,,,,,,,,,,,*%@@@@@@@@@@@@@&*,,,,,*#%,,,,,,,@@@*,,,,,,,,,,");
	mvprintw(26, 0, ",,,,,,,,,,,,,,,,,,&@@@@@@,,,,,,,,,,,,,,,,,,,,***#@@@@@/%@@@@@@@,,,,,,,&@@/,,,,,,,,,,");
	mvprintw(27, 0, ",,,,,,,,,,,,,,,,,,/@@@@@@/,,,,,,,,,,,,,,,,,,,,*/%@@@@@@@@@@@@@@/,,,,,,#@@(,,,,,,,,,,");
	mvprintw(28, 0, ",,,,,,,,,,,,,,,,,,,@@@@@@&,,,,,,,,,,,,,*(%@@@@@@@@@@@@@@@@@@*,,,,,,,(@@#,,,,,,,,,,,,");
	mvprintw(29, 0, ",,,,,,,,,,,,,,,,,,,(@@@@@@*,,,,,,,,,,,#@@@@@@@@@@@@@@@@#*,,,,,,,,,,,*#@@@(,,,,,,,,,,");
	mvprintw(30, 0, ",,,,,,,,,,,,,,,,,,,,@@@@@@%,,,,,,,,,,,*@@@@@@@@@&(*,,,,,,,,,,,,,/&@@@@@@@/,,,,,,,,,,");
	mvprintw(31, 0, ",,,,,,,,,,,,,,,,,,,,%@@@@@@,,,,,,,,,,,,@@*,,,,,,,,,,,,,,,(&@@@@@@@@@&/,,,,,,,,,,,,,,");
	mvprintw(32, 0, ",,,,,,,,,,,,,,,,,,,,*@@@@@@(,,,,,,,,,,,,,,,,,,,,,,,,,/%@@@@@@@@@@@@(,,,,,,,,,,,,,,,,");
	mvprintw(33, 0, ",,,,,,,,,,,,,,,,,,,,,&@@@@@&,,,,,,,,,,,,,,,,,,,/%@@@@@@@@@@@@@&/,,,,,,,,,,,,,,,,,,,,");
	mvprintw(34, 0, ",,,,,,,,,,,,,,,,,,,,,/@@@@@@*,,,,,,,,,,,*#&@@@@@@@@@@@@@@&/,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(35, 0, ",,,,,,,,,,,,,,,,,,,,,,@@@@@@(,,,,*#&@@@@@@@@@@@@@@@@%*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(36, 0, ",,,,,,,,,,,,,,,,,,,,,,#@@@@@@@@@@@@@@@@@@@@@@@%/,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(37, 0, ",,,,,,,,,,,,,,,,,,,,,,*@@@@@@@@@@@@@@@@&(*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(38, 0, ",,,,,,,,,,,,,,,,,,,,,,,&@@@@@@@@&(*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");
	mvprintw(39, 0, ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,");

	attroff(COLOR_PAIR(7));
	refresh();
	_sleep(2000);
	erase();
	refresh();
}

void uputstvo()
{
	resize_term(12, 75);
	mvprintw(1, 1, "O igri:");
	mvprintw(2, 1, "2048 se igra na tabli 4x4 ili 5x5 polja koja sadrze brojeve.");
	mvprintw(3, 1, "Igrac pomera polja u zeljenu stranu izborom strelice na tastaturi.");
	mvprintw(4, 1, "Nakon poteza igraca stvara se novo polje sa vrednoscu 2 ili 4 na");
	mvprintw(5, 1, "nasumicnom praznom polju. Polja se pomeraju na jednu stranu dok");
	mvprintw(6, 1, "ih ne zaustavi polje sa razlicitom vrednoscu ili ne dodju do kraja");
	mvprintw(7, 1, "table. Ako se dva polja sa istom vrednoscu sudare prilikom pomeranja");
	mvprintw(8, 1, "doci ce do njihovog spajanja u polje koje predstavlja njihov zbir.");
	mvprintw(10, 1, "Za nastavak pritisnite bilo koje dugme...");
	refresh();
	getch();
	clear();
	mvprintw(1, 1, "Rezimi:");
	mvprintw(2, 1, "-Klasican");
	mvprintw(3, 1, "-X Polje - nije spojivo ni sa jednim drugim poljem");
	mvprintw(4, 1, "-Brzopotezno - novo polje svake sekunde, ne smeju da se popune sva polja");
	mvprintw(5, 1, "-Automatski - posmatrate AI kako igra");
	mvprintw(10, 1, "Za nastavak pritisnite bilo koje dugme...");
	refresh();
	getch();
	clear();
}

