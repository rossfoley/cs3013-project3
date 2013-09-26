#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define NINJA_COUNT 14
#define PIRATE_COUNT 18

pthread_t ninjaThreads[NINJA_COUNT];
pthread_t pirateThreads[PIRATE_COUNT];

semaphore ninjas = 2;
semaphore pirates = 0;
int ninjasInRoom = 0;
int piratesInRoom = 0;

// Ninjas
void *run_ninja(void *ninjaID) {
	while (TRUE) {
		down(ninjas);
		ninjasInRoom++;
		printf("Ninja %d is in the room\n", ninjaID);
		sleep(1); // Change costume
		printf("Ninja %d is leaving the room\n", ninjaID);
		ninjasInRoom--;
		if (ninjasInRoom == 0 && ninjas == 0) {
			up(pirates);
			up(pirates);
		}
	}
	pthread_exit(NULL);
}

// Pirates
void *run_pirate(void *pirateID) {
	while (TRUE) {
		down(pirates);
		piratesInRoom++;
		printf("Pirate %d is in the room\n", pirateID);
		sleep(1); // Change costume
		printf("Pirate %d is leaving the room\n", pirateID);
		piratesInRoom--;
		if (piratesInRoom == 0 && pirates == 0) {
			up(ninja);
			up(ninja);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	// Create our threads
	int i, status;

	for (i=0; i < NINJA_COUNT; i++) {
		status = pthread_create(&ninjaThreads[i], NULL, run_ninja, (void *) i);
		if (status != 0) {
			printf("Error creating ninja #%d!\n", i);
			exit(-1);
		}
	}

	for (i=0; i < PIRATE_COUNT; i++) {
		status = pthread_create(&pirateThreads[i], NULL, run_pirate, (void *) i);
		if (status != 0) {
			printf("Error creating pirate #%d!\n", i);
			exit(-1);
		}
	}
	
	exit(0);
}