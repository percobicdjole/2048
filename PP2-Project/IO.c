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
	E->score ^= INT_MASK;
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