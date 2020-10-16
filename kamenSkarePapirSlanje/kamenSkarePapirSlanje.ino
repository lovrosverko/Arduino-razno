
/*
  ArduinoMqttClient - WiFi Simple Sender

  This example connects to a MQTT broker and publishes a message to
  a topic once a second.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev.2 board

  This example code is in the public domain.
*/

#include <ArduinoMqttClient.h>
#include <WiFi101.h> // for MKR1000 change to: #include <WiFi101.h>

#define vibro A1
#define Led11 4
#define Led12 3
#define Led13 2
#define Led21 5
#define Led22 7
#define Led23 6
#define Led31 10
#define Led32 9
#define Led33 8
#define LedPobjeda 11
#define LedGubitak 12

char ssid[] = "MW40V_2384";      //  your WiFi network SSID (name)
char pass[] = "04598995";       // your WiFi network key

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "sszc";
const char igra[] = "rezultat";
const long interval = 1000;
unsigned long previousMillis = 0;
String poruka = "";
int count = 0;
int start;
int rezultat1;
int rezultat2;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(Led11, OUTPUT);
  pinMode(Led12, OUTPUT);
  pinMode(Led13, OUTPUT);
  pinMode(Led21, OUTPUT);
  pinMode(Led22, OUTPUT);
  pinMode(Led23, OUTPUT);
  pinMode(Led31, OUTPUT);
  pinMode(Led32, OUTPUT);
  pinMode(Led33, OUTPUT);
  pinMode(LedPobjeda, OUTPUT);
  pinMode(LedGubitak, OUTPUT);
  iskljuci();

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  // subscribe to a topic
  mqttClient.subscribe(igra);
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();
  start = analogRead(vibro);
  if (start > 100) {
    rezultat1 = akcija();

    switch (rezultat1) {
      case 0:
        Serial.println("Kamen");
        poruka = "1";
        Serial.println(poruka);
        mqttClient.beginMessage(topic);
        mqttClient.print(poruka);
        mqttClient.endMessage();
        kamen();
        break;
      case 1:
        Serial.println("Škare");
        poruka = "22";
        Serial.println(poruka);
        mqttClient.beginMessage(topic);
        mqttClient.print(poruka);
        mqttClient.endMessage();
        skare();
        break;
      case 2:
        Serial.println("Papir");
        poruka = "333";
        Serial.println(poruka);
        mqttClient.beginMessage(topic);
        mqttClient.print(poruka);
        mqttClient.endMessage();
        papir();
        break;
    }
    for (int i = 5; i > 0; i--) {
      Serial.print(".");
      delay(100);
    }
    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(poruka);
    Serial.println();

  }
  // primanje i čitanje mqtt poruke na topic 'igra'
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();

  }
}
void skare() {
  digitalWrite(Led11, HIGH);
  digitalWrite(Led13, HIGH);
  digitalWrite(Led22, HIGH);
  digitalWrite(Led31, HIGH);
  digitalWrite(Led33, HIGH);
  digitalWrite(Led12, LOW);
  digitalWrite(Led21, LOW);
  digitalWrite(Led23, LOW);
  digitalWrite(Led32, LOW);


}
void papir() {
  digitalWrite(Led11, HIGH);
  digitalWrite(Led13, HIGH);
  digitalWrite(Led22, LOW);
  digitalWrite(Led31, HIGH);
  digitalWrite(Led33, HIGH);
  digitalWrite(Led12, HIGH);
  digitalWrite(Led21, HIGH);
  digitalWrite(Led23, HIGH);
  digitalWrite(Led32, HIGH);

}
void kamen() {
  digitalWrite(Led11, LOW);
  digitalWrite(Led13, LOW);
  digitalWrite(Led22, LOW);
  digitalWrite(Led31, LOW);
  digitalWrite(Led33, LOW);
  digitalWrite(Led12, HIGH);
  digitalWrite(Led21, HIGH);
  digitalWrite(Led23, HIGH);
  digitalWrite(Led32, HIGH);

}
void pobjeda() {
  digitalWrite(LedPobjeda, HIGH);
  digitalWrite(LedGubitak, LOW);


}
void gubitak() {
  digitalWrite(LedPobjeda, LOW);
  digitalWrite(LedGubitak, HIGH);


}
void iskljuci() {
  digitalWrite(Led11, LOW);
  digitalWrite(Led13, LOW);
  digitalWrite(Led22, LOW);
  digitalWrite(Led31, LOW);
  digitalWrite(Led33, LOW);
  digitalWrite(Led12, LOW);
  digitalWrite(Led21, LOW);
  digitalWrite(Led23, LOW);
  digitalWrite(Led32, LOW);
  digitalWrite(LedPobjeda, LOW);
  digitalWrite(LedGubitak, LOW);
}



int akcija() {
  int akcija = random(3);
  return akcija;
}
