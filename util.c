#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

void showGrid(unsigned int tab[NMAX][NMAX]) {
	int x, y, z;
	for (x = 0 ; x < NMAX ; x++)
	{
		for (y = 0 ; y < NMAX ; y++)
		{
			if (y % 3 == 0 && y != 0)
				printf("\033[31;1m|\033[0m");
			if (tab[x][y] == 0)
				printf("  ");
			else
				printf("%i ", tab[x][y]);
		}
		
		if (x % 3 == 2 && x != 0 && x != NMAX-1)
		{
			printf("\n\033[31;1m");
			for (z = 0 ; z < NMAX*2+1 ; z++)
			{
				if ((z+1) % 7 != 0)
					printf("-");
				else
					printf("+");
			}
		}
		printf("\033[0m\n");
	}
}

int seekKCL(int compteur) {
	int i;
	
	for (i = 111 ; counter[i] != compteur && i <= NVAR ; i++);
	
	return i;
}

/*int seekKCLinFile(int index) {
	int i, var;
	bool valid = false;
	FILE* import = fopen("export.tmp", "r");
	
	if (import == NULL)
	{
		perror("export.tmp");
		exit(-2);
	}
	
	while (!feof(import) && !valid)
	{
		fscanf(import, "%i %i\n", &var, &i);
		
		if (i == index)
			valid = true;
	}
	
	fclose(import);
	
	if (!valid)
		return 1000;
	
	return var;
}*/

// Line
void initLine() {
	_line = 0;
}

int useLine() {
	_line++;
	return _line;
}

int getLine() {
	return _line;
}

// Ghost
void initGhost(int nb_var) {
	_ghost = nb_var;
}

int useGhost() {
	_ghost++;
	return _ghost;
}

int getGhost() {
	return _ghost;
}

// Counter
int useCount() {
	_count++;
	return _count;
}

void initCounter() {
	_count = 0;
}

int useCounter(int position) {
	int count = _count;
	
	if (counter[position] == 0)
	{
		count = useCount();
		counter[position] = count;
		return count;
	}
	else
		return counter[position];
}

int getCount() {
	return _count;
}

void exportCounter() {
	int k, c, l, i;
	FILE* export = fopen("export.tmp", "w");
	
	if (export == NULL)
	{
		perror("export.tmp");
		exit(-2);
	}
	
	for (i = 111 ; i <= 999 ; i++)
	{
		l = i % 10;
		c = ((i - l) % 100)/10;
		k = (i - l - 10 * c) / 100;
		
		if (counter[i] != 0)
		{
			fprintf(export, "%i%i%i ", k, c, l);
			fprintf(export, "%i\n", counter[i]);
		}
	}
	
	fclose(export);
}

void importCounter() {
	int var, i;
	FILE* import = fopen("export.tmp", "r");
	
	if (import == NULL)
	{
		perror("export.tmp");
		exit(-2);
	}
	
	for (i = 0 ; i < NVAR ; i++)
		counter[i] = 0;
	
	while (!feof(import))
	{
		fscanf(import, "%i %i\n", &var, &i);
		
		//printf("%i recoit %i\n", var, i);
		counter[var] = i;
	}
}
