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
	pthread_mutex_t lock;
};

pthread_mutex_t quadrants[4];
pthread_mutex_t check;

pthread_t car_threads[CARS];

struct car cars[CARS];

void *run_car(void *carID) {
	int id = (int) carID;
	
	pthread_mutex_lock(&cars[id].lock);

	while (TRUE) {
		int i, posCount = -1, destination, position;
		int source = rand() % 4;
		do {
			destination = rand() % 4;
		} while (source == destination);
		
		for (i = 0; i < CARS; i++) {
			if (cars[i].source == source && cars[i].position < 0) {
				posCount--;
			}
		}

		position = posCount;

		cars[id].source = source;
		cars[id].destination = destination;
		cars[id].position = position;

		printf("Car #%i was given source %i, destination %i, and position %i.\n", id, cars[id].source, cars[id].destination, -1*cars[id].position);

		while (cars[id].position != cars[id].destination) {
			if (cars[id].position >= -1 && cars[id].position <= 5) {
				int nextMove, updatePreviousCars;
				int i, madeMove = FALSE, deadlockCount = 0, canMakeMove = TRUE, wontCauseDeadlock = TRUE;

				pthread_mutex_lock(&check);

				if (cars[id].position == -1) {
					nextMove = (cars[id].source + 1) % 4;
					updatePreviousCars = TRUE;
				} else {
					nextMove = (cars[id].position + 1) % 4;
				}

				pthread_mutex_unlock(&check);

				pthread_mutex_lock(&quadrants[nextMove]);

				for (i = 0; i < CARS; i++) {
					canMakeMove = canMakeMove && !(cars[i].position == nextMove);
				}
				
				for (i = 0; i < CARS; i++) {
					if (cars[i].position >= 0 && cars[i].position <= 3 && i != id && cars[i].destination != cars[i].position) {
						deadlockCount++;
					}	
				}

				// If deadlockCount is 3, then letting the current car into the intersection will cause deadlock
				wontCauseDeadlock = (deadlockCount < 3); 

				if (canMakeMove && wontCauseDeadlock) {
					cars[id].position = nextMove;
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
					for (i = 0; i < CARS; i++) {
						if (cars[i].position >= -1 && cars[i].position <= 3 && i != id) {
							pthread_mutex_unlock(&cars[i].lock);
						}
					}
				}

				pthread_mutex_unlock(&quadrants[nextMove]);
				
				if (!madeMove) {
					// Couldn't make a move, so let another car try
				}

			} else {
				printf("Car #%i is still in line at source %i with position %i.\n", id, cars[id].source, -1*cars[id].position);
				// Still stuck in line, let another car try to move
				pthread_mutex_lock(&cars[id].lock);
			}
		}

		printf("Car #%i has left the intersection at destination %i.\n", id, cars[id].destination);
	}
}

int main(int argc, char *argv[]) {
	int i, status;
	
	for (i = 0; i < 4; i++) {
		pthread_mutex_init(&quadrants[i], NULL);
	}
	pthread_mutex_init(&check, NULL);

	srand(time(NULL));

	for (i = 0; i < CARS; i++) {
		cars[i].position = 5;
		pthread_mutex_init(&cars[i].lock, NULL);
	}

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