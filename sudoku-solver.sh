#!/bin/bash

./interface $1 n-sat.txt
#./3sat n-sat.txt 3-sat.txt
echo "-------------------"
./MiniSat_v1.14_linux n-sat.txt solution.txt > /dev/null
./solver solution.txt
