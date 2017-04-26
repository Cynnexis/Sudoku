#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

unsigned int tab[NMAX][NMAX];

// Table pour le kcl (l'indicage commence à 1)
bool kcl[NMAX+1][NMAX+1][NMAX+1];
// Table pour le kabxy (l'indicage commence à 1)
bool kabxy[NMAX+1][3+1][3+1][3+1][3+1];

int main(int argc, char *argv[]) {
	int x, y, z, a, b, c, k, l, i, j;
	int c1, c2;
	unsigned char ch;
	
	FILE *f_sudoku, *f_dimacs;
	
	if (argc != 3)
	{
		fprintf(stderr, "%s: Error: Two arguments are missing.", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	f_sudoku = fopen(argv[1], "r");
	
	if (f_sudoku == NULL)
	{
		perror(argv[1]);
		exit(-2);
	}
	
	f_dimacs = fopen(argv[2], "w");
	
	if (f_dimacs == NULL)
	{
		perror(argv[2]);
		exit(-3);
	}
	
	// Initialisation de counter
	for (i = 0 ; i <= NVAR + 1 ; i++)
		counter[i] = 0;
	
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
			fscanf(f_sudoku, "%c", &ch);
			if (ch >= '0' && ch <= '9')
			{
				tab[x][y] = (int) (ch - '0');
			}
			else
			{
				y--;
			}
		}
	}
	
	fclose(f_sudoku);
	
	// Définition de kcl
	for (x = 1 ; x <= NMAX ; x++)
		for (y = 1 ; y <= NMAX  ; y++)
			kcl[tab[x-1][y-1]][x][y] = 1;
	
	// Définition de kabxy
	for (c = 1 ; c <= NMAX ; c++)
	{
		for (l = 1 ; l <= NMAX  ; l++)
		{
			if (c % 3 == 0 && l % 3 == 0)
				kabxy[tab[c-1][l-1]][(int) ceil(c/3.0)][(int) ceil(l/3.0)][3][3] = 1;
			else if (c % 3 == 0)
				kabxy[tab[c-1][l-1]][(int) ceil(c/3.0)][(int) ceil(l/3.0)][3][l%3] = 1;
			else if (l % 3 == 0)
				kabxy[tab[c-1][l-1]][(int) ceil(c/3.0)][(int) ceil(l/3.0)][c%3][3] = 1;
			else
				kabxy[tab[c-1][l-1]][(int) ceil(c/3.0)][(int) ceil(l/3.0)][c%3][l%3] = 1;
		}
	}
	
	
	
	
	
	
	// Affichage de la grille de sudoku
	afficherGrille(tab);
	
	
	
	initLine();
	initCounter();
	
	if (f_dimacs == NULL)
	{
		perror("dimacs.txt");
		exit(-3);
	}
	
	fprintf(f_dimacs, "p cnf %i %i\n", NB_VARIABLES, NB_CLAUSES); // 22113 sans compter les doublons et les ghosts, 22842 sans compter les ghosts, 25345 avec doublons et ghosts, 22842 sans optimisations (i != ...)
	
	for (k = 1 ; k <= 9 ; k++)
	{
		// Hyp 1
		for (c = 1 ; c <= 9 ; c++)
		{
			for (l = 1 ; l <= 9 ; l++)
			{
				for (i = 1 ; i <= 9 ; i++)
				{
					if (i != c)
					{
						c1 = useCounter(k * 100 + c * 10 + l);
						c2 = useCounter(k * 100 + i * 10 + l);
						fprintf(f_dimacs, "-%i -%i 0\n", c1, c2); useLine();
					}
				}
				
				for (j = 1 ; j <= 9 ; j++)
				{
					if (j != l)
					{
						c1 = useCounter(k * 100 + c * 10 + l);
						c2 = useCounter(k * 100 + c * 10 + j);
						fprintf(f_dimacs, "-%i -%i 0\n", c1, c2); useLine();
					}
				}
			}
		}
		
		// Hyp 2
		for (a = 1 ; a <= 3 ; a++)
		{
			for (b = 1 ; b <= 3 ; b++)
			{
				for (x = 1 ; x <= 3 ; x++)
				{
					for (y = 1 ; y <= 3 ; y++)
					{
						for (i = 1 ; i <= 3 ; i++)
						{
							for (j = 1 ; j <= 3 ; j++)
							{
								if (i != x || j != y) // TODO: && ou || ?
								{
	c1 = useCounter(k * 10000 + a * 1000 + b * 100 + x * 10 + y);
	c2 = useCounter(k * 10000 + a * 1000 + b * 100 + i * 10 + j);
	fprintf(f_dimacs, "-%i -%i 0\n", c1, c2); useLine();
								}
							}
						}
					}
				}
			}
		}
		
		// Hyp 3
		for (c = 1 ; c <= 9 ; c++)
		{
			for (l = 1 ; l <= 9 ; l++)
			{
				for (i = 1 ; i <= 9 ; i++)
				{
					if (i != k)
					{
						c1 = useCounter(k * 100 + c * 10 + l);
						c2 = useCounter(i * 100 + c * 10 + l);
						fprintf(f_dimacs, "-%i -%i 0\n", c1, c2); useLine();
					}
				}
			}
		}
		
		// Hyp 4
		for (c = 1 ; c <= 9 ; c++)
		{
			for (l = 1 ; l <= 9 ; l++)
			{
				c1 = useCounter(k * 100 + c * 10 + l);
				fprintf(f_dimacs, "%i ", c1);
			}
			fprintf(f_dimacs, "0\n"); useLine();
		}
		
		// Hyp 5
		for (l = 1 ; l <= 9 ; l++)
		{
			for (c = 1 ; c <= 9 ; c++)
			{
				c1 = useCounter(k * 100 + c * 10 + l);
				fprintf(f_dimacs, "%i ", c1);
			}
			fprintf(f_dimacs, "0\n"); useLine();
		}
		
		// Hyp 6
		for (a = 1 ; a <= 3 ; a++)
		{
			for (b = 1 ; b <= 3 ; b++)
			{
				for (x = 1 ; x <= 3 ; x++)
				{
					for (y = 1 ; y <= 3 ; y++)
					{
						c1 = useCounter(k * 10000 + a * 1000 + b * 100 + x * 10 + y);
						fprintf(f_dimacs, "%i ", c1);
					}
				}
				fprintf(f_dimacs, "0\n"); useLine();
			}
		}
		
		// Hyp 7
		for (c = 1 ; c <= 9 ; c++)
		{
			for (l = 1 ; l <= 9 ; l++)
			{
				a = (int) ceil(c/3);
				b = (int) ceil(l/3);
				
				x = c % 3;
				y = l % 3;
				
				
				x = (x == 0 ? 3 : x);
				y = (y == 0 ? 3 : y);
				
				
				
				c1 = useCounter(k * 100 + c * 10 + l);
				c2 = useCounter(k * 10000 + a * 1000 + b * 100 + x * 10 + y);
				fprintf(f_dimacs, "-%i %i 0\n", c1, c2); useLine();
				fprintf(f_dimacs, "%i -%i 0\n", c1, c2); useLine();
			}
		}
	}
	
	// Validation des variable dans la grille (mise en place des conditions initiales)
	// kcl
	for (k = 1 ; k <= 9 ; k++)
		for (c = 1 ; c <= 9 ; c++)
			for (l = 1 ; l <= 9 ; l++)
				if (kcl[k][c][l] != 0)
					fprintf(f_dimacs, "%i 0\n", useCounter(k * 100 + c * 10 + l)); useLine();
	
	// kabxy
	/*for (k = 1 ; k <= 9 ; k++)
		for (a = 1 ; a <= 3 ; a++)
			for (b = 1 ; b <= 3 ; b++)
				for (x = 1 ; x <= 3 ; x++)
					for (y = 1 ; y <= 3 ; y++)
						if (kabxy[k][a][b][x][y] != 0)
							fprintf(f_dimacs, "%i 0\n", useCounter(k * 10000 + a * 1000 + b * 100 + x * 10 + y)); useLine();*/
	
	// On réecrit le header pour actualiser le nombre de clause
	rewind(f_dimacs);
	fprintf(f_dimacs, "p cnf %i %i\n", NB_VARIABLES, getLine());
	
	fclose(f_dimacs);
	
	exportCounter();
	
	return EXIT_SUCCESS;
}
