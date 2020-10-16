// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor motorL(1);
AF_DCMotor motorD(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  // turn on motor
  motorL.setSpeed(200);
  motorD.setSpeed(200);

  motorL.run(RELEASE);
  motorD.run(RELEASE);
}

void loop() {
  uint8_t i;

  Serial.print("tick");

  motorL.run(FORWARD);
  motorD.run(FORWARD);
  for (i = 0; i < 255; i++) {
    motorL.setSpeed(i);
    motorD.setSpeed(i);
    delay(10);
  }

  for (i = 255; i != 0; i--) {
    motorL.setSpeed(i);
    motorD.setSpeed(i);
    delay(10);
  }

  Serial.print("tock");

  motorL.run(BACKWARD);
  motorD.run(BACKWARD);
  for (i = 0; i < 255; i++) {
    motorL.setSpeed(i);
    motorD.setSpeed(i);
    delay(10);
  }

  for (i = 255; i != 0; i--) {
    motorL.setSpeed(i);
    motorD.setSpeed(i);
    delay(10);
  }


  Serial.print("tech");
  motorL.run(RELEASE);
  motorD.run(RELEASE);
  delay(1000);
}
