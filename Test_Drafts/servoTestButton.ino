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
	
	bool buttonState = digitalRead(buttonPin);

	if (!buttonState) {
		Serial.print("Pressed");
		moveServo();    
	} 
}
   
void moveServo() {
	
	for (int pos(0); pos < 180; pos += 10) { 

    serv0.write(pos);
    delay(15);
  }

  for (int pos(180); pos >= 0; pos -= 10) { 

    serv0.write(pos);           
    delay(15);
  }
}

