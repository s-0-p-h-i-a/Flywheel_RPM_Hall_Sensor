#include <Servo.h>
#include <map>

const int potPin(A0);
const int servoPin(7);
Servo serv0;

int servoAngle(0);
int servoDirection(1);
unsigned long lastServoMove(0);

int lastAngle(0);
int secondLastAngle(0);
int currentAngle(0);

void setup() {
	serv0.attach(servoPin);
	pinMode(potPin, INPUT);
	Serial.begin(9600);
}

void moveServo1(unsigned long now);
void moveServo2(unsigned long now, int wait);

void loop() {
	
	unsigned long now = millis();

	int potLevel = analogRead(potPin);
	int wait(20);
	
	// "Zero" range to account for current flicker
	if (potLevel == 0 || potLevel < 5) { }
	
	else {
		
		Serial.print("Pot ON");
		
		// Level 1 continuous speed movement
		moveServo1(now);
		
		// Level 2 map speed to pot level
		wait = 80 - potLevel * (60.0/1023.0);
		moveServo2(now, wait);
	} 
}
 
// 10° steps 
void moveServo1(unsigned long now) {
	
	if (now - lastServoMove >= 20) { 
		servoAngle += servoDirection;

	if (servoAngle >= 0 && servoAngle < 180) {
		servoDirection = 10;
		}
    else if (servoAngle >= 180) {
		servoDirection = -10;
		}

    serv0.write(servoAngle);
    lastServoMove = now;
  }
}

// 12° steps
void moveServo2(unsigned long now, int wait) {
	
	bool moveNow = now - lastServoMove >= wait;
	
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

