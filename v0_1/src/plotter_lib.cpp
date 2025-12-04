#include "global.h"
#include "flywheel_lib.h"
#include "plotter_lib.h"
#include "hall_lib.h"
#include "rpmlib.h"
#include <Arduino.h>

const int RPM_SHIFTUP = 300;
const int ANGLE_SHIFTUP = 150;
const int ANGLE_AMPLIFY = 10;
const int HALL_AMPLIFY = 80;
const int SPEED_AMPLIFY = 10;

void plotterDisplay(void) {

	Serial.print("RPM:");
	Serial.println(RPM);

	Serial.print("Speed:");
	Serial.println(speed);
	
	Serial.print("Angle:");
	Serial.println(flywheelAngle);
	
	Serial.print("Hall Sensor:");
	Serial.println(hallON);
}
