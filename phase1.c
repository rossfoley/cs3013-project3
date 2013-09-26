#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0
#define NINJA_COUNT 14
#define PIRATE_COUNT 18

pthread_t ninjaThreads[NINJA_COUNT];
pthread_t pirateThreads[PIRATE_COUNT];

sem_t ninjas, pirates;
int ninjasInRoom = 0;
int piratesInRoom = 0;

// Ninjas
void *run_ninja(void *ninjaID) {
	while (TRUE) {
		int ninjaValue;
		sem_wait(&ninjas);
		ninjasInRoom++;
		printf("Ninja %d is in the room\n", (int) ninjaID);
		sleep(1); // Change costume
		printf("Ninja %d is leaving the room\n", (int) ninjaID);
		ninjasInRoom--;
		sem_getvalue(&ninjas, &ninjaValue);
		if (ninjasInRoom == 0 && ninjaValue == 0) {
			sem_post(&pirates);
			sem_post(&pirates);
		}
	}
	pthread_exit(NULL);
}

// Pirates
void *run_pirate(void *pirateID) {
	while (TRUE) {
		int pirateValue;
		sem_wait(&pirates);
		piratesInRoom++;
		printf("Pirate %d is in the room\n", (int) pirateID);
		sleep(1); // Change costume
		printf("Pirate %d is leaving the room\n", (int) pirateID);
		piratesInRoom--;
		sem_getvalue(&pirates, &pirateValue);
		if (piratesInRoom == 0 && pirateValue == 0) {
			sem_post(&ninjas);
			sem_post(&ninjas);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	// Create our threads
	sem_init(&ninjas, 0, 2); // Start by allowing 2 ninjas
	sem_init(&pirates, 0, 0); // Start by not allowing pirates

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

	for (i=0; i < NINJA_COUNT; i++) {
		status = pthread_join(ninjaThreads[i], NULL);
		if (status != 0) {
			printf("Error joining ninja #%d!\n", i);
			exit(-1);
		}
	}

	for (i=0; i < PIRATE_COUNT; i++) {
		status = pthread_join(pirateThreads[i], NULL);
		if (status != 0) {
			printf("Error joining pirate #%d!\n", i);
			exit(-1);
		}
	}

	exit(0);
}