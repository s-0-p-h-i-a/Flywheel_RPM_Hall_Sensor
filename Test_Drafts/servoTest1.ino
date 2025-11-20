#include <Servo.h>

const int servoPin(7);
Servo serv0;

void setup() {
	serv0.attach(servoPin);
}

void moveServo();

void loop() {

  moveServo();
  
}
   
void moveServo() {
	
	int pos(0);
	
	/**for (pos = 0; pos < 180; pos += 10) { 

    serv0.write(pos);
    delay(15);
  }

  for (pos = 180; pos >= 0; pos -= 10) { 

    serv0.write(pos);           
    delay(15);
  }**/
}
