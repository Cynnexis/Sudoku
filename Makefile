CC=gcc
CFLAGS=-Wall -Werror -g
EXECUTABLES=interface 3sat solver
EXTRA_FILES=n-sat.txt 3-sat.txt solution.txt

all: $(EXECUTABLES)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

interface: interface.o util.o
	$(CC) $(CFLAGS) -lm $< util.o -o $@

3sat: 3sat.o util.o
	$(CC) $(CFLAGS) -lm $< util.o -o $@

solver: solver.o util.o
	$(CC) $(CFLAGS) -lm $< util.o -o $@

test: $(EXECUTABLES)
	./sudoku-solver examples/easy2.txt

open:
	gedit --new-window interface.c 3sat.c solver.c util.h util.c Makefile sudoku-solver.sh &

clean:
	rm -f $(EXECUTABLES) $(EXTRA_FILES) *.o 2> /dev/null
