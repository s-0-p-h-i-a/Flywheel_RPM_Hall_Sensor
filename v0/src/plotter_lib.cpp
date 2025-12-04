#include "flywheel_lib.h"
#include "plotter_lib.h"
#include "hall_lib.h"
#include "rpmlib.h"
#include <Arduino.h>

extern const int RPM_SHIFTUP = 300;
extern const int ANGLE_SHIFTUP = 150;
extern const int ANGLE_AMPLIFY = 10;
extern const int HALL_AMPLIFY = 80;

void plotterDisplay(void) {

	Serial.print("RPM:");
	Serial.println(getRPM()+RPM_SHIFTUP);
	
	Serial.print("Angle:");
	Serial.println(flywheelAngle*ANGLE_AMPLIFY+ANGLE_SHIFTUP);
	
	Serial.print("Hall Sensor:");
	Serial.println(hallON*HALL_AMPLIFY);
}
