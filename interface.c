#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

unsigned int tab[NMAX][NMAX];

// Array for kcl (indexes begin at 1)
bool kcl[NMAX+1][NMAX+1][NMAX+1];

int main(int argc, char *argv[]) {
	int x, y, z, c, k, l, i, j;
	int c1, c2;
	unsigned char ch;
	
	FILE *f_sudoku, *f_dimacs;
	
	// Check number of argument
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
	
	// Initialization of counter
	for (i = 0 ; i <= NVAR + 1 ; i++)
		counter[i] = 0;
	
	// Initialization of kcl
	for (x = 1 ; x <= NMAX ; x++)
		for (y = 1 ; y <= NMAX ; y++)
			for (z = 1 ; z <= NMAX ; z++)
				kcl[x][y][z] = 0;
	
	// Filling array tab
	for (x = 0 ; x < NMAX ; x++)
	{
		for (y = 0 ; y < NMAX  ; y++)
		{
			fscanf(f_sudoku, "%c", &ch);
			if (ch >= '0' && ch <= '9')
				tab[x][y] = (int) (ch - '0');
			else
				y--;
			
			// Check the dimension of the grid
			if (ch == '\n' && (y != -1 || x > 9))
			{
				printf("%s: Error: The grid must be 9x9.\n", argv[0]);
				return -1;
			}
		}
	}
	
	// Check the dimension of the grid
	fscanf(f_sudoku, "%c", &ch);
	fscanf(f_sudoku, "%c", &ch);
	if (!feof(f_sudoku))
	{
		printf("%s: Error: The grid is too big.\n", argv[0]);
		return -1;
	}
	
	
	
	
	fclose(f_sudoku);
	
	// Filling KCL
	for (x = 1 ; x <= NMAX ; x++)
		for (y = 1 ; y <= NMAX  ; y++)
			kcl[tab[x-1][y-1]][x][y] = 1;
	
	showGrid(tab);
	
	
	
	initLine();
	initCounter();
	
	if (f_dimacs == NULL)
	{
		perror("dimacs.txt");
		exit(-3);
	}
	
	fprintf(f_dimacs, "p cnf %i                      \n", NB_VARIABLES);
	
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
		for (c = 1 ; c <= 9 ; c++)
		{
			for (l = 1 ; l <= 9 ; l++)
			{
				x = (int) (floor((c - 1)/3)*3 + 1);
				y = (int) (floor((l - 1)/3)*3 + 1);
				for (i = x ; i <= x + 2 ; i++)
				{
					for (j = y ; j <= y + 2 ; j ++)
					{
						if (i != c || j != l)
						{
							c1 = useCounter(k * 100 + c * 10 + l);
							c2 = useCounter(k * 100 + i * 10 + j);
							fprintf(f_dimacs, "-%i -%i 0\n", c1, c2); useLine();
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
		for (x = 0 ; x <= 2 ; x++)
		{
			for (y = 0 ; y <= 2 ; y++)
			{
				for (c = 3*x + 1 ; c <= 3*x + 3 ; c++)
				{
					for (l = 3*y + 1 ; l <= 3*y + 3 ; l++)
					{
						c1 = useCounter(k * 100 + c * 10 + l);
						fprintf(f_dimacs, "%i ", c1);
					}
				}
				fprintf(f_dimacs, "0\n"); useLine();
			}
		}
	}
	
	// Initial conditions
	for (k = 1 ; k <= 9 ; k++)
	{
		for (c = 1 ; c <= 9 ; c++)
		{
			for (l = 1 ; l <= 9 ; l++)
			{
				if (kcl[k][c][l] != 0)
				{
					c1 = useCounter(k * 100 + c * 10 + l);
					fprintf(f_dimacs, "%i 0\n", c1);
					useLine();
				}
			}
		}
	}
	
	// Rewriting header to refresh the number of clauses
	fseek(f_dimacs, 0, SEEK_SET);
	fprintf(f_dimacs, "p cnf %i %i", NB_VARIABLES, getLine());
	
	fclose(f_dimacs);
	
	// Exporting counter to solver
	exportCounter();
	
	return EXIT_SUCCESS;
}
