all: phase1

phase1: phase1.c
	gcc -pthread -o phase1 phase1.c