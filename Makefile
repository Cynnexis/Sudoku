CC=gcc
CFLAGS=-Wall -Werror
EXECUTABLES=interface

interface.o: interface.c
	$(CC) $(CFLAGS) -c interface.c

interface: interface.o
	$(CC) $(CFLAGS) $< -o $@

test: $(EXECUTABLES)
	./interface example.txt
	#less dimacs.txt

clean:
	rm -f $(EXECUTABLES) *.o 2> /dev/null
