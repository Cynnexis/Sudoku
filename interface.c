#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned short int bool;
#define true 1
#define false 0

#define NMAX 9
unsigned char tab[NMAX][NMAX];

#define NVAR 2 * 9 * 9 * 9
//bool vars[NVAR];

// Table pour le kcl (l'indicage commence à 1)
bool kcl[NMAX+1][NMAX+1][NMAX+1];
// Table pour le kabxy (l'indicage commence à 1)
bool kabxy[NMAX+1][3+1][3+1][3+1][3+1];

int main(int argc, char *argv[]) {
	int x, y, z, a, b, k;
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
	
	// Initialisation de kcl
	for (x = 1 ; x <= NMAX ; x++)
		for (y = 1 ; y <= NMAX ; y++)
			for (z = 1 ; z <= NMAX ; z++)
				kcl[x][y][z] = 0;
	
	// Initialisation de kabxy
	for (k = 1 ; k <= NMAX ; k++)
		for (a = 1 ; a <= 3 ; a++)
			for (b = 1 ; b <= 3 ; b++)
				for (x = 1 ; x <= 3 ; x++)
					for (y = 1 ; y <= 3 ; y++)
						kabxy[k][a][b][x][y] = 0;
	
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
	
	// Définition de kcl
	for (x = 1 ; x <= NMAX ; x++)
		for (y = 1 ; y <= NMAX  ; y++)
			kcl[tab[x-1][y-1]-'0'][x][y] = 1;
	
	// Définition de kabxy
	for (x = 1 ; x <= NMAX ; x++)
		for (y = 1 ; y <= NMAX  ; y++)
			kabxy[tab[x-1][y-1]-'0'][(int) ceil(x/3.0)][(int) ceil(y/3.0)][x%3][y%3] = 1;
	
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
