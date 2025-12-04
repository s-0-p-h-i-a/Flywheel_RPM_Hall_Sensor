#include "hall_lib.h"
#include <stdbool.h>
#include <Arduino.h>

const int HALL_PIN = 13;

bool hallON = false;

unsigned long currentSensorRead = 0;
unsigned long lastSensorRead = 0;

bool secondPass = false;

void initHall(void) {
	
	currentSensorRead = 0;
	lastSensorRead = 0;
	secondPass = false;
	
	pinMode(HALL_PIN, INPUT_PULLUP);

}

void checkReadState(void) {
	
	if (digitalRead(HALL_PIN) && !secondPass)
	if (!secondPass) {
				
		secondPass = true;		
	} else {
			secondPass = false;
	}
}
