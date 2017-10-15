all: main

main: main.o parse.o read.o execute.o
	gcc -pedantic -o main main.o parse.o read.o execute.o

main.o: main.c
	gcc -pedantic -c main.c

read.o: read.c
	gcc -pedantic -c read.c

parse.o: parse.c
	gcc -pedantic -c parse.c

execute.o: execute.c
	gcc -pedantic -c execute.c

clean:
	rm -f main *.o
