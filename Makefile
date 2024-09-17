all: prog

prog: main.o schrift.o
	gcc main.o schrift.o -o prog -lm

main.o: main.c
	gcc -c main.c

schrift.o: schrift.c
	gcc -c schrift.c

clean: 
	rm -f main.o schrift.o prog