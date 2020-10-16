// this sketch cycles three servos at different rates 

#include <ServoTimer2.h>  // the servo library

// define the pins for the servos
#define rollPin  9
#define pitchPin 10


ServoTimer2 servoRoll;    // declare variables for up to eight servos
ServoTimer2 servoPitch; 

 
void setup() {
  servoRoll.attach(rollPin);     // attach a pin to the servos and they will start pulsing
  servoPitch.attach(pitchPin); 
 
}


// this function just increments a value until it reaches a maximum 
int incPulse(int val, int inc){
   if( val + inc  > 2000 )
      return 1000 ;
   else
       return val + inc;  
}

void loop()
{ 
 int val;
  
   val = incPulse( servoRoll.read(), 1);
   servoRoll.write(val);

   val =  incPulse( servoPitch.read(), 2);
   servoPitch.write(val);
   
   
   delay(10);   
}
