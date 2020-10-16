
#include <Servo.h>


#define servoPin 15
#define echoPin 12
#define trigPin 14



Servo armServo;

long duration;
int distance;


int readPing() { // read the ultrasonic sensor distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;

  return distance;
}


void setup() {
  armServo.attach(servoPin);
  armServo.write(143);
  delay(1000);
  pinMode(echoPin, INPUT)  ;
  pinMode(trigPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int udaljenost = readPing();
  Serial.print("Udaljenost: ");
  Serial.println(udaljenost);


  if (udaljenost < 5) {
    delay(500);
    armServo.write(40);
    delay(500);
    armServo.write(143);
    delay(1000);
  }

}

