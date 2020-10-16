#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define wifi_ssid "DevetaLegija"
#define wifi_password "3DD44B54EECF"
#define mqtt_server "192.168.5.100"
#define nodeId 01

//  sensor setting

#define DHTTYPE DHT11
#define RELAY1 D2
#define DHTPin D4
#define Moist1 A0
#define Moist1Power D6

#define SERIAL_BAUD 115200

//  STARTUP DEFAULTS
long  TXinterval = 60;
long  TIMinterval = 30;           // timer interval in seconds


//  VARIABLES

int   DID;                  // Device ID
int   error;                  // Syntax error code
long  lastPeriod = -1;            // timestamp last transmission
long  lastMinute = -1;            // timestamp last minute
long  upTime = 0;               // uptime in minutes
int   ACT1State;                // status ACT1 output
bool  wakeUp = true;              // wakeup indicator
bool  setAck = false;             // acknowledge receipt of actions
bool  msgBlock = false;           // flag to hold button message
bool  readAction;               // indicates read / set a value
bool  relej, sendVlaga;
bool  send16, send40, send99;         // message triggers
char  buff_topic[30];             // mqtt topic
char  buff_msg[32];             // mqtt message

void mqttSubs(char* topic, byte* payload, unsigned int length);
bool sendTemp, sendMoist;
SimpleTimer timer;
DHT dht(DHTPin, DHTTYPE);
WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, mqttSubs, espClient); // instantiate MQTT client
WiFiServer server(80);
//  FUNCTIONS

//===============================================================================================

void pubMQTT(String topic, String topic_val) { // publish MQTT message to broker
  Serial.print("topic " + topic + " value:");
  Serial.println(String(topic_val).c_str());
  client.publish(topic.c_str(), String(topic_val).c_str(), true);
}

void mqttSubs(char* topic, byte* payload, unsigned int length) {  // receive and handle MQTT messages
  int i;
  error = 4;                    // assume invalid device until proven otherwise
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (strlen(topic) == 27) {            // correct topic length ?
    DID = (topic[25] - '0') * 10 + topic[26] - '0'; // extract device ID from MQTT topic
    payload[length] = '\0';           // terminate string with '0'
    String strPayload = String((char*)payload); // convert to string
    readAction = false;               // default is 'SET' value
    if (strPayload == "READ") readAction = true;  // in this case 'READ' value
    if (strPayload == "On")
    {
      digitalWrite (RELAY1, HIGH);
      relej = true;
    }
    if (strPayload == "Off")
    {
      digitalWrite (RELAY1, LOW);
      relej = false;
    }
    if (length == 0) {
      error = 2; // no payload sent
    }
  }
}

void reconnect() {                // reconnect to mqtt broker
  // sprintf(buff_topic, "home/esp_gw/sb/node%02d/#", nodeId);
  sprintf(buff_topic, "kuca/slanje/wemos%02d/#", nodeId);
  while (!client.connected()) {
    Serial.print("Connect to MQTT broker...");
    if (client.connect("ESP_GW")) {
      client.subscribe(buff_topic);
      Serial.println("connected");
    } else {
      Serial.println("Failed, try again in 5 seconds");
      delay(5000);
    }
  }
}

void sendMsg() {
  int i;

  if (wakeUp) {
    wakeUp = false;
    // sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev99", nodeId);
    sprintf(buff_topic, "kuca/primanje/wemos%02d/wakeup", nodeId);
    sprintf(buff_msg, "NODE %d WAKEUP", nodeId);
    send99 = false;
    pubMQTT(buff_topic, buff_msg);
  }


  if (sendTemp) {
    int temperatura = dht.readTemperature();
    delay(100);
    if (temperatura > 100) {
      return;
    }
    else {
      sprintf(buff_topic, "home/grow/node%02d/temp", nodeId);
      char tempIspis[2];
      dtostrf(temperatura, 2, 0, tempIspis);
      sendTemp = false;
      pubMQTT(buff_topic, tempIspis);
    }
  }

  if (sendVlaga) {
    int vlaga = dht.readHumidity();
    delay(100);
    if (vlaga > 100) {
      return;
    }
    else {
      sprintf(buff_topic, "home/grow/node%02d/vlaga", nodeId);
      char tempIspis[2];
      dtostrf(vlaga, 2, 0, tempIspis);
      sendVlaga = false;
      pubMQTT(buff_topic, tempIspis);
    }
  }

  if (sendMoist) {
    digitalWrite (Moist1Power, HIGH);
    delay (200);
    int moist1 = analogRead(Moist1);
    delay(200);
    digitalWrite (Moist1Power, LOW);
    sprintf(buff_topic, "home/grow/node%02d/moist1", nodeId);
    char moistIspis[2];
    dtostrf(moist1, 2, 0, moistIspis);
    sendMoist = false;
    pubMQTT(buff_topic, moistIspis);
    Serial.print("Vlaga zemlje: ");
    Serial.println(moistIspis);

  }

}

void repeatMe() {

  sendTemp = true;
  sendVlaga = true;
  sendMoist = true;

}
//  SETUP

//===============================================================================================

void setup() {
  sendTemp = true;
  sendVlaga = true;
  sendMoist = true;
  pinMode(Moist1, INPUT);
  pinMode (Moist1Power, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  digitalWrite (Moist1Power, LOW);
  digitalWrite(RELAY1, LOW);
  pinMode(DHTPin, INPUT);
  dht.begin();
  timer.setInterval(600000, repeatMe);
  Serial.begin(SERIAL_BAUD);
  delay(10);
  Serial.println();             // connect to WIFI
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  sendMsg();
}

//  LOOP

//===============================================================================================
void loop() {
  sendMsg();
  timer.run();
  // Main program loop
  
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  WiFiClient client = server.available();
  delay(10);


  if (!client) {
    return;
  }

  // Čekanje da klijent pošalje neki podatak
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Čitanje prve linije zahtjeva
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // provjera zahtjeva

  if (req.indexOf("/relay1on") != -1) {

    digitalWrite(RELAY1, HIGH);
  }
  else if (req.indexOf("/relay1off") != -1) {

    digitalWrite(RELAY1, LOW);
  }

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.stop();
  // Priprema odgovora na zahtjev

  Serial.println("Zahtjev obrađen.");


  // PERIODIC TRANSMISSION

  /*
    if (TXinterval > 0) {
      int currPeriod = millis() / (TXinterval * 1000);
      if (currPeriod != lastPeriod) {             // interval elapsed ?
        lastPeriod = currPeriod;


        // list of sensordata to be sent periodically..
        // remove comment to include parameter in transmission

      }
    }
  */


}   // end loop


