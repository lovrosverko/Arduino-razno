#include <SPI.h>
#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>
#define botun 6


int rezultat;

char ssid[] = "MW40V_2384";      //  your WiFi network SSID (name)
char pass[] = "04598995";       // your WiFi network key

int status = WL_IDLE_STATUS;


// Initialize the Wifi client library.  I couldn't get WiFiSSLClient to work with ISY!
WiFiClient client;


void setup() {
  Serial.begin(9600); //Initialize Serial and wait for port to open:
  for (int i = 10; i > 0; i--) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  connectToAP();    // connect the board to router
  printWifiStatus();  // Check WiFi connection
}

void loop() {
  if (digitalRead(botun)) {
    rezultat = akcija();
    
    switch (rezultat) {
      case 0:
        Serial.println("Kamen");
        Serial.println(rezultat);
        break;
      case 1:
        Serial.println("Škare");
        Serial.println(rezultat);
        break;
      case 2:
        Serial.println("Papir");
        Serial.println(rezultat);
        break;
    }
    for (int i = 5; i > 0; i--) {
      Serial.print(".");
      delay(100);
    }
    Serial.println("");
  }
}

int akcija() {
  int akcija = random(3);
  return akcija;
}

// WiFi connection
void connectToAP() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    Serial.print(".");
    // wait 1 second for connection:
    delay(500);
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
