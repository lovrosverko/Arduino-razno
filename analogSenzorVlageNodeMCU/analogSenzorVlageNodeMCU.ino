#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// On ESP8266:  0 - 1023 maps to 0 - 1 volts
#define VOLTAGE_MAX 1.0
#define VOLTAGE_MAXCOUNTS 1023.0

const char* ssid = "FunkyMonkey";
const char* password = "7755747C77F0";
int status = WL_IDLE_STATUS;
WiFiClient client;

WiFiServer server(80);

int svjetlostD;
int svjetlostA;
int senzor = 13;


unsigned long weatherStationChannelNumber = 12397;
unsigned int temperatureFieldNumber = 4;

unsigned long myChannelNumber = 196696;
const char * myWriteAPIKey = "AVELG5KLKFHOT2FJ";

void setup() {
  Serial.begin(9600);
  pinMode (senzor, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {
  // Check if a client has connected
  svjetlostD = digitalRead(senzor);
  delay(100);
  svjetlostA = analogRead(senzor);

  int mapirano = map(svjetlostA, 0, 1023, 1, 0);
  Serial.print("Analog= "); Serial.println(svjetlostA);
  Serial.print("Digital= "); Serial.println(svjetlostD);
  Serial.print("Mapirano= "); Serial.println(mapirano);
  Serial.println(" "); Serial.println(" ");
  delay (100);
  ThingSpeak.writeField(myChannelNumber, 1, mapirano, myWriteAPIKey);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(15000);


}
