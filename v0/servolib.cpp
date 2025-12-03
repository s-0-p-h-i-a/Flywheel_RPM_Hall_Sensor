#include "flywheel_lib.h"
#include "servolib.h"
#include <Arduino.h>
#include <Servo.h>

const int SERVO_PIN = 7;

const int SERVO_DELAY = 15;
unsigned long lastStep = 0;

servoAngle servoPosition = 0;

Servo serv0;

void initServoSystem(void) {
	
	servoPosition = SERVO_MIN;
	
	serv0.attach(SERVO_PIN);
	
	pinMode(BUTTON_PIN, INPUT);
	
}

void moveServo(void) {
	
	unsigned long thisTime = millis();
	
	if (thisTime - lastStep >= SERVO_DELAY) {
		
		// update timestamp
		lastStep = thisTime;
			
		serv0.write(flywheelAngle);
	}
}
