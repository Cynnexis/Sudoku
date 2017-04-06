#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned short int bool;
#define true 1
#define false 0

#define NVAR 93333

#define NMAX 9
unsigned char tab[NMAX][NMAX];

// Table pour le kcl (l'indicage commence à 1)
bool kcl[NMAX+1][NMAX+1][NMAX+1];
// Table pour le kabxy (l'indicage commence à 1)
bool kabxy[NMAX+1][3+1][3+1][3+1][3+1];

// Counter
int counter[NVAR+1];
int _count = 0;

int useCount();
int useCounter(int position);
int chercherKcl(int kcl);

int main(int argc, char *argv[]) {
	int x, y, z, a, b, c, k, l, i, j;
	int c1, c2;
	unsigned char ch;
	
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
	
	// Initialisation de counter
	for (i = 0 ; i <= NMAX + 1 ; i++)
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
				tab[x][y] = ch;
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
	
	fprintf(f_dimacs, "p cnf 1458 22842\n"); // 22113 sans compter les doublons et les ghosts, 22842 sans compter les ghosts, 25345 avec doublons et ghosts
	
	for (k = 1 ; k <= 9 ; k++)
	{
		// Hyp 1
		for (c = 1 ; c <= 9 ; c++)
		{
			for (l = 1 ; l <= 9 ; l++)
			{
				for (i = 1 ; i <= 9 ; i++)
				{
					c1 = useCounter(k * 100 + c * 10 + l);
					c2 = useCounter(k * 100 + i * 10 + l);
					fprintf(f_dimacs, "-%i -%i 0\n", c1, c2);
				}
				
				for (j = 1 ; j <= 9 ; j++)
				{
					c1 = useCounter(k * 100 + c * 10 + l);
					c2 = useCounter(k * 100 + c * 10 + j);
					fprintf(f_dimacs, "-%i -%i 0\n", c1, c2);
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
							if (i != x)
							{
								for (j = 1 ; j <= 3 ; j++)
								{
									if (j != y)
									{
	c1 = useCounter(k * 10000 + a * 1000 + b * 100 + x * 10 + y);
	c1 = useCounter(k * 10000 + a * 1000 + b * 100 + i * 10 + j);
	fprintf(f_dimacs, "-%i -%i 0\n", c1, c2);
									}
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
					c1 = useCounter(k * 100 + c * 10 + l);
					c2 = useCounter(i * 100 + c * 10 + l);
					fprintf(f_dimacs, "-%i -%i 0\n", c1, c2);
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
			fprintf(f_dimacs, "0\n");
		}
		
		// Hyp 5
		for (l = 1 ; l <= 9 ; l++)
		{
			for (c = 1 ; c <= 9 ; c++)
			{
				c1 = useCounter(k * 100 + c * 10 + l);
				fprintf(f_dimacs, "%i ", c1);
			}
			fprintf(f_dimacs, "0\n");
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
				fprintf(f_dimacs, "0\n");
			}
		}
	}
	
	fclose(f_dimacs);
	
	
	return EXIT_SUCCESS;
}
int useCount() {
	_count++;
	return _count;
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
