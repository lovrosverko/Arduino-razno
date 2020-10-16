/**************************************************************
   Blynk is a platform with iOS and Android apps to control
   Arduino, Raspberry Pi and the likes over the Internet.
   You can easily build graphic interfaces for all your
   projects by simply dragging and dropping widgets.

     Downloads, docs, tutorials: http://www.blynk.cc
     Sketch generator:           http://examples.blynk.cc
     Blynk community:            http://community.blynk.cc
     Social networks:            http://www.fb.com/blynkapp
                                 http://twitter.com/blynk_app

   Blynk library is licensed under MIT license
   This example code is in public domain.

 **************************************************************
   This example shows how to use Arduino with HC-06/HC-05
   Bluetooth 2.0 Serial Port Profile (SPP) module
   to connect your project to Blynk.

   Note: This only works on Android! iOS does not support SPP :(
         You may need to pair the module with your smartphone
         via Bluetooth settings. Default pairing password is 1234

   Feel free to apply it to any other example. It's simple!

   NOTE: Bluetooth support is in beta!

 **************************************************************/

// You could use a spare Hardware Serial on boards that have it (like Mega)
#define BLYNK_PRINT Serial3
#define HC05 Serial3
#include <BlynkSimpleSerialBLE.h>
#include <Servo.h>
Servo servo1;
Servo servo2;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "aa88213c9c6844579a6e51c365f6e19a";
int pos = 90;
BLYNK_WRITE(V0) {
  int b = param[0].asInt();
  // Do something with x and y
  servo1.write(b);
}
BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();

}
BLYNK_WRITE(V2) {
  int a = param[0].asInt();
  // Do something with x and y
  servo2.write(a);
}
void setup()
{
  servo1.attach(8);
  servo1.write(pos);
  servo2.attach(9);
  servo2.write(pos);
  // Debug console
  HC05.begin(9600);  // via BT Module
  Blynk.begin(HC05, auth);
}

void loop()
{
  Blynk.run();
}

