#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11

// postavke WiFi mreže (ssid i lozinka)
const char* ssid = "FunkyMonkey";
const char* password = "7755747C77F0";
int status = WL_IDLE_STATUS;
WiFiClient client;

WiFiServer server(80);
// DHT Sensor
const int DHTPin = 14;
const int RainPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// korisnički podaci o kanalu i API ključu sa ThingSpeak.com
unsigned long myChannelNumber = 196696;
const char * myWriteAPIKey = "AVELG5KLKFHOT2FJ";

void setup() {
  Serial.begin(115200);
  pinMode (DHTPin, INPUT);

  // spajanje na mrežu
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
  dht.begin();
  // pokretanje ThingSpeak klijenta
  ThingSpeak.begin(client);
}

void loop() {
  // Check if a client has connected
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  int rain = digitalRead(RainPin);
Serial.println(t);Serial.println(h);Serial.println(f);
  // slanje podataka na ThingSpeak kanal
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, f);
  ThingSpeak.setField(3, h);
  ThingSpeak.setField(4, rain);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);



  // debug (provjera podataka)
  // Serial.print("CO razina= ");Serial.println(ocitanje);

  // koliko često će se slati podaci na ThingSpeak (15 sekundi je najkraći dopušteni interval)
  delay(15000);


}
