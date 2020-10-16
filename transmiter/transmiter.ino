#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver (2000, 4, 2, 5);
int VRx = A0;
int VRy = A1;
int SW = 2;

int xPosition = 0;
int yPosition = 0;
int mapX = 0;
int mapY = 0;

const char *naprijed = "w";
const char *stani = "s";
const char *desno = "d";
const char *lijevo = "a";
const char *lijevoNaprijed = "q";
const char *lijevoNatrag = "y";
const char *desnoNaprijed = "e";
const char *desnoNatrag = "c";
const char *natrag = "x";

void setup()
{
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  if (!driver.init())
    Serial.println("Radio veza ne radi!");
}

void loop()
{
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  mapX = map(xPosition, 0, 1023, 0, 10);
  mapY = map(yPosition, 0, 1023, 0, 10);

  if (mapX == 5 && mapY == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Naprijed");
    driver.send((uint8_t *)naprijed, strlen(naprijed));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (mapX == 5 && mapY == 5) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Stop");
    driver.send((uint8_t *)stani, strlen(stani));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (mapX == 10 && mapY == 5) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Desno");
    driver.send((uint8_t *)desno, strlen(desno));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (mapX == 0 && mapY == 5) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Lijevo");
    driver.send((uint8_t *)lijevo, strlen(lijevo));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (mapX == 0 && mapY == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Lijevo naprijed");
    driver.send((uint8_t *)lijevoNaprijed, strlen(lijevoNaprijed));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (mapX == 0 && mapY == 10) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Lijevo natrag");
    driver.send((uint8_t *)lijevoNatrag, strlen(lijevoNatrag));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (mapX == 10 && mapY == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Desno naprijed");
    driver.send((uint8_t *)desnoNaprijed, strlen(desnoNaprijed));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (mapX == 10 && mapY == 10) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Desno natrag");
    driver.send((uint8_t *)desnoNatrag, strlen(desnoNatrag));
    driver.waitPacketSent();
    delay(200);
  }
  else if (mapX == 5 && mapY == 10) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Natrag");
    driver.send((uint8_t *)natrag, strlen(natrag));
    driver.waitPacketSent();
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
