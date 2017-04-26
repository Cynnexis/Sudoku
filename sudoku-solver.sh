#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Error: Bad number of argument."
	echo "Usage: $0 <File>"
	echo "       File : path to file containing a sudoku grid."
	exit 1;
fi

echo "Turning grid into n-sat (dimacs)"
./interface $1 n-sat.txt
echo "-------------------"
echo "Changing n-sat into 3-sat"
./3sat n-sat.txt 3-sat.txt
echo "-------------------"
echo "Computing solution"
./MiniSat_v1.14_linux 3-sat.txt solution.txt > /dev/null
echo "-------------------"
echo "Reading solution"
./solver solution.txt
echo "Done."
