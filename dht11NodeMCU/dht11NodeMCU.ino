/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define svjetloSensor A0
// Replace with your network details
const char* ssid = "Caffe Piramida";
const char* password = "nemampojma";

// Web Server on port 80
WiFiServer server(80);

WiFiClient client;

// DHT Sensor
const int DHTPin = D7;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];
int svjetlo;

/*
unsigned long weatherStationChannelNumber = 12397;
unsigned int temperatureFieldNumber = 4;
*/
unsigned long myChannelNumber = 196696;
const char * myWriteAPIKey = "AVELG5KLKFHOT2FJ";


// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();

  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(1000);

  // Printing the ESP IP address
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);

}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  svjetlo = analogRead(svjetloSensor);

  Serial.println(t);
  Serial.println(h);
  Serial.println(f);
  Serial.println(svjetlo);
  Serial.println("-----");

  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, f, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 4, svjetlo, myWriteAPIKey);

  delay (15000);



}

