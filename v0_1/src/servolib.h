#include "flywheel_lib.h"
#include <Arduino.h>
#include <Servo.h>

#ifndef SERVOLIB_H
#define SERVOLIB_H

typedef enum {
	
	SERVO_MIN = 0,
	SERVO_MAX = 180
} servoAngle;

extern const int SERVO_PIN;

extern const int SERVO_DELAY;
extern unsigned long lastStep;

extern servoAngle servoPosition;

extern Servo serv0;

void initServoSystem(void);

void moveServo(void); 

#endif
