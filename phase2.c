#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define RIGHT 0
#define STRAIGHT 1
#define LEFT 2

#define NE 0
#define NW 1
#define SW 2
#define SE 3

#define NORTH 0
#define WEST 1
#define SOUTH 2
#define EAST 3

#define CARS 20

struct car {
	int destination;
	int source;
	int position;
};

sem_t quadrants[4];

pthread_t car_threads[CARS];

struct car cars[CARS];

void *run_car(void *carID) {
	int id = (int) carID;
	while (TRUE) {
		int i, min, destination, position, source = rand() % 4;
		do {
			destination = rand() % 4;
		} while (source == destination);
		
		for (i = 0; i < CARS; i++) {
			if (cars[i].source == cars[id].source && cars[i].position < min) {
				min = cars[i].position;
			}
		}

		position = min - 1;

		cars[id].source = source;
		cars[id].destination = destination;
		cars[id].position = position;

		printf("Car #%i given new orders. Source: %i, Destination: %i.\n", id, cars[id].source, cars[id].destination);
		while (cars[id].position != cars[id].destination) {
			if (cars[id].position >= -1) {
				int nextMove, updatePreviousCars;
				int i, madeMove = FALSE, deadlockCount, canMakeMove = TRUE, wontCauseDeadlock = TRUE;
				
				if (cars[id].position == -1) {
					nextMove = cars[id].source;
					updatePreviousCars = TRUE;
				} else {
					nextMove = cars[id].position + 1;
				}

				sem_wait(&quadrants[nextMove]);

				for (i = 0; i < CARS; i++) {
					canMakeMove = canMakeMove && !(cars[i].position == nextMove);
				}
				
				for (i = 0; i < CARS; i++) {
					if (cars[i].position >= 0 && i != id) {
						deadlockCount += !!(cars[i].destination - cars[i].position);
					}	
				}

				// If deadlockCount is 3, then letting the current car into the intersection will cause deadlock
				wontCauseDeadlock = (deadlockCount < 3); 

				if (canMakeMove && wontCauseDeadlock) {
					cars[id].position++;
					madeMove = TRUE;
					printf("Car #%i just moved to position %i.\n", id, cars[id].position);
					if (updatePreviousCars) {
						// Loop through all cars
						// If they have the same source
						// And position < -1, do position++
						for (i = 0; i < CARS; i++) {
							if (cars[i].source == cars[id].source && cars[i].position < -1) {
								cars[i].position++;
							}
						}
					}
				}

				sem_post(&quadrants[nextMove]);
				if (madeMove) {
					// Couldn't make a move, so let another car try
					pthread_yield();
				}

			} else {
				printf("Car #%i is still in line at %i with position %i.\n", id, cars[id].source, -1*cars[id].position);
				// Still stuck in line, let another car try to move
				pthread_yield();
			}
		}

		printf("Car #%i has left the intersection at destination %i.\n", id, cars[id].destination);
	}
}

int main(int argc, char *argv[]) {
	int i, status;

	srand(time(NULL));

	for (i=0; i < CARS; i++) {
		status = pthread_create(&car_threads[i], NULL, run_car, (void *) i);
		if (status != 0) {
			printf("Error creating car #%d!\n", i);
			exit(-1);
		}
	}

	for (i=0; i < CARS; i++) {
		status = pthread_join(car_threads[i], NULL);
		if (status != 0) {
			printf("Error joining car #%d!\n", i);
			exit(-1);
		}
	}

	exit(0);
}