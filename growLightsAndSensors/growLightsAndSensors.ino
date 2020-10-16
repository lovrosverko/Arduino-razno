
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

/***************************** Uređaji **************************************/
#define glavni 2
#define s01 16
#define s02 4
#define s03 14
#define s04 12
#define s05 13
#define senzorVlage A0


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);

Adafruit_MQTT_Subscribe onsvjetlo = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/svjetloOn");
Adafruit_MQTT_Subscribe offsvjetlo = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/svjetloOff");
Adafruit_MQTT_Subscribe onled01 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led01on");
Adafruit_MQTT_Subscribe offled01 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led01off");
Adafruit_MQTT_Subscribe onled02 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led02on");
Adafruit_MQTT_Subscribe offled02 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led02off");
Adafruit_MQTT_Subscribe onled03 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led03on");
Adafruit_MQTT_Subscribe offled03 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led03off");
Adafruit_MQTT_Subscribe onled04 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led04on");
Adafruit_MQTT_Subscribe offled04 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led04off");
Adafruit_MQTT_Subscribe onled05 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led05on");
Adafruit_MQTT_Subscribe offled05 = Adafruit_MQTT_Subscribe(&mqtt, "home/grow/node01/led05off");

Adafruit_MQTT_Publish topicVlaga = Adafruit_MQTT_Publish(&mqtt, "smartGrow/node01/vlaga");


void MQTT_connect();

void setup() {
  Serial.begin(115200);
  pinMode(glavni, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(s01, OUTPUT);
  pinMode(s02, OUTPUT);
  pinMode(s03, OUTPUT);
  pinMode(s04, OUTPUT);
  pinMode(s05, OUTPUT);
  pinMode(senzorVlage, INPUT);
  digitalWrite(glavni, HIGH);
  digitalWrite(s01, LOW);
  digitalWrite(s02, HIGH);
  digitalWrite(s03, LOW);
  digitalWrite(s04, LOW);
  digitalWrite(s05, LOW);


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

}

// the loop function runs over and over again forever
void loop() {

  MQTT_connect();
  saljiVlagu();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onsvjetlo)
    {
      Serial.println("Svjetlo ON");
      digitalWrite (glavni, LOW);
      digitalWrite(s01, HIGH);
      digitalWrite(s02, LOW);
      digitalWrite(s03, HIGH);
      digitalWrite(s04, HIGH);
      digitalWrite(s05, HIGH);

      Serial.print(F("Got: "));
      Serial.println((char *)onsvjetlo.lastread);
    }

    if (subscription == &offsvjetlo)
    {
      Serial.println("Svjetlo OFF");

      digitalWrite(glavni, HIGH);
      digitalWrite(s01, LOW);
      digitalWrite(s02, HIGH);
      digitalWrite(s03, LOW);
      digitalWrite(s04, LOW);
      digitalWrite(s05, LOW);

      Serial.print(F("Got: "));
      Serial.println((char *)offsvjetlo.lastread);
    }
  }
}
void saljiVlagu() {
  int vlaga = analogRead(senzorVlage);
  Serial.print("Vlaga: "); Serial.println(vlaga);


  // debug
  Serial.print(F("\nVlaga "));

  // slanje i debug
  if (!topicVlaga.publish(vlaga)) {
    Serial.println(F("\nVlaga nije poslano!"));
  }
  else {
    Serial.println(F("\nVlaga poslano!"));
  }
}
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
