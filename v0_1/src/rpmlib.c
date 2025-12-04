#include "global.h"
#include "hall_lib.h"
#include "rpmlib.h"
#include <stdbool.h>

int RPM = 0;
int thisRPM = 0;
int lastRPM = 0;

void getTimeWindow(void) {
	
	if (hallON) {

		timeWindow = currentSensorRead - lastSensorRead;		
	} else {

		timeWindow = now - lastSensorRead;	
	}
}

void getRPM(void) {

	if (hallON && secondPass) {;}
	else if (isIdle) {
		RPM = 0;
	} else {
		lastRPM = thisRPM;			
		thisRPM = round(timeWindow/60000.0);	// just in case to avoid div by 0
	}
	RPM = round((thisRPM + lastRPM) / 2.0);
}
