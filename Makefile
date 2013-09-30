all: phase1 phase2

phase1: phase1.c
	gcc -pthread -o phase1 phase1.c

phase2: phase2.c
	gcc -pthread -o phase2 phase2.c