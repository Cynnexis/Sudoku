#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

int kabxy2kcl(int i) {
	int k, a, b, x, y, c, l;
	y = i % 10;
	x = ((i - y) % 100) / 10;
	b = ((i - y - 10 * x) % 1000) / 100;
	a = ((i - b * 100 - 10* x - y) % 10000) / 1000;
	k = (i - a * 1000 - b * 100 - 10* x - y) / 10000;
	
	c = (a-1)*3+x;
	l = (b-1)*3+y;
	return k * 100 + c * 10 + l;
}

void afficherGrille(unsigned int tab[NMAX][NMAX]) {
	int x, y, z, t;
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
				printf("-");
			}
		}
		printf("\033[0m\n");
	}
}

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
	int k, c, l, a, b, x, y, i;
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
	
	for (i = 11111 ; i <= 93333 ; i++)
	{
		if (counter[i] != 0)
		{
			y = i % 10;
			x = ((i - y) % 100) / 10;
			b = ((i - y - 10 * x) % 1000) / 100;
			a = ((i - b * 100 - 10* x - y) % 10000) / 1000;
			k = (i - a * 1000 - b * 100 - 10* x - y) / 10000;
			
			fprintf(export, "%i%i%i%i%i ", k, a, b, x, y);
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
