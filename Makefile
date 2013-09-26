all: phase1

phase1.o: phase1.c
	gcc -c phase1.c

phase1: phase1.o
	gcc -o phase1 phase1.o