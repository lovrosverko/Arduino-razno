// this sketch cycles three servos at different rates

#include <ServoTimer2.h>  // the servo library

// define the pins for the servos
#define servoPin1 10
ServoTimer2 servo1;    // declare variables for up to eight servos
int pos = 180;


void setup() {
  servo1.attach(servoPin1);     // attach a pin to the servos and they will start pulsing
  servo1.write(pos);
}


void loop()
{


  for (pos = 750; pos <= 2250; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 2250; pos >= 750; pos -= 10) { // goes from 180 degrees to 0 degrees
    servo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);
  }
  delay(10);
}
