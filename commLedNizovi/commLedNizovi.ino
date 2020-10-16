#include <Servo.h>

int servoPin1 = 2;
int ledPin1 = 6;
int ledPin2 = 7;
int ledPin = 13;

// početne pozicije
int startPos1 = 90;

// struktura naredbe
byte inputByte[5];

Servo servo1;

int i1;

void setup() {

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(9600);

  pinMode (servoPin1, OUTPUT);
  servo1.attach (servoPin1);
  servo1.write(startPos1);

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
    inputByte[0] = Serial.read();
    delay(10);
    inputByte[1] = Serial.read();
    delay(10);
    inputByte[2] = Serial.read();
    delay(10);
    inputByte[3] = Serial.read();
    delay(10);
    inputByte[4] = Serial.read();
  }

  // Provjera oznake za početak poruke
  if (inputByte[0] == 16)
  {

    // provjera naredbe (127 - naredba za uređaje, 128 - provjera da li je Arduino spojen)
    switch (inputByte[1])
    {
      case 127:
        {
          //provjera za koga je naredba poslana (1 - servo 1, 5 - LED)
          switch (inputByte[2])
          {
            case 1:
              {
                // inputByte[3] sadrži željeni kut servo motora
                int stopPos1 = inputByte[3];

                if (startPos1 < stopPos1)
                {
                  for (i1 = startPos1; i1 < stopPos1; i1++)
                  {
                    servo1.write(i1);
                    delay(5);
                  }

                }
                else
                {
                  for (i1 = startPos1; i1 > stopPos1; i1--)
                  {
                    servo1.write(i1);
                    delay(5);
                  }
                }
                startPos1 = i1;
                Serial.print("Vrti servo, Meho!!!");
                break;
              }
            case 5:
              // inputByte[3] šalje HIGH (255) ili LOW (!255) naredbu
              if (inputByte[3] == 255)
              {
                digitalWrite(ledPin1, HIGH);
                Serial.print("Pali LED 1, Meho!!!");
                break;
              }
              else
              {
                digitalWrite(ledPin1, LOW);
                Serial.print("Gasi LED 1, Meho!!!");
                break;
              }
              break;
            case 6:
              // inputByte[3] šalje HIGH (255) ili LOW (!255) naredbu
              if (inputByte[3] == 255)
              {
                digitalWrite(ledPin2, HIGH);
                Serial.print("Pali LED 2, Meho!!!");
                break;
              }
              else
              {
                digitalWrite(ledPin2, LOW);
                Serial.print("Gasi LED 2, Meho!!!");
                break;
              }
              break;
          }
          break;
        }
      case 128:
        {
          //Potvrda nazočnosti
          Serial.print("Arduino je ovdje!");
          break;
        }


    }

    //čišćenje poruke
    inputByte[0] = 0;
    inputByte[1] = 0;
    inputByte[2] = 0;
    inputByte[3] = 0;
    inputByte[4] = 0;

    //poruka aplikaciji da je spreman za novu naredbu

    Serial.print(" :: Spreman sam za rad!");
  }
}
