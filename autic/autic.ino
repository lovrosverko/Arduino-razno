#include <AFMotor.h> // import the Adafruit Motor Shield library
#include <ServoTimer2.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#define servoPin 10

RH_ASK driver (2000, 2, 0, 1);

ServoTimer2 servo1;

AF_DCMotor motor1(4); // Set up a DC Motor named 'motor1' on the channel M1 of the motor shield
AF_DCMotor motor2(3); // Set up a DC Motor named 'motor2' on the channel M2 of the motor shield

char var;
void setup()
{
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600); // begin serial communication @ 9600 baud
  pinMode(LED_BUILTIN, OUTPUT);
  motor1.setSpeed(255); // Set the speed of motor1 to 125. You can choose any speed from 0 to 255
  motor2.setSpeed(255); // Set the speed of motor2 to 125
  servo1.attach(servoPin);
  servo1.write(1500);
  if (!driver.init())
    Serial.println("init failed");
}
void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) // Non-blocking
  {
    digitalWrite(LED_BUILTIN, HIGH);
    //int i;
    // Message with a good checksum received, dump it.
    driver.printBuffer("Got: ", buf, buflen);
    delay(500);
    String rcv;
    for (int i = 0; i < buflen; i++) {
      rcv += (char)buf[i];
    }
    Serial.println(rcv);
    digitalWrite(LED_BUILTIN, LOW);

    switch ((char)buf[0]) {
      case 's':
        stani();
        break;
      case 'w':
        naprijed();
        break;
      case 'a':
        lijevo();
        break;
      case 'd':
        desno();
        break;
      case 'e':
        naprijedDesno();
        break;
      case 'q':
        naprijedLijevo();
        break;
      case 'x':
        natrag();
        break;
      case 'y':
        natragLijevo();
        break;
      case 'c':
        natragDesno();
        break;
      default:
        // if nothing else matches, do the default
        // default is optional
        break;
    }
  }
  else {

  }

}


void naprijed() {
  servo1.write(1500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void natrag() {
  servo1.write(1500);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void stani() {
  servo1.write(1500);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
void desno() {
  servo1.write (1900);
}

void lijevo() {
  servo1.write (1100);
}

void naprijedDesno() {
  servo1.write (1900);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void natragDesno() {
  servo1.write (1100);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void naprijedLijevo() {
  servo1.write (1100);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void natragLijevo() {
  servo1.write (1900);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}
