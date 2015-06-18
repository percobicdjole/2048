#define INT_MASK 48621488
#define CHAR_MASK 61


//Highscire unos
typedef struct entry
{
	char name[50];
	unsigned int score;
}entry;

//Informacije o datoteci
typedef struct file_info
{
	unsigned int bit_count, entry_count;
}file_info;

//Novi unos
entry newEntry(char *player_name, unsigned int score);

//Dodaje novi unos u niz i odrzava poredak
void addEntry(entry  **score_list, unsigned int *entry_count, entry newScore);

//Snima highcsore iz niza u datoteku
void saveHsc(entry *score_list, unsigned int entry_count);

//Ucitava highscore iz datoteke u niz (NULL ako nema datoteke)
entry *loadHsc(unsigned int *entry_conunt, unsigned int *bit_check);

//Cuva stanje igre u unapred definisanu datoteku savegame.dat
void saveGame(matrix M, unsigned int score);

//Ucitava igru iz datoteke, vraca matricu ako je uspesno procitana
int loadGame(matrix *Mp, unsigned int *score, unsigned int new_size);

//Provera greske pri dodeli memorije
void checkMemError(void *new_pointer);

//Provera greske pri otvaranju datoteke
void checkFileError(FILE *file_pointer);

//Upisuje podatke o AI statistici
void writeAIstats(matrix M);

//Detektuje cheat za dati string
int findCode(char *cheats[], char *buffer, int prev_code);