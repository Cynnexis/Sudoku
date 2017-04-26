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
	int number = 0, var = 0, k, i, j, c, l;
	char cc;
	
	// Checking the number of argument
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
	
	// Check if the solution is unsatisfiable ("UNSAT" in solution.txt)
	if (cc == 'U')
	{
		printf("This set of clauses is unsatisfiable.\n");
		return EXIT_SUCCESS;
	}
	
	// Check if the solution is satisfiable ("SAT" in solution.txt)
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
	// Importing counter from interface
	importCounter();
	
	// Importing solution into grid
	fscanf(f_solv, "%i", &number);
	while (number != 0)
	{
		if (number > 0)
		{
			var = seekKCL(number);
			
			if (var >= 111 && var <= 999)
			{
				l = var % 10;
				c = ((var - l) % 100)/10;
				k = (var - l - 10 * c) / 100;
			
				grille[c-1][l-1] = k;
			}
		}
		
		fscanf(f_solv, "%i", &number);
	}
	
	fclose(f_solv);
	
	showGrid(grille);
	
	return EXIT_SUCCESS;
}
