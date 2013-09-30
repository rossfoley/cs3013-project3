/* Mutex for each quadrant, queue of intentions (i.e. next positions)
  When a car is about to release a mutex, it looks */

/*
1. Car approaches intersection and queues its set of moves
2. Car attempts to obtain mutex for first quadrant

*/

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

struct car {
	int id;
	int destination;
	int source;
	int position;
};

while (car.position != car.destination) {
	if (car.position >= -1) {
		int nextMove, updatePreviousCars;
		if (car.position == -1) {
			nextMove = car.source;
			updatePreviousCars = TRUE;
		} else {
			if (car.position == car.destination) {
				// Just exit the intersection
				break;
			} else {
				nextMove = car.position + 1;
			}
		}
		down(nextMove);
		if (canMakeMove && wontCauseDeadlock) {
			car.position++;
			if (updatePreviousCars) {
				// Loop through all cars
				// If they have the same source
				// And position < -1, do position++
			}
		}
		up(nextMove);
	} else {
		printf("Car #%i is still in line at %i and can't move forward.\n", car.id, car.source);
	}
}