CC = gcc
CFLAGS = -pedantic -Wall -g
LD = gcc
LDFLAGS = -g

all: timeit timeit.o

timeit: timeit.o
	$(LD) $(LDFLAGS) -o timeit timeit.o

timeit.o: timeit.c
	$(CC) $(CFLAGS) -c -o timeit.o timeit.c

clean: timeit
	rm timeit.o
run: timeit
	./timeit 7
debug: timeit
	gdb timeit
valgrind: timit
	valgrind -s --leak-check=full --track-origins=yes --show-leak-kinds=all \
	./timeit 3