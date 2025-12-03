#include "hall_lib.h"
#include "rpmlib.h"
#include <stdbool.h>
#include <Arduino.h>

int thisRPM = 0;
int lastRPM = 0;

int getTimeWindow(void) {
	
	int timeWindow = 1; // avoid division by 0 in RPM calculation
	
	if (digitalRead(HALL_PIN)) {
		
		currentSensorRead = millis();		
		timeWindow = currentSensorRead - lastSensorRead;		
		lastSensorRead = currentSensorRead;
		
	} else {
		
		timeWindow = millis() - lastSensorRead;		
	}
	
	return timeWindow;
}

int getRPM(void) {

	bool hallInput = digitalRead(HALL_PIN);

	if (hallInput && !secondPass) {	
		lastRPM = thisRPM;			
		thisRPM = 60000 / getTimeWindow();	
	}
	
	int result = round((thisRPM + lastRPM) / 2.0);
	return result;
}
