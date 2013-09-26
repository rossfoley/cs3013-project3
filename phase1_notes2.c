semaphore ninjas = 2;
semaphore pirates = 0;
int ninjasInRoom = 0;
int piratesInRoom = 0;

// Ninjas
while (TRUE) {
	down(ninjas);
	ninjasInRoom++;
	printf("Ninja is in the room\n");
	sleep(1);
	printf("Ninja is leaving the room\n");
	ninjasInRoom--;
	if (ninjasInRoom == 0 && ninjas == 0) {
		up(pirates);
		up(pirates);
	}
}

// Pirates
while (TRUE) {
	down(pirates);
	piratesInRoom++;
	printf("Ninja is in the room\n");
	sleep(1);
	printf("Ninja is leaving the room\n");
	piratesInRoom--;
	if (piratesInRoom == 0 && pirates == 0) {
		up(ninja);
		up(ninja);
	}
}