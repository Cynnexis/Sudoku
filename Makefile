CC=gcc
CFLAGS=-Wall -Werror -g
EXECUTABLES=interface 3sat

all: $(EXECUTABLES)

interface.o: interface.c
	$(CC) $(CFLAGS) -c interface.c

interface: interface.o
	$(CC) $(CFLAGS) -lm $< -o $@

3sat.o: 3sat.c
	$(CC) $(CFLAGS) -c 3sat.c

3sat: 3sat.o
	$(CC) $(CFLAGS) -lm $< -o $@

test: $(EXECUTABLES)
	./interface examples/easy.txt
	./3sat dimacs.txt result.txt

open:
	gedit --new-window interface.c Makefile dimacs.txt examples/easy.txt &

clean:
	rm -f $(EXECUTABLES) *.o 2> /dev/null
