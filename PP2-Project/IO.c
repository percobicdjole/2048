#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IO.h"

void cdcEntry(entry *E)
{
	char *s = E->name;
	while (*s)
	{
		*s ^= CHAR_MASK;
		s++;
	}
	if (E)
	{
		E->score ^= INT_MASK;
	}
	//else
		//Poruka o greski
}

void cdcInfo(file_info *inf)
{
	inf->bit_count ^= INT_MASK;
	inf->entry_count ^= INT_MASK;
}

int countBits(unsigned int data)
{
	int count = 0;
	while (data)
	{
		count += data & 1;
		data >>= 1;
	}
	return count;
}

int bitCheck(entry E)
{
	int count = 0;
	char *s = E.name;
	while (*s)
	{
		count += countBits(*s);
		s++;
	}
	count += countBits(E.score);
	return count;
}

entry newEntry(char *player_name, unsigned int score)
{
	entry N;
	memcpy(N.name, player_name, sizeof(char)*(strlen(player_name) + 1));
	N.score = score;
	return N;
}

void addEntry(entry  **score_list, unsigned int *entry_count, entry newScore)
{
	(*entry_count)++;
	if (score_list)
	{
		unsigned int i = 0, mem;
		entry *P = *score_list;
		*score_list = realloc(*score_list, sizeof(entry)*(*entry_count));
		checkMemError(score_list);
		while (P[i].score > newScore.score  && i<entry_count)
			i++;
		P += i;
		mem = (*entry_count - (i + 1))*(sizeof(entry));
		memmove(P + 1, P, mem);
		*P = newScore;
	}
	else
	{
		*score_list = malloc(sizeof(entry));
		checkMemError(score_list);
		*score_list[0] = newScore;
	}
}

void saveHsc(char *file_name, entry *score_list, unsigned int entry_count)
{
	int i;
	file_info inf;
	entry buffer;
	FILE *hsc_file = fopen(file_name, "wb");
	fseek(hsc_file, sizeof(inf), SEEK_SET);
	inf.bit_count = 0;
	inf.entry_count = entry_count;
	for (i = 0; i < entry_count; i++)
	{
		buffer = score_list[i];
		inf.bit_count += bitCheck(buffer);
		cdcEntry(&buffer);
		fwrite(&buffer, sizeof(buffer), 1, hsc_file);
	}
	rewind(hsc_file);
	cdcInfo(&inf);
	fwrite(&inf, sizeof(inf), 1, hsc_file);
	fclose(hsc_file);
}

entry *loadHsc(char *file_name, unsigned int *entry_conunt, unsigned int *bit_check)
{
	FILE *hsc_file = fopen(file_name, "rb");
	entry *score_list = NULL;
	if (hsc_file)
	{
		entry buffer;
		file_info inf;
		unsigned int readBits = 0, i;
		fread(&inf, sizeof(inf), 1, hsc_file);
		cdcInfo(&inf);
		*entry_conunt = inf.entry_count;
		if (inf.entry_count)
		{
			score_list = malloc(sizeof(entry)*inf.entry_count);
			checkMemError(score_list);
			for (i = 0; i < inf.entry_count; i++)
			{
				fread(&buffer, sizeof(buffer), 1, hsc_file);
				cdcEntry(&buffer);
				readBits += bitCheck(buffer);
				score_list[i] = buffer;
			}
		}
		*bit_check = (inf.bit_count == readBits);
		fclose(hsc_file);
	}
	return score_list;
}

void saveGame(matrix M, unsigned int score)
{
	int bit_count = 0, i, j, buffer;
	char size = M.size ^ CHAR_MASK;
	FILE *svg = fopen("savegame.dat", "wb");
	checkFileError(svg);
	fseek(svg, sizeof(bit_count), SEEK_SET);
	bit_count = countBits(score) + countBits(M.size);
	score ^= INT_MASK;
	fwrite(&score, sizeof(score), 1, svg);
	fwrite(&size, sizeof(size), 1, svg);
	for (i = 0; i < M.size; i++)
		for (j = 0; j < M.size; j++)
		{
			buffer = M.set[i][j];
			bit_count += countBits(buffer);
			buffer ^= INT_MASK;
			fwrite(&buffer, sizeof(buffer), 1, svg);
		}
	rewind(svg);
	bit_count ^= INT_MASK;
	fwrite(&bit_count, sizeof(bit_count), 1, svg);
	fclose(svg);
}

matrix loadGame(unsigned int *score, char *status)
{
	FILE *svg = fopen("savegame.dat", "rb");
	if (svg)
	{
		matrix N;
		int i,j, **M;
		int expectBits, readBits, buffer;
		readBits = 0;
		fread(&expectBits, sizeof(expectBits), 1, svg);
		expectBits ^= INT_MASK;
		fread(score, sizeof(*score), 1, svg);
		*score ^= INT_MASK;
		readBits += countBits(*score);
		fread(&N.size, sizeof(N.size), 1, svg);
		N.size ^= CHAR_MASK;
		readBits += countBits(N.size);
		**M = malloc(N.size*sizeof(int*));
		checkMemError(M);
		for (i = 0; i < N.size; i++)
		{
			M[i] = calloc(N.size, sizeof(int));
			checkMemError(M[i]);
			for (j = 0; j < N.size; j++)
			{
				fread(&buffer, sizeof(buffer), 1, svg);
				buffer ^= INT_MASK;
				readBits += countBits(buffer);
				M[i][j] = buffer;
			}
		}
		N.set = M;
		*status = (readBits == expectBits);
		return N;
	}
	else
	{
		*status = 0;
		*score = 0;
		return newMatrix(4);
	}
}

void checkMemError(void *new_pointer)
{
	if (new_pointer == NULL)
	{
		//Poruka o greski
		exit(1);
	}
}

void checkFileError(FILE *file_pointer)
{
	if (file_pointer == NULL)
	{
		//Poruka o greski
		exit(1);
	}
}