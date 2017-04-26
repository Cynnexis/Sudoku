#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

// Table pour le sudoku
unsigned int grille[NMAX][NMAX];

int seekKCL(int compteur);
int seekKCLinFile(int index);

int main(int argc, char *argv[]) {
	FILE* f_solv;
	int number = 0, var = 0, k, x, y, i, j, a, b, c, l;
	char cc;
	
	if (argc != 2)
	{
		fprintf(stderr, "%s: Error: One argument is missing.\n", argv[0]);
		return -1;
	}
	
	f_solv = fopen(argv[1], "r");
	
	if (f_solv == NULL)
	{
		perror(argv[1]);
		return -2;
	}
	
	fscanf(f_solv, "%c ", &cc);
	
	if (cc == 'U')
	{
		printf("This set of clauses is unsatisfiable.\n");
		return EXIT_SUCCESS;
	}
	
	if (cc == 'S')
	{
		do {
			cc = fgetc(f_solv);
		} while (cc != '\n');
	}
	
	for (i = 0 ; i < NMAX ; i++)
		for (j = 0 ; j < NMAX ; j++)
			grille[i][j] = 0;
	
	initCounter();
	importCounter();
	
	fscanf(f_solv, "%i", &number);
	while (number != 0)
	{
		if (number > 0)
		{
			var = seekKCL(number);
			//printf("debug> Traitement du compteur %i, associé à kcl %i\n", number, var);
			
			if (var >= 111 && var <= 999)
			{
				l = var % 10;
				c = ((var - l) % 100)/10;
				k = (var - l - 10 * c) / 100;
			
				grille[c-1][l-1] = k;
			}
			
			/*if (var >= 11111 && var <= NVAR)
			{
				y = var % 10;
				x = ((var - y) % 100) / 10;
				b = ((var - y - 10 * x) % 1000) / 100;
				a = ((var - b * 100 - 10* x - y) % 10000) / 1000;
				k = (var - a * 1000 - b * 100 - 10* x - y) / 10000;
				
				grille[(a-1)*3+x][(b-1)*3+y] = k;
			}*/
		}
		
		fscanf(f_solv, "%i", &number);
	}
	
	fclose(f_solv);
			
	// Affichage de la grille de sudoku
	afficherGrille(grille);
	/*int z;
	for (x = 0 ; x < NMAX ; x++)
	{
		for (y = 0 ; y < NMAX ; y++)
		{
			if (y % 3 == 0 && y != 0)
				printf("\033[31;1m|\033[0m");
			printf("%i ", grille[x][y]);
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
	}*/
	
	return EXIT_SUCCESS;
}

int seekKCL(int compteur) {
	int i;
	
	for (i = 111 ; counter[i] != compteur && i <= NVAR ; i++);
	
	return i;
}

int seekKCLinFile(int index) {
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
}
