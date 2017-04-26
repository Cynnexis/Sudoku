#ifndef __UTIL_H__
#define __UTIL_H__

typedef unsigned short int bool;
#define true 1
#define false 0

#define NMAX 9
#define NMAXVAR 9
#define NVAR 93333

#define NB_VARIABLES 1458
#define NB_CLAUSES 22113

int kabxy2kcl(int i);
int kcl2kabxy(int i);

void afficherGrille(unsigned int tab[NMAX][NMAX]);

// Line
int _line;
void initLine();
int useLine();
int getLine();

// Ghost
int _ghost;
void initGhost(int nb_var);
int useGhost();
int getGhost();

// Counter
int counter[NVAR+1];
int _count;

int useCount();
void initCounter();
int useCounter(int position);
int getCount();
void exportCounter();
void importCounter();

#endif
