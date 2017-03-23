#include <stdio.h>
#include <stdlib.h>

#define NMAX 9
unsigned char tab[NMAX][NMAX];

int main(int argc, char *argv[]) {
	int x, y, z;
	unsigned char c;
	
	FILE *f_sudoku, *f_dimacs;
	
	if (argc != 2)
	{
		fprintf(stderr, "%s: Error: One argument is missing.", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	f_sudoku = fopen(argv[1], "r");
	
	if (f_sudoku == NULL)
	{
		perror(argv[1]);
		exit(-2);
	}
	
	// Remplissage du tableau
	for (x = 0 ; x < NMAX ; x++)
	{
		for (y = 0 ; y < NMAX  ; y++)
		{
			fscanf(f_sudoku, "%c", &c);
			if (c >= '0' && c <= '9')
			{
				tab[x][y] = c;
			}
			else
			{
				y--;
			}
		}
	}
	
	// Affichage de la grille de sudoku
	for (x = 0 ; x < NMAX ; x++)
	{
		for (y = 0 ; y < NMAX ; y++)
		{
			if (y % 3 == 0 && y != 0)
				printf("\033[31;1m|\033[0m");
			printf("%c ", (tab[x][y] == '0' ? ' ' : tab[x][y]));
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
	
	
	
	
	
	
	
	f_dimacs = fopen("dimacs.txt", "w");
	
	if (f_dimacs == NULL)
	{
		perror("dimacs.txt");
		exit(-3);
	}
	
	fprintf(f_dimacs, "p cnf");
	
	
	return EXIT_SUCCESS;
}
