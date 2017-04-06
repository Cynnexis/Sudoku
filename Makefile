CC=gcc
CFLAGS=-Wall -Werror -g
EXECUTABLES=interface

all: $(EXECUTABLES)

interface.o: interface.c
	$(CC) $(CFLAGS) -c interface.c

interface: interface.o
	$(CC) $(CFLAGS) -lm $< -o $@

test: $(EXECUTABLES)
	./interface examples/easy.txt

open:
	gedit --new-window interface.c Makefile dimacs.txt examples/easy.txt &

clean:
	rm -f $(EXECUTABLES) *.o 2> /dev/null
