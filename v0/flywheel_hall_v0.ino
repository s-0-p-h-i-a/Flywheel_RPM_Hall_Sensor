#include <Servo.h>

/** FLYWHEEL HALL SENSOR EMULATION V0.0 
 * 
 CHANGES:
  - Speed regulation using angle (0-12)
  CONCEPT:
  - Since servo sweeps back and forth, Hall sensor response logic needs to change:
  	- Issues: sensor triggered twice per 'revolution', time interval of magnet passing below sensor
  	would make RPM calculation out of sync with real-life behaviour
  	-> Only count first sensor pass then every other pass
  
  - State machine logic for secondPass:
  	-> secondPass update inside void loop:
  		this state needs to be re-checked and tracked at every loop regardless of RPM calculation
  	-> Intentional global variables for values that need 'memory' outside of individual loop runs
  
  - Sequential circuit based logic for thisRPM, lastRPM, currentSensorRead, lastSensorRead:
  	-> values initialised at 0 = sending initial 0 signal to set circuit
  
 * TO-DO:
 * - Integrate RGB LED for RPM visualisation?
 * 	- 3 phases (based on int speed):
 * 		- Phase 1: 1-4 (green LED)
 * 		- Phase 2: 4-8 (yellow LED)
 * 		- Phase 3: 8-12 (red LED)
 * 
	PWM LED format:
 * 	- Green -> (Turquoise) -> Blue -> (Purple) -> Red
 * 
 * 	- Phase 1: 255 Green, 0 Blue -> 128 Green, 127 Blue
 * 	- Phase 2: 127 Green, 128 Blue -> 0 Green, 255 Blue ; 255 Blue, 0 Red -> 127 Blue, 128 Blue, 127 Red
 * 	- Phase 3: 127 Blue - 128 Red -> 0 Blue - 255 Red
 * 	- Consider adding hyst
 
 
 03/12
 REFACTOR OF PREVIOUS DRAFT

-> Multi-file structure
-> Separation of concerns: flywheel object, servo control, hall sensor, rpm calculation now have individual libraries
  
  PROCESS:
  
  - Separated into multi-file, custom libs structure
  - Main now empty, rebuild from the inside out starting with main design concept of virtual flywheel, servo 'actuators' and rpm calculation being separate but interacting with each other
  - Intermediary solution before debugging button-based speed regulation: ask user for speed input during setup
  - Servo control funcs don't take the servo as argument anymore: there is one central servo that all servolib funcs can use
	- New logic:
		- int flywheelAngle = central virtual flywheel
		- button input increments/decrements flywheelAngle between 0 and 180
		- serv0.write(flywheelAngle) (with delay check) inside servoMove
  - Issues with linking -> move all files to same folder, change .c to .cpp
	- Multi file structure made it easier to implement new, clearer logic structure and refactor old code accordingly
	- Fixed issue with previous servo control logic:
		- With previous if branch, servo would not sweep back after reaching 180:
			if angle <= 180, then increment
			else decrement
			-> servo stays at 180 due to positions over 180 getting clamped down to 180
		- Current version toggles step direction when servo hits 0 or 180
 * **/

#include "flywheel_lib.h"
#include "hall_lib.h"
#include "plotter_lib.h"
#include "rpmlib.h"
#include "servolib.h"
#include <stdbool.h>
#include <Servo.h>

void setup() {
	
	initFlywheel();
	
	initHall();
	
	initServoSystem();
	
	Serial.begin(9600);
	
	setSpeed();
}

void loop() {

	checkSpeed();
	
	spinFlywheel();
	
	moveServo();

	plotterDisplay();
	
	checkReadState();		
}