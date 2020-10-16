/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <DHT.h>
#include <SimpleTimer.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "DevetaLegija"
#define WLAN_PASS       "3DD44B54EECF"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.5.100"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
//#define AIO_USERNAME    "...your AIO username (see https://accounts.adafruit.com)..."
//#define AIO_KEY         "...your AIO key..."

/***************************** Uređaji **************************************/

#define DHTTYPE DHT11
#define RELAY1 D2
#define RELAY2 D3
#define DHTPin D4
#define Moist1 A0
#define Moist1Power D6

/******************************** bool funkcije *****************************/

bool relay1State = false;
bool relay2State = false;

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);

/*************************** DHT definiranje ********************************/

DHT dht(DHTPin, DHTTYPE);

/****************************** Feeds ***************************************/

// Setup a feed called 'temperatura' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperatura = Adafruit_MQTT_Publish(&mqtt, "home/grow/node01/temp");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onsvjetlo = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/svjetloOn");
Adafruit_MQTT_Subscribe offsvjetlo = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/svjetloOff");
Adafruit_MQTT_Subscribe onled = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/ledOn");
Adafruit_MQTT_Subscribe offled = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/ledOff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {

  Serial.begin(115200);
  pinMode(Moist1, INPUT);
  pinMode (Moist1Power, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode (RELAY2, OUTPUT);
  digitalWrite (RELAY2, LOW);
  digitalWrite (Moist1Power, LOW);
  digitalWrite(RELAY1, LOW);
  pinMode(DHTPin, INPUT);
  dht.begin();
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onsvjetlo);
  mqtt.subscribe(&offsvjetlo);
  mqtt.subscribe(&onled);
  mqtt.subscribe(&offled);
}

uint32_t x = 0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onsvjetlo)
    {
      relay1State = true;
      Serial.println("Svjetlo ON");
      digitalWrite (RELAY1, HIGH);
      Serial.print(F("Got: "));
      Serial.println((char *)onsvjetlo.lastread);
    }

if (subscription == &offsvjetlo)
    {
      relay1State = false;
      Serial.println("Svjetlo OFF");
      digitalWrite (RELAY1, LOW);
      Serial.print(F("Got: "));
      Serial.println((char *)offsvjetlo.lastread);
    }

if (subscription == &onled)
    {
      relay2State = true;
      Serial.println("Led ON");
      digitalWrite (RELAY2, HIGH);
      Serial.print(F("Got: "));
      Serial.println((char *)onled.lastread);
    }

if (subscription == &offled)
    {
      relay1State = false;
      Serial.println("Led OFF");
      digitalWrite (RELAY2, LOW);
      Serial.print(F("Got: "));
      Serial.println((char *)offled.lastread);
    }


  }


  int temp = dht.readTemperature();
  delay(100);
  if (temp < 100)
  {
    Serial.print(F("\nSending temperatura val "));
    Serial.print(temp);
    Serial.print("...");

    if (! temperatura.publish(temp)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  }


int vlaga = dht.readHumidity();
  delay(100);
  if (vlaga < 100)
  {
    Serial.print(F("\nSending vlaga zraka val "));
    Serial.print(vlaga);
    Serial.print("...");

    if (! temperatura.publish(vlaga)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  }
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
    if(! mqtt.ping()) {
    mqtt.disconnect();
    }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
