#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

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

sem_t quadrants[4];

pthread_t cars[CARS];

struct car {
	int id;
	int destination;
	int source;
	int position;
};
void *run_car(void *id) {
	while (TRUE) {
		// Generate source and destination for car
		printf("Car #%i given new orders. Source: %i, Destination: %i.\n", id, cars[id].source, cars[id].destination);
		while (cars[id].position != cars[id].destination) {
			if (cars[id].position >= -1) {
				int nextMove, updatePreviousCars;

				if (cars[id].position == -1) {
					nextMove = cars[id].source;
					updatePreviousCars = TRUE;
				} else {
					nextMove = cars[id].position + 1;
				}

				sem_wait(&quadrants[nextMove]);

				if (canMakeMove && wontCauseDeadlock) {
					cars[id].position++;
					if (updatePreviousCars) {
						// Loop through all cars
						// If they have the same source
						// And position < -1, do position++
					}
				}

				sem_post(&quadrants[nextMove]);
			} else {
				printf("Car #%i is still in line at %i with position %i.\n", id, cars[id].source, -1*cars[id].position);
			}
		}
		
		printf("Car #%i has left the intersection at destination %i.\n", id, cars[id].destination);
	}
}