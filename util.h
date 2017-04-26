#ifndef __UTIL_H__
#define __UTIL_H__

typedef unsigned short int bool;
#define true 1
#define false 0

#define NMAX 9
#define NMAXVAR 9
#define NVAR 93333

#define NB_VARIABLES 729
#define NB_CLAUSES 22113

// Show the grid to the user
void showGrid(unsigned int tab[NMAX][NMAX]);

// Turn counter into kcl notation
int seekKCL(int compteur);
//int seekKCLinFile(int index);

/************* Line **************/
// Variable which will count the line
int _line;

// Initialization if _line with 0
void initLine();

// Increment _line and return it
int useLine();

// Return _line
int getLine();

/************* Ghost **************/
// Variable which will count the number of "ghost variable" (variables that are use to transform a n-sat into 3-sat).
int _ghost;

// Initialization of _ghost with nb_var
void initGhost(int nb_var);

// Increment _ghost and return it
int useGhost();

// Return _ghost
int getGhost();

/************* Counter **************/
// Array containing the translation from kcl to counter. Use of counter: counter[k * 100 + c * 10 + l] = counter of kcl
int counter[NVAR+1];

// Variable to count the kcl
int _count;

// Increment _count and return it
int useCount();

// Initialization of _count with 0
void initCounter();

// If position (which is a kcl) has no associated counter, useCounter create it (by using useCount() ) and add it in counter. In any case, it returns the associated counter to position
int useCounter(int position);

// Return _count
int getCount();

// Export the array counter to the file "export.tmp"
void exportCounter();

// Importing the array counter from the file "export.tmp"
void importCounter();

#endif
