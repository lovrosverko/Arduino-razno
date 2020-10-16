#include <Servo.h>

int ledPin = 13;
int servoPin1 = 2;
int servoPin2 = 3;
int servoPin3 = 4;
int servoPin4 = 5;

// početne pozicije
int startPos1 = 90;
int startPos2 = 90;
int startPos3 = 90;
int startPos4 = 90;

// struktura naredbe
byte inputByte_0=0;
byte inputByte_1=0;
byte inputByte_2=0;
byte inputByte_3=0;
byte inputByte_4=0;
byte inputByte_5=0;
byte inputByte_6=0;
byte inputByte_7=0;
byte inputByte_8=0;
byte inputByte_9=0;
byte inputByte_10=0;
byte inputByte_11=0;
byte inputByte_12=0;
byte inputByte_13=0;
byte inputByte_14=0;
byte inputByte_15=0;
byte inputByte_16=0;
byte inputByte_17=0;
byte inputByte_18=0;
byte inputByte_19=0;
byte inputByte_20=0;
byte inputByte_21=0;



Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int i1;
int i2;
int i3;
int i4;

void setup() {

  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  pinMode (servoPin1, OUTPUT);
  servo1.attach (servoPin1);
  servo1.write(startPos1);

  pinMode (servoPin2, OUTPUT);
  servo2.attach (servoPin2);
  servo2.write(startPos2);

  pinMode (servoPin3, OUTPUT);
  servo3.attach (servoPin3);
  servo3.write(startPos3);

  pinMode (servoPin4, OUTPUT);
  servo4.attach (servoPin4);
  servo4.write(startPos4);

  // Start signal
  for (int a = 250; a > 50; a -= 50)
  {
    digitalWrite(ledPin, HIGH);
    delay(a);
    digitalWrite(ledPin, LOW);
    delay(a);
  }
  for (int a2 = 50; a2 > 10; a2 -= 5)
  {
    digitalWrite(ledPin, HIGH);
    delay(a2);
    digitalWrite(ledPin, LOW);
    delay(a2);
  }

}

void loop() {

  //čitanje naredbi sa serijskog porta
  if (Serial.available() == 5)
  {
    // poruka
    inputByte_0 = Serial.read();
    delay(10);
    inputByte_1 = Serial.read();
    delay(10);
    inputByte_2 = Serial.read();
    delay(10);
    inputByte_3 = Serial.read();
    delay(10);
    inputByte_4 = Serial.read();
  }

  if (Serial.available() == 23)
  {
     
    
    // poruka
    inputByte_0 = Serial.read();
    delay(10);
    inputByte_1 = Serial.read();
    delay(10);
    inputByte_2 = Serial.read();
    delay(10);
    inputByte_3 = Serial.read();
    delay(10);
    inputByte_4 = Serial.read();
    delay(10);
    inputByte_5 = Serial.read();
    delay(10);
    inputByte_6 = Serial.read();
    delay(10);
    inputByte_7 = Serial.read();
    delay(10);
    inputByte_8 = Serial.read();
    delay(10);
    inputByte_9 = Serial.read();
    delay(10);
    inputByte_10 = Serial.read();
    delay(10);
    inputByte_11 = Serial.read();
    delay(10);
    inputByte_12 = Serial.read();
    delay(10);
    inputByte_13 = Serial.read();
    delay(10);
    inputByte_14 = Serial.read();
    delay(10);
    inputByte_15 = Serial.read();
    delay(10);
    inputByte_16 = Serial.read();
    delay(10);
    inputByte_17 = Serial.read();
    delay(10);
    inputByte_18 = Serial.read();
    delay(10);
    inputByte_19 = Serial.read();
    delay(10);
    inputByte_20 = Serial.read();
    delay(10);
    inputByte_21 = Serial.read();
    delay(10);


  }


  // Provjera oznake za početak poruke
  if (inputByte_0 == 16)
  {
    
    // provjera naredbe (127 - naredba za uređaje, 128 - provjera da li je Arduino spojen)
    switch (inputByte_1)
    {
      case 126:
        {
          
        // Pozicija 1
        int stopPos1 = inputByte_2;
        int stopPos2 = inputByte_7;
        int stopPos3 = inputByte_12;
        int stopPos4 = inputByte_17;


        // Servo 1
        if (startPos1 < stopPos1)
        {
          for (i1 = startPos1; i1 < stopPos1; i1++)
          {
            servo1.write(i1);
            delay(25);
          }

        }
        else
        {
          for (i1 = startPos1; i1 > stopPos1; i1--)
          {
            servo1.write(i1);
            delay(25);
          }
        }
        startPos1 = i1;

        // Servo 2
        if (startPos2 < stopPos2)
        {
          for (i2 = startPos2; i2 < stopPos2; i2++)
          {
            servo2.write(i2);
            delay(25);
          }

        }
        else
        {
          for (i2 = startPos2; i2 > stopPos2; i2--)
          {
            servo2.write(i2);
            delay(25);
          }
        }
        startPos2 = i2;

        // Servo 3
        if (startPos3 < stopPos3)
        {
          for (i3 = startPos3; i3 < stopPos3; i3++)
          {
            servo3.write(i3);
            delay(25);
          }

        }
        else
        {
          for (i3 = startPos3; i3 > stopPos3; i3--)
          {
            servo3.write(i3);
            delay(25);
          }
        }
        startPos3 = i3;

        // Servo 4
        if (startPos4 < stopPos4)
        {
          for (i4 = startPos4; i4 < stopPos4; i4++)
          {
            servo4.write(i4);
            delay(25);
          }

        }
        else
        {
          for (i4 = startPos4; i4 > stopPos4; i4--)
          {
            servo4.write(i4);
            delay(25);
          }
        }
        startPos4 = i4;

        delay (1000);

        // Pozicija 2
        stopPos1 = inputByte_3;
        stopPos2 = inputByte_8;
        stopPos3 = inputByte_13;
        stopPos4 = inputByte_18;


        // Servo 1
        if (startPos1 < stopPos1)
        {
          for (i1 = startPos1; i1 < stopPos1; i1++)
          {
            servo1.write(i1);
            delay(25);
          }

        }
        else
        {
          for (i1 = startPos1; i1 > stopPos1; i1--)
          {
            servo1.write(i1);
            delay(25);
          }
        }
        startPos1 = i1;

        // Servo 2
        if (startPos2 < stopPos2)
        {
          for (i2 = startPos2; i2 < stopPos2; i2++)
          {
            servo2.write(i2);
            delay(25);
          }

        }
        else
        {
          for (i2 = startPos2; i2 > stopPos2; i2--)
          {
            servo2.write(i2);
            delay(25);
          }
        }
        startPos2 = i2;

        // Servo 3
        if (startPos3 < stopPos3)
        {
          for (i3 = startPos3; i3 < stopPos3; i3++)
          {
            servo3.write(i3);
            delay(25);
          }

        }
        else
        {
          for (i3 = startPos3; i3 > stopPos3; i3--)
          {
            servo3.write(i3);
            delay(25);
          }
        }
        startPos3 = i3;

        // Servo 4
        if (startPos4 < stopPos4)
        {
          for (i4 = startPos4; i4 < stopPos4; i4++)
          {
            servo4.write(i4);
            delay(25);
          }

        }
        else
        {
          for (i4 = startPos4; i4 > stopPos4; i4--)
          {
            servo4.write(i4);
            delay(25);
          }
        }
        startPos4 = i4;

        delay (1000);

        // Pozicija 3
        stopPos1 = inputByte_4;
        stopPos2 = inputByte_9;
        stopPos3 = inputByte_14;
        stopPos4 = inputByte_19;


        // Servo 1
        if (startPos1 < stopPos1)
        {
          for (i1 = startPos1; i1 < stopPos1; i1++)
          {
            servo1.write(i1);
            delay(25);
          }

        }
        else
        {
          for (i1 = startPos1; i1 > stopPos1; i1--)
          {
            servo1.write(i1);
            delay(25);
          }
        }
        startPos1 = i1;

        // Servo 2
        if (startPos2 < stopPos2)
        {
          for (i2 = startPos2; i2 < stopPos2; i2++)
          {
            servo2.write(i2);
            delay(25);
          }

        }
        else
        {
          for (i2 = startPos2; i2 > stopPos2; i2--)
          {
            servo2.write(i2);
            delay(25);
          }
        }
        startPos2 = i2;

        // Servo 3
        if (startPos3 < stopPos3)
        {
          for (i3 = startPos3; i3 < stopPos3; i3++)
          {
            servo3.write(i3);
            delay(25);
          }

        }
        else
        {
          for (i3 = startPos3; i3 > stopPos3; i3--)
          {
            servo3.write(i3);
            delay(25);
          }
        }
        startPos3 = i3;

        // Servo 4
        if (startPos4 < stopPos4)
        {
          for (i4 = startPos4; i4 < stopPos4; i4++)
          {
            servo4.write(i4);
            delay(25);
          }

        }
        else
        {
          for (i4 = startPos4; i4 > stopPos4; i4--)
          {
            servo4.write(i4);
            delay(25);
          }
        }
        startPos4 = i4;

        delay (1000);

        // Pozicija 4
        stopPos1 = inputByte_5;
        stopPos2 = inputByte_10;
        stopPos3 = inputByte_15;
        stopPos4 = inputByte_20;


        // Servo 1
        if (startPos1 < stopPos1)
        {
          for (i1 = startPos1; i1 < stopPos1; i1++)
          {
            servo1.write(i1);
            delay(25);
          }

        }
        else
        {
          for (i1 = startPos1; i1 > stopPos1; i1--)
          {
            servo1.write(i1);
            delay(25);
          }
        }
        startPos1 = i1;

        // Servo 2
        if (startPos2 < stopPos2)
        {
          for (i2 = startPos2; i2 < stopPos2; i2++)
          {
            servo2.write(i2);
            delay(25);
          }

        }
        else
        {
          for (i2 = startPos2; i2 > stopPos2; i2--)
          {
            servo2.write(i2);
            delay(25);
          }
        }
        startPos2 = i2;

        // Servo 3
        if (startPos3 < stopPos3)
        {
          for (i3 = startPos3; i3 < stopPos3; i3++)
          {
            servo3.write(i3);
            delay(25);
          }

        }
        else
        {
          for (i3 = startPos3; i3 > stopPos3; i3--)
          {
            servo3.write(i3);
            delay(25);
          }
        }
        startPos3 = i3;

        // Servo 4
        if (startPos4 < stopPos4)
        {
          for (i4 = startPos4; i4 < stopPos4; i4++)
          {
            servo4.write(i4);
            delay(25);
          }

        }
        else
        {
          for (i4 = startPos4; i4 > stopPos4; i4--)
          {
            servo4.write(i4);
            delay(25);
          }
        }
        startPos4 = i4;

        delay (1000);

        // Pozicija 5
        stopPos1 = inputByte_6;
        stopPos2 = inputByte_11;
        stopPos3 = inputByte_16;
        stopPos4 = inputByte_21;


        // Servo 1
        if (startPos1 < stopPos1)
        {
          for (i1 = startPos1; i1 < stopPos1; i1++)
          {
            servo1.write(i1);
            delay(25);
          }

        }
        else
        {
          for (i1 = startPos1; i1 > stopPos1; i1--)
          {
            servo1.write(i1);
            delay(25);
          }
        }
        startPos1 = i1;

        // Servo 2
        if (startPos2 < stopPos2)
        {
          for (i2 = startPos2; i2 < stopPos2; i2++)
          {
            servo2.write(i2);
            delay(25);
          }

        }
        else
        {
          for (i2 = startPos2; i2 > stopPos2; i2--)
          {
            servo2.write(i2);
            delay(25);
          }
        }
        startPos2 = i2;

        // Servo 3
        if (startPos3 < stopPos3)
        {
          for (i3 = startPos3; i3 < stopPos3; i3++)
          {
            servo3.write(i3);
            delay(25);
          }

        }
        else
        {
          for (i3 = startPos3; i3 > stopPos3; i3--)
          {
            servo3.write(i3);
            delay(25);
          }
        }
        startPos3 = i3;

        // Servo 4
        if (startPos4 < stopPos4)
        {
          for (i4 = startPos4; i4 < stopPos4; i4++)
          {
            servo4.write(i4);
            delay(25);
          }

        }
        else
        {
          for (i4 = startPos4; i4 > stopPos4; i4--)
          {
            servo4.write(i4);
            delay(25);
          }
        }
        startPos4 = i4;

        delay (1000);

        break;
        }
      case 127:
        //provjera za koga je naredba poslana (1 - servo 1, 5 - LED)
        switch (inputByte_2)
        {
          case 1:
            {
              // inputByte_3 sadrži željeni kut servo motora
              int stopPos1 = inputByte_3;

              if (startPos1 < stopPos1)
              {
                for (i1 = startPos1; i1 < stopPos1; i1++)
                {
                  servo1.write(i1);
                  delay(25);
                }

              }
              else
              {
                for (i1 = startPos1; i1 > stopPos1; i1--)
                {
                  servo1.write(i1);
                  delay(25);
                }
              }
              startPos1 = i1;

              break;
            }
          case 2:
            {
              // inputByte_3 sadrži željeni kut servo motora
              int stopPos2 = inputByte_3;

              if (startPos2 < stopPos2)
              {
                for (i2 = startPos2; i2 < stopPos2; i2++)
                {
                  servo2.write(i2);
                  delay(25);
                }

              }
              else
              {
                for (i2 = startPos2; i2 > stopPos2; i2--)
                {
                  servo2.write(i2);
                  delay(25);
                }

              }
              startPos2 = i2;
              break;
            }
          case 3:
            {
              // inputByte_3 sadrži željeni kut servo motora
              int stopPos3 = inputByte_3;

              if (startPos3 < stopPos3)
              {
                for (i3 = startPos3; i3 < stopPos3; i3++)
                {
                  servo3.write(i3);
                  delay(25);
                }

              }
              else
              {
                for (i3 = startPos3; i3 > stopPos3; i3--)
                {
                  servo3.write(i3);
                  delay(25);
                }
              }
              startPos3 = i3;
              break;
            }
          case 4:
            {
              // inputByte_3 sadrži željeni kut servo motora
              int stopPos4 = inputByte_3;
              if (startPos4 < stopPos4)
              {
                for (i4 = startPos4; i4 < stopPos4; i4++)
                {
                  servo4.write(i4);
                  delay(25);
                }

              }
              else
              {
                for (i4 = startPos4; i4 > stopPos4; i4--)
                {
                  servo4.write(i4);
                  delay(25);
                }
              }
              startPos4 = i4;
              break;
            }
          case 5:
            // inputByte_3 šalje HIGH (255) ili LOW (!255) naredbu
            if (inputByte_3 == 255)
            {
              digitalWrite(ledPin, HIGH);
              Serial.print("Pali Meho!!!");
              break;
            }
            else
            {
              digitalWrite(ledPin, LOW);
              Serial.print("Gasi Meho!!!");
              break;
            }
            break;
        }
        break;
      case 128:
      {
        //Potvrda nazočnosti
        Serial.print("Arduino je ovdje!");
        break;
      }


    }

        //čišćenje poruke
             inputByte_0=0;
     inputByte_1=0;
     inputByte_2=0;
     inputByte_3=0;
     inputByte_4=0;
     inputByte_5=0;
     inputByte_6=0;
     inputByte_7=0;
     inputByte_8=0;
     inputByte_9=0;
     inputByte_10=0;
     inputByte_11=0;
     inputByte_12=0;
     inputByte_13=0;
     inputByte_14=0;
     inputByte_15=0;
     inputByte_16=0;
     inputByte_17=0;
     inputByte_18=0;
     inputByte_19=0;
     inputByte_20=0;
     inputByte_21=0;
        //poruka aplikaciji da je spreman za novu naredbu

        Serial.print(" - Spreman sam za rad!");
    }
  }
