#include <ESP8266WiFi.h>
#include <DHT.h>



const char* ssid = "DevetaLegija"; //wifi ssid / wifi name
const char* password = "3DD44B54EECF"; //wifi password
const char* host = "192.168.5.20";
int svjetloPin = 10;
int tempPin = D7;
int svjetlo;
int temp;

DHT dht(tempPin, DHT11);

void setup() {
  pinMode (svjetloPin, INPUT);
  pinMode (tempPin, INPUT);
  dht.begin();
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
int value = 0;

void loop() {
int  svjetloX= analogRead(svjetloPin);
  svjetlo = map(svjetloX, 0, 1023, 0, 100);
  temp = dht.readTemperature();
  delay(50);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

    Serial.print("Svjetlo: "); Serial.println(svjetlo);
    Serial.print("Temperatura: "); Serial.println(temp);
    // We now create a URI for the request
    

  }


