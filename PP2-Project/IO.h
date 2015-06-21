#ifndef IO_H
#define IO_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MASK 48621488
#define CHAR_MASK 61


//Highscire unos
typedef struct entry
{
	char name[20], mode[20];
	unsigned int score;
}entry;

//Informacije o datoteci
typedef struct file_info
{
	unsigned int bit_count, entry_count;
}file_info;

//Kreiranje novog unosa
entry newEntry(char *player_name, char *mode, unsigned int score);

//Dodaje novi unos u niz i odrzava poredak
void addEntry(entry  **score_list, unsigned int *entry_count, entry newScore);

//Snima highcsore iz niza u datoteku
void saveHsc(entry *score_list, unsigned int entry_count);

//Ucitava highscore iz datoteke u niz (NULL ako nema datoteke)
entry *loadHsc(unsigned int *entry_conunt, unsigned int *bit_check);

//Cuva stanje igre u unapred definisanu datoteku savegame.dat
void saveGame(matrix M, unsigned int score, enum mode mode);

//Ucitava igru iz datoteke, vraca matricu ako je uspesno procitana
int loadGame(matrix *Mp, unsigned int *score, enum mode *mode, unsigned int new_size);

//Provera greske pri dodeli memorije
void checkMemError(void *new_pointer);

//Provera greske pri otvaranju datoteke
void checkFileError(FILE *file_pointer);

//Upisuje podatke o AI statistici
void writeAIstats(matrix M);

//Detektuje cheat za dati string
int findCode(char *cheats[], char *buffer, int prev_code);

#endif /* IO_H */