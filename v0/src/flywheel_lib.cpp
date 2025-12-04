#include "flywheel_lib.h"
#include <stdbool.h>
#include <Arduino.h>

const int ANGLE_ZERO = 0;
const int ANGLE_180 = 180;
const int BUTTON_PIN = 5;
const int BUTTON2_PIN = 12;
int flywheelAngle = 0;
int speed = 0;

void initFlywheel(void) {
	
	flywheelAngle = ANGLE_ZERO;
}

void spinFlywheel(void) {
	
	if (flywheelAngle <= ANGLE_ZERO || flywheelAngle >= ANGLE_180) {
		
		speed = -speed;
	}
	
	if (digitalRead(BUTTON_PIN)) {
	
	 flywheelAngle += speed;
	}
}

void setSpeed(void) {

	bool inputValid = false;

	while (!inputValid) {

		Serial.println("Choose speed (0-12):");

		if (Serial.available() > 0) {
			speed = Serial.read();
		}
	
		if (speed < 0 || speed > 12) {
			inputValid = false;
			Serial.println("Invalid input, must be integer between 0-12");
		} else {
			inputValid = true;
		}
	}
}

void checkSpeed(void) {

	if (digitalRead(BUTTON2_PIN)) {

		bool inputValid = false;

		while (!inputValid) {

			Serial.println("Choose new speed (0-12):");

			if (Serial.available() > 0) {
				speed = Serial.read();
			}
		
			if (speed < 0 || speed > 12) {
				inputValid = false;
				Serial.println("Invalid input, must be integer between 0-12");
			} else {
				inputValid = true;
			}
			
		}
	}
}
