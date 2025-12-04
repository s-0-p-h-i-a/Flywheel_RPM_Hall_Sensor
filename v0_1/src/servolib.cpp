#include "global.h"
#include "flywheel_lib.h"
#include "servolib.h"
#include <Arduino.h>
#include <Servo.h>

const int SERVO_PIN = 11;

const int SERVO_DELAY = 15;
unsigned long lastStep = 0;

Servo serv0;

void initServoSystem(void) {
	
	serv0.write(ANGLE_ZERO);
}

void moveServo(void) {
	
	if (now - lastStep >= SERVO_DELAY) {
		
		// update timestamp
		lastStep = now;
			
		serv0.write(flywheelAngle);
	}
}
