#include "global.h"
#include "rpmlib.h"
#include "flywheel_lib.h"
#include <Arduino.h>

const int ANGLE_ZERO = 0;
const int ANGLE_180 = 180;
const int MAX_SPEED = 12;
const int MIN_SPEED = 1;
const int BUTTON_PIN = 2;
const int FASTER_BUTTON_PIN = 4;
const int SLOWER_BUTTON_PIN = 7;
const int BUTTON_DELAY = 1000;
unsigned int flywheelAngle = 0;
unsigned int lastAngle = 0;
int speed = 1;
bool isIdle = false;
const int idleInterval = 10000;
long unsigned int thisSpeedIncreaseTime = 0;
long unsigned int lastSpeedIncreaseTime = 0;
long unsigned int thisSpeedDecreaseTime = 0;
long unsigned int lastSpeedDecreaseTime = 0;

void initFlywheel(void) {
	
	flywheelAngle = ANGLE_ZERO;
}

void handleEdgeAngles(void) {

	if (flywheelAngle = lastAngle) {;}

	else {
		// prevent angle from going beyond 0-180
		if (flywheelAngle < ANGLE_ZERO) { flywheelAngle = ANGLE_ZERO;}
		else if (flywheelAngle > ANGLE_180) { flywheelAngle = ANGLE_180;}
		
		if (flywheelAngle == ANGLE_ZERO || flywheelAngle == ANGLE_180) {
			if (speed !=0) {speed = -speed;}
		}
	}
}

void checkFlywheelState(void) {
	
	if (flywheelAngle = lastAngle) {
		if (timeWindow >= idleInterval) {
			isIdle = true;
		}
	} else { isIdle = true;}
}

bool canAccelerate(void) {

	thisSpeedIncreaseTime = now;
	unsigned long accelTimeGap = thisSpeedIncreaseTime - lastSpeedIncreaseTime;
	lastSpeedIncreaseTime = thisSpeedIncreaseTime;

	bool canChange = accelTimeGap >= BUTTON_DELAY && speed < MAX_SPEED;
	return canChange;
}

bool canDecelerate(void) {

	thisSpeedDecreaseTime = now;
	unsigned long decelTimeGap = thisSpeedDecreaseTime - lastSpeedDecreaseTime;
	lastSpeedDecreaseTime = thisSpeedDecreaseTime;

	bool canChange = decelTimeGap >= BUTTON_DELAY && speed > MIN_SPEED+1; // if speed drops to 0, flywheelAngle doesn't get updated
	return canChange;
}

void updateSpeed(void) {

	bool faster = digitalRead(FASTER_BUTTON_PIN);
	bool slower = digitalRead(SLOWER_BUTTON_PIN);

	if (faster && canAccelerate()) { ++speed;}
	if (slower && canDecelerate()) { --speed;}
}

void spinFlywheel(void) {

	bool spin = !digitalRead(BUTTON_PIN);
	
	if (spin) {
		lastAngle = flywheelAngle;
	 	flywheelAngle += speed;
	}
}
