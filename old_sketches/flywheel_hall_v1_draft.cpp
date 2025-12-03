/** FLYWHEEL HALL SENSOR EMULATION V1.0 DRAFT
 * 
 * NOTE: CODE MAY NOT BE FUNCTIONAL, HAS NOT BEEN TESTED OR REVIEWED YET
 * COMMITTING TO REPO FOR PROGRESS TRACKING
 * 
 * 15/11
 * 
 * V0 was not functional but this is a departure from the original concept, so v1.0
 * 
 * CHANGES:
 * - Speed regulation: using angle instead of delay:
 * 	- if potValue > 100: servoAngle =  round(potValue/103.0)
 * 		-> between 1° and 10°, division by 103.0 to avoid max angle values above 10°
 * 
 * CONCEPT:
 * - Since servo sweeps back and forth, Hall sensor response logic needs to change:
 * 	- Issues: sensor triggered twice per 'revolution', time interval of magnet passing below sensor
 * 	would make RPM calculation out of sync with real-life behaviour
 * 	-> Only count first sensor pass then every other pass
 * 
 * - Implemented state machine logic for secondPass:
 * 	-> Was initially inside getRPM, then its own function, then moved secondPass update inside void loop:
 * 		this state needs to be re-checked and tracked at every loop regardless of RPM calculation
 * 	-> Intentional global variables for values that need 'memory' outside of individual loop runs
 * 
 * - Sequential circuit based logic for thisRPM, lastRPM, currentSensorRead, lastSensorRead:
 * 	-> values initialised at 0 = sending initial 0 signal to set circuit
 * 
 * - Defined 'safety range' for pot values to avoid irregularities due to flickering input current:
 * Only values above 100 trigger servo movement
 * 
 * - Split servo movement into separate sweepServo and moveServo functions for logical separation and 
 * modularity for future changes
 * 
 * - Defined easier to implement logic for speed regulation + state machine concept for program execution and
 * cross-loop state tracking -> straightforward refactor process, no conceptual/logic issue points
 * 
 * - Trying out if-based logic for immediate response to speed regulation (previous for loops made servo finish
 * sweep before updating speed)
 * 
 * DATA VISUALISATION:
 * - Signal processing math for RPM, angle and pot value display: so that all variable variations can be visualised
 * at once without overlap + without loss of detail (angle, RPM, and pot value all have different magnitudes)
 * 
 * 19/11 CODE REVIEW & TESTING
 * 
 * Issue noticed:
 * - With speed/step angle varying, sweep math can fail
 * 		- Example:
 * 		- Servo position 176°. Servo speed set to 10° angles. The next loop will send the servo to 186°
 * 		- Question: how does this servo handle 'spill over' angle position commands?
 * - Test: Set servo near 0° and 180°, then test 'spill over' behaviour
 * - Test for various edge cases
 * 
 * Outcomes:
 * 		- Spill over simply ignored -> scale angle steps to 1°-12°
 * 		- Spill over transformed into return angle -> also scale angle steps to 1°-12°
 * 		-> If spill over effect stays within normal behaviour, it doesn't actually matter
 * 		- If spill over causes unexpected/undesired behaviour: go back to delay-based speed control logic
 * 
 * Result: spill-over simply ignored, anything below 0° acts like 0°, same for 180°
 * 
 * -> New math for pot value mapping: potValue/85 (is max 12°)
 * 
 * TO-DO:
 * - Integrate RGB LED for RPM visualisation?
 * 	- 3 phases (based on potentiometer reading):
 * 		- Phase 1: 100 - 400
 * 		- Phase 2: 401 - 700
 * 		- Phase 3: 701 - 1000
 * 
 * 	- Green -> (Turquoise) -> Blue -> (Purple) -> Red
 * 
 * 	- Phase 1: 255 Green, 0 Blue -> 128 Green, 127 Blue
 * 	- Phase 2: 127 Green, 128 Blue -> 0 Green, 255 Blue ; 255 Blue, 0 Red -> 127 Blue, 128 Blue, 127 Red
 * 	- Phase 3: 127 Blue - 128 Red -> 0 Blue - 255 Red
 * 
 * - Consider smoothing pot values in case flickers affect servo movement smoothness?
 * 
 * **/

const int servoPin(7);
const int sensorPin(9);

servo serv0;

int servoAngle(1);

int thisRPM(0);
int lastRPM(0);

unsigned long currentSensorRead(0);
unsigned long lastSensorRead(0);

bool secondPass(false);

void setup () {
	
	// attach servo, set pot as input, set hall sensor as input, begin Serial
	
}

void sweepServo(servo servO, int servoStep);
void moveServo(servo servO);
int getTimeWindow();
int getRPM();


void loop () {
		
	if (analogRead(potPin) < 100) { }
	
	else {
		
		moveServo(serv0);
	}
	
	Serial.print("RPM:");
	Serial.println(getRPM()+300);
	
	Serial.print("Angle:");
	Serial.println(angle*10+150);
	
	Serial.print("Pot value:");
	Serial.println((potValue+1)/100); // +1 because idk if 0/100 should be avoided
	
	// TO-DO: add Hall sensor reading to Serial Plotter display
	
	
	if (!digitalRead(sensorPin) && !secondPass) {
			
		secondPass = true;		
	} else {
		secondPass = false;
	}	
}


void sweepServo(servo servO, int servoStep) {
	
	if (angle <= 180 - servoStep) {
		servO.write(servoAngle);
		servoAngle += servoStep;
		delay(15);
	} else {
		servO.write(servoAngle);
		servoAngle -= servoStep;
		delay(15);
		}
}

void moveServo(servo servO) {
			
	int step = round(analogRead(potPin)/103.0);
		
	sweepServo(servO, step);
}
	
	
int getRPM () {
	
	if (digitalRead(sensorPin) || secondPass) {		
		thisRPM = lastRPM;	
	}
	
	else {						
		thisRPM = 60000 / getTimeWindow();	
		}
	
	int result = round((thisRPM + lastRPM) / 2.0);
	return result;
}

int getTimeWindow() {
	
	int timeWindow(1); // avoid division by 0 in RPM calculation
	
	if (!digitalRead(sensorPin)) {
		
		currentSensorRead = millis();
		
		timeWindow = currentSensorRead - lastSensorRead;
		
		lastSensorRead = currentSensorRead;
		
	} else {
		
		timeWindow = millis() - lastSensorRead;		
	}
	
	return timeWindow;
}
