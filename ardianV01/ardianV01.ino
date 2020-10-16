#include <WiFi101.h>
#include "ThingSpeak.h"

#define senzorVlage A1
#define pumpa 4
#define prekidac 1


const char* ssid = "iPhone od: Ardijan";
const char* password = "12345678";
int status = WL_IDLE_STATUS;
WiFiClient client;

WiFiServer server(80);

unsigned long myChannelNumber = 1048296;
const char * myWriteAPIKey = "XYBNU8TZEWQ9V7IP";

int vlaga;
int pumpaOn = 1;
int pumpaOff = 0;

int odgovor;

void setup() {
  Serial.begin(115200);
  pinMode (senzorVlage, INPUT);
  pinMode (prekidac, OUTPUT);
  pinMode (pumpa, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(prekidac, LOW);
  digitalWrite(pumpa, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Test");


  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);

}

void loop() {

  digitalWrite(prekidac, HIGH);
  delay(1000);
  vlaga = analogRead(senzorVlage);
  digitalWrite (prekidac, LOW);
  int mapirano = map(vlaga, 0, 300, 0, 100);
  Serial.print("Vlaga= "); Serial.println(mapirano);
  Serial.print("Analog input= "); Serial.println(vlaga);
  Serial.println(" "); Serial.println(" ");
  delay (500);

  if (mapirano < 60) {
    ThingSpeak.setField(1, mapirano);
    ThingSpeak.setField(2, pumpaOn);
    Serial.println("Pumpa ON");
    odgovor = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (odgovor == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(odgovor));
    }
    delay(100);
    digitalWrite(pumpa, HIGH);
    for (int j = 15; j > 0; j--) {
      delay(1000);
      Serial.println(j);
    }
    digitalWrite(pumpa, LOW);
    digitalWrite(prekidac, HIGH);
    delay(500);
    vlaga = analogRead(senzorVlage);
    digitalWrite (prekidac, LOW);
    int mapirano = map(vlaga, 0, 300, 0, 100);
    ThingSpeak.setField(1, mapirano);
    ThingSpeak.setField(2, pumpaOff);
    odgovor = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (odgovor == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(odgovor));
    }
  }
  else {
    ThingSpeak.setField(1, mapirano);
    ThingSpeak.setField(2, pumpaOff);
    Serial.println("Pumpa OFF");
    digitalWrite(pumpa, LOW);
    delay(100);
    odgovor = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (odgovor == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(odgovor));
    }
  }



  for (int i = 60; i > 0; i--) {
    delay(1000);
    Serial.println(i);
  }



}
