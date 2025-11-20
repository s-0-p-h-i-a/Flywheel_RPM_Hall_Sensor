/**
 * 19/11 Testing New servoMove Logic
 * 
 * - Using global variables to track state+store values throughout loops
 * - Using void loop and int newStep instead of for loop
 * - Direction variable sets/resets every time servo reaches min or max angle

 Testing:
 - Issue: program acts as if both push buttons are always pressed
		-> Test buttons for responsiveness using LED
			- LED has dim light, no response to button: move button pins to other part of sensor shield
		-> Test vcc and gnd rails connection on breadboard
		-> Rewire: make sure there is common GND, check vcc and signal paths
 * 
 * 
 * 
 * **/





#include <Servo.h>

const int servoPin(7);
const int ON_button(12);
const int speedUpButton(11);
const int slowDownButton(8);
const int ledPin(6);
Servo serv0;

bool faster(false);
bool slower(false);
bool spin(false);
int angle(0);
int step(0);
int direction(1);

void setup() {
	serv0.attach(servoPin);
	pinMode(ON_button, INPUT);
	pinMode(speedUpButton, INPUT);
	pinMode(slowDownButton, INPUT);
	Serial.begin(9600);
}

int newStep();
void moveServo();

void loop () {
	
	faster = digitalRead(speedUpButton);
	slower = digitalRead(slowDownButton);
	spin = !digitalRead(ON_button);

	if (faster) {
		digitalWrite(ledPin, HIGH);
	}
	
	/**if (spin) {
		//Serial.println("ON");
		moveServo();
	}

	if (faster) {
		Serial.println("Faster");
	}

	if (slower) {
		Serial.println("Slower");
	}**/
	
	// Direction check + reset if necessary
	if (angle == 180) {
		direction = -1;
	} else if (angle == 0) {
		direction = 1;
	}		
}

int newStep() {
	
	if (faster) {
		if (angle == 12) { ;}
		else {
			++step;
		}
	}
	
	else if (slower) {
		if (angle == 0) { ;}
		else {
			--step;
		}	
	}	
}

void moveServo() {
	
	angle = angle + direction*step;
	serv0.write(angle);
	delay(15);
}
