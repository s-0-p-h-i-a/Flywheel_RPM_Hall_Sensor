#include <Servo.h>

const int buttonPin(12);
const int servoPin(7);
Servo serv0;

void setup() {
	serv0.attach(servoPin);
	pinMode(buttonPin, INPUT);
	Serial.begin(9600);
}

void moveServo();

void loop() {
	
	serv0.write(-50);
	
}

