// Ninja Code
while(TRUE) {
	// Code to enter the costume department
	down(mutex);
	if (ninjaAccess) {
		ninjas++;
		pirateAccess = false;
	}
	up(mutex);

	// Code to get dressed and leave
	down(mutex);
	ninjas--;
	if (ninjas == 0) {
		ninjaAccess = false;
		pirateAccess = true;
	}
	up(mutex);
}