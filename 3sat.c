#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned short int bool;
#define true 1
#define false 0

int nb_var, nb_clause;

#define NMAXVAR 9
int vars[NMAXVAR];

int _line = 0;
int useLine();

int _ghost = 0;
void initGhost();
int useGhost();

int main(int argc, char *argv[]) {
	FILE *f_dimacs, *f_3sat;
	char ch;
	int i, j, var, g, g1, g2;
	
	if (argc != 3)
	{
		fprintf(stderr, "%s: Error: Two arguments are missing.", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	f_dimacs = fopen(argv[1], "r");
	if (f_dimacs == NULL)
	{
		perror(argv[1]);
		exit(-2);
	}
	
	f_3sat = fopen(argv[2], "w");
	if (f_3sat == NULL)
	{
		perror(argv[2]);
		exit(-3);
	}
	
	// Lecture du header
	fscanf(f_dimacs, "%c", &ch);
	if (ch != 'p') exit(EXIT_FAILURE);
	
	fscanf(f_dimacs, "%c", &ch);
	fscanf(f_dimacs, "%c", &ch);
	fscanf(f_dimacs, "%c", &ch);
	fscanf(f_dimacs, "%c", &ch);
	fscanf(f_dimacs, "%c", &ch);
	fscanf(f_dimacs, "%i", &nb_var);
	fscanf(f_dimacs, "%i", &nb_clause);
	
	printf("nb var : %i\nnb clause : %i\n", nb_var, nb_clause);
	
	//fprintf(f_3sat, "\n");
	fprintf(f_3sat, "p cnf %i %i\n", nb_var, nb_clause);
	
	initGhost();
	
	var = -1;
	while (!feof(f_dimacs))
	{
		fscanf(f_dimacs, "%c", &ch); // Récupération \n
		fscanf(f_dimacs, "%i", &var);
		for (i = 0 ; var != 0 ; i++)
		{
			vars[i] = var;
			fscanf(f_dimacs, "%i", &var);
		}
		
		// Le nombre d'élément dans la ligne est i. On fait un disjonction de cas:
		if (i == 1)
		{
			g1 = useGhost();
			g2 = useGhost();
			fprintf(f_3sat, "%i %i %i 0\n", vars[0], g1, g2); useLine();
			fprintf(f_3sat, "%i %i -%i 0\n", vars[0], g1, g2); useLine();
			fprintf(f_3sat, "%i -%i %i 0\n", vars[0], g1, g2); useLine();
			fprintf(f_3sat, "%i -%i -%i 0\n", vars[0], g1, g2); useLine();
		}
		else if (i == 2)
		{
			g = useGhost();
			fprintf(f_3sat, "%i %i %i 0\n", vars[0], vars[1], g); useLine();
			fprintf(f_3sat, "%i %i -%i 0\n", vars[0], vars[1], g); useLine();
		}
		else if (i == 3)
		{
			fprintf(f_3sat, "%i %i %i 0\n", vars[0], vars[1], vars[2]); useLine();
		}
		else
		{
			for (j = 0; j < i ; j++)
			{
				if (j == 0)
				{
					g = useGhost();
					fprintf(f_3sat, "%i %i %i 0\n", vars[0], vars[1], g); useLine();
					j++;
				}
				else if (j == i-2)
				{
					fprintf(f_3sat, "-%i %i %i 0\n", g, vars[j], vars[j+1]); useLine();
					j++;
				}
				else
				{
					g1 = useGhost();
					fprintf(f_3sat, "-%i %i %i 0\n", g, vars[j], g1); useLine();
					g = g1;
				}
			}
		}
	}
	
	// On réecrit le header pour actualiser le nombre de clause
	rewind(f_3sat);
	fprintf(f_3sat, "p cnf %i %i\n", nb_var + _ghost, _line);
	
	fclose(f_dimacs);
	fclose(f_3sat);
	
	return EXIT_SUCCESS;
}

int useLine() {
	_line++;
	return _line;
}

void initGhost() {
	_ghost = nb_var;
}

int useGhost() {
	_ghost++;
	return _ghost;
}

