mutex mutex;
int[PIRATE_COUNT] piratesInRoom;
int[NINJA_COUNT] ninjasInRoom;
int ninjaAccess;
int pirateAccess;

// Ninja Code
int ninjaID = 0; // Some real value
while(TRUE) {
	// Code to enter the costume department
	down(mutex);
	if (ninjaAccess && !ninjasInRoom[ninjaID]) {
		ninjas++;
		ninjasInRoom[ninjaID] = TRUE;
	}
	up(mutex);

	// Code to get dressed and leave
	down(mutex);
	if (ninjaAccess && ninjasInRoom[ninjaID]) {
		ninjas--;
		ninjasInRoom[ninjaID] = FALSE;

		if (ninjas == 0) {
			ninjaAccess = FALSE;
			pirateAccess = TRUE;
		}
	}
	up(mutex);
}

// Pirate Code
int pirateID = 0; // Some real value
while(TRUE) {
	// Code to enter the costume department
	down(mutex);
	if (pirateAccess && !piratesInRoom[pirateID]) {
		pirates++;
		piratesInRoom[pirateID] = TRUE;
	}
	up(mutex);

	// Code to get dressed and leave
	down(mutex);
	if (pirateAccess && piratesInRoom[pirateID]) {
		pirates--;
		piratesInRoom[pirateID] = FALSE;
		
		if (pirates == 0) {
			ninjaAccess = TRUE;
			pirateAccess = FALSE;
		}
	}
	up(mutex);
}