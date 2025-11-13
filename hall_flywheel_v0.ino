/*
TEST / PROOF OF CONCEPT: hall flywheel sensor emulator

13/10

GOAL:
- First implementation / test of servo + push button + hall sensor setup
- Write base program as basis for future developments

SETUP:
- Arduino uno board
- V5 sensor shield
- Digital push button module
- Digital hall sensor module
- SG90 servo motor
- Cardboard disc with one magnet on the edge, mounted on the servo

BEHAVIOUR:
- Servo motor: continuous speed
- Push button: controls motor, pressed = ON, not pressed = OFF
- Hall sensor: reads when the magnet passes by
- Track time between current and previous instance of magnet detection by the hall sensor
- Calculate current rpm based on time interval: 60 000ms / currentTimeWindow
- Calculate average with last rpm to smooth signal
- Display rpm in serial plotter

LOG:
- Considered scope for the variables involved to decide whether to declare globally or in loop
- Hall sensor does react to fridge magnet, experimented with different positions:
  - Magnet passing right below the module gives most reliable sensor reaction
  - Researched why: conductive elements on module board may influence magnet EMF
  - Looked at serial plotter reaction to magnet movement
  - Tested RPM calculation + result display via plotter and monitor with manual movement
- Servo not working:
  - Hall sensor OK
  - Button OK
  - Must be the servo
- Initially assumed blocking servo moving code (for loop) may be causing issues
- Tested setup with simple servo-only code: still did not work
- Issue: did not know servo cannot be powered via arduino board, do not have the material to connect it to usb adapter
  - Researched powering and risk issues:
    - Arduino board power not sufficient to properly power servo, may affect other parts of hardware setup
    - Have the material to power modules via 9v battery, but voltage too high for servo
  - No terminal adapter for usb at hand -> cut and stripped usb wire to power servo via wall adapter
- Setup not working: issue narrowed down to servo motor no getting enough current
- Will rewire tomorrow then start again

- Next day, considered potential servo powering issues across the setup, narrowed down to two main probable issues:
  1. Stripped usb cable: insufficient breadboard slot contact and too many broken copper wires
  2. Wall adapter known to have fluctuating current: when used to charge phone, charge times vary a lot and can be very long
- Cut and stripped cable repeatedly until figuring out proper technique to strip without breaking copper wires
- Contacted electronics repair shop to ask about copper soldering to strengthen the wires: they do not do this specific type of soldering at their shop

13/11
- Quick code clean up before continuing with hardware debugging:
	-> fixed function/variable names, redundant servo moving logic,
	adjusted servo steps from 1° to 12° for max speed as default, formatting
	-> implemented new servo moving math to account for 180 not being a multiple of 12

*/

#include <map>
#include <Servo.h>

const int servoPin(7);
const int sensorPin(3);
const int buttonPin(12);

Servo serv0;

unsigned long lastSensorRead(0);
unsigned long lastTimeWindow(millis());
unsigned long averageWindow(0);
unsigned long currentSensorRead(0);
unsigned long currentTimeWindow(0);
float currentRPM(0);
float averageRPM(0);

unsigned long lastServoMove = 0;

int currentAngle(0);
int servoAngle = 0;
int servoDirection = 12;

int secondLastAngle(0);
int lastAngle(0);
			

void setup() {

  pinMode(buttonPin, INPUT);
  pinMode(sensorPin, INPUT);
  serv0.attach(servoPin);

  Serial.begin(9600);

}

void moveServo(unsigned long now);

void loop() {

  unsigned long now = millis();

  bool buttonState = digitalRead(buttonPin);

  if (buttonState) {
    Serial.print("Pressed");
    moveServo(now);    
  }

  int sensorState = digitalRead(sensorPin);

  if (sensorState == LOW) {

    currentSensorRead = millis();
    currentTimeWindow = currentSensorRead - lastSensorRead;
    
    if (lastTimeWindow == 0) { lastTimeWindow = currentTimeWindow;}
    averageWindow = (currentTimeWindow + lastTimeWindow) / 2.0;
    
    if (averageRPM == 0) { averageRPM = currentRPM;}
    currentRPM = 60000.0 / averageWindow;
    averageRPM = (currentRPM + averageRPM) / 2.0;

	}
  }

	Serial.print("RPM:");
	Serial.println(averageRPM);
	Serial.print("Sensor:");
	Serial.println(digitalRead(sensorPin) * 100);

	lastSensorRead = currentSensorRead;
	lastTimeWindow = currentTimeWindow;
  
}

void moveServo(unsigned long now) {
	
	bool moveNow = now - lastServoMove >= 20;
	
	if (moveNow) {
		
		// First loop
		if ( lastAngle == secondLastAngle) {
			currentAngle = 12;
		}
		
		// Forward motion: 0 -> 180
		if (lastAngle - secondLastAngle > 0) {
			
			if (lastAngle >= 168 && lastAngle < 180) {
				serv0.write(180);
				currentAngle = 12 - 180 + lastAngle;
				serv0.write(currentAngle);
			}
			else {
				currentAngle = lastAngle + 12;
				serv0.write(currentAngle);
			}	
		}
		
		// Backward motion: 180 -> 0
		else if (lastAngle - secondLastAngle < 0) {
			
			if (lastAngle < 12) {
				serv0.write(0);
				currentAngle = 12 - lastAngle;	
			}
			else {
				currentAngle = lastAngle - 12;
				serv0.write(currentAngle);
			}	
		}
	
	secondLastAngle = lastAngle;
	lastAngle = currentAngle;	
	lastServoMove = now;
	}
}
