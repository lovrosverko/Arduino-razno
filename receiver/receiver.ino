#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#define GND 2
#define VCC 5
RH_ASK driver (2000, 2, 0, 1);

void setup()
{
  pinMode (GND, OUTPUT);
  pinMode (VCC, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite (VCC, HIGH);
  digitalWrite (GND, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);  // Debugging only
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
    digitalWrite(LED_BUILTIN, LOW);
  }
  else {

  }
}
