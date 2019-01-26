#include <Servo.h>

Servo servo;  // create servo object to control a servo

void setup() {
  servo.attach(7);  // attaches the servo on pin 7 to the servo object
}

void loop() {
  servo.write(0);
  delay(1000);
  servo.write(180);
  delay(1000);
}
