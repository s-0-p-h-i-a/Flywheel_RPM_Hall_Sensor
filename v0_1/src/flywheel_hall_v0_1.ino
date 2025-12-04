/*

04/12

- Main refactor goal: introducing global int now to address desync/data flow/timing issues, general code clean-up, where possible porting files back to C

- canAccelerate/Decelerate functions previously used local time=millis() to prevent constant input when button is pressed using a preset delay time. the previous local millis() may have been causing timing issues

- Same for getRPM:

 void getRPM(void) {

	if (hallON && secondPass) {;}
	else if (getTimeWindow() > 10000) {
		RPM = 0;
	} else {
		lastRPM = thisRPM;			
		thisRPM = round(getTimeWindow()/60000.0);	// just in case to avoid div by 0
	}
	RPM = round((thisRPM + lastRPM) / 2.0);
}

	-> calling getTimeWindow twice, which itself uses a local time=millis()
	
- Now getRPM uses the global int now

- flywheel_lib.c had to become .cpp because the buttons are meant to control the flywheel state machine, which then works as an actuator for the servo

- handleEdgeAngles, previously updateAngles, was only being run inside spinFlyWheel. But the angle characteristic is equivalent to the state itself. Moved it to main loop

- spinFlyWheel calls updateSpeed, which calls canAccelerate/Decelerate, which use the global now. Moving updateSpeed call to main loop

- Main loop function call order change: 
	- get now timestamp
	- handle edge case angles
	- update speed -> spin flywheel -> move servo
	- check+update sensor read state -> get rpmTimeWindow with sensor data -> get rpm with time window sensor data

- Now getRPM uses central time window value instead of calling gettimewindow (which itself uses data from sensor input)
	
- Assign int now at the start or end of main loop?
	- I need the same int now to be available to all the functions used in one individual loop
	-> update int now at the end of loop so it is ready BEFORE the next round of function calls

- Currently getTimeWindow is only called inside getRPM. getTimeWindow itself uses lastSensorRead/currentSensorRead and now.
	- idea: introduce global timeWindow variable, can also be used to have a bool isIdle
	- also introduced const idleInterval to drive isIdle
	
- new checkFlywheelState updates idle status each loop

- Somehow wasn't calling getRPM anywhere before adding it today ??

- Now using global timeWindow defined in global.h: now loop with global int time and long timeWindow acts as a centralised clock/timekeeper

- Moved all buttons pinMode to setup, they were previously in initFlywheel but now initFlywheel is just for the actual flywheel object, pinning buttons is part of the general arduino interface setup

- Other changes: now working outside Arduino IDE, compiling/building via terminal using gcc avr and a makefile

*/

#include "global.h"
#include "flywheel_lib.h"
#include "hall_lib.h"
#include "plotter_lib.h"
#include "rpmlib.h"
#include "servolib.h"
#include <stdbool.h>
#include <Arduino.h>
#include <Servo.h>

void setup() {
	
	initFlywheel();
	
	initHall();

	serv0.attach(SERVO_PIN);
	
	pinMode(BUTTON_PIN, INPUT);
	pinMode(FASTER_BUTTON_PIN, INPUT);
	pinMode(SLOWER_BUTTON_PIN, INPUT);

	initServoSystem();
	
	Serial.begin(9600);
	
}

void loop() {
	
	handleEdgeAngles();
	
	updateSpeed();
	
	spinFlywheel();
	
	moveServo();
	
	checkReadState();
	
	getTimeWindow();
	
	checkFlywheelState();
	
	getRPM();

	plotterDisplay();
	
	now = millis();
			
}

