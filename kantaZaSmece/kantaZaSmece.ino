#include <NewPing.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TRIG_PIN 8
#define ECHO_PIN 2
#define MAX_DUBINA 200
#define MIN_DUBINA 15
#define ALARM MIN_DUBINA+10

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DUBINA); 
// biblioteka za WiFi
#include <ESP8266WiFi.h>
// biblioteka za MQTT protokol
#include <PubSubClient.h>
// biblioteka za senzor temperature i vlage
#include <DHT.h>

// wifi postavke
#define wifi_ssid "FunkyMonkey"
#define wifi_password "7755747C77F0"

// definiranje mqtt servera koji je pokrenut na Raspberry Pi (openhab)
#define mqtt_server "192.168.5.100"

// redni broj uređaja (da znamo sa kojeg uređaja dolaze poruke)
#define nodeId 80

//  vrsta senzora temperature i vlage zraka (postoje DHT11 i DHT22)
#define DHTTYPE DHT11

// integrirana LED
#define ACT1 2

// senzor temperature
#define TEMP 14
// senzor za kišu
#define RAIN 5
#define LIGHT A0
// brzina serijskog porta
#define SERIAL_BAUD 115200

//  početne postavke
DHT dht(TEMP, DHTTYPE);
long  TXinterval = 60;
long  TIMinterval = 30;           // timer interval in seconds
int rainState = LOW;
int valRain = 0;

//  varijable

int   DID;                  // Device ID
int   error;                  // Syntax error code
long  lastPeriod = 0;            // timestamp last transmission
long  lastMinute = -1;            // timestamp last minute
long  upTime = 0;               // uptime in minutes
int   ACT1State;                // status ACT1 output
bool  wakeUp = true;              // wakeup indicator
bool  setAck = false;             // acknowledge receipt of actions
bool  msgBlock = false;           // flag to hold button message
bool  readAction;               // indicates read / set a value
bool  sendLight, sendTemp, padaKisa, nePadaKisa; // varijable koje određuju što će se slati
bool  send16, send40, send99;         // message triggers
char  buff_topic[30];             // mqtt topic
char  buff_msg[32];             // mqtt message


// poetne postavke za MQTT
void mqttSubs(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, mqttSubs, espClient); // instantiate MQTT client

//  FUNKCIJE

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
    if (length == 0) {
      error = 2; // no payload sent
    }
  }
}

void reconnect() {                // reconnect to mqtt broker
  sprintf(buff_topic, "home/esp_gw/sb/node%02d/#", nodeId);
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
  // prva poruka "WakeUp"
  if (wakeUp) {
    wakeUp = false;
    sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev99", nodeId);
    sprintf(buff_msg, "NODE %d WAKEUP", nodeId);
    send99 = false;
    pubMQTT(buff_topic, buff_msg);
  }
  // slanje temperature
  if (sendTemp) {
    int temperatura = dht.readTemperature();
    delay(100);
    // senzor ponekad podivlja pa ako se to dogodi, ne šalje podatke
    if (temperatura > 100) {
      return;
    }
    else {
      sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev03", nodeId);
      char tempIspis[2];
      dtostrf(temperatura, 2, 0, tempIspis);
      sendTemp = false;
      pubMQTT(buff_topic, tempIspis);
    }
  }

  if (sendLight) {
    int aSvjetlost = analogRead(LIGHT);
    int svjetlost = map (aSvjetlost, 0, 1023, 99, 0);
    delay(100);
    sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev09", nodeId);
    char lightIspis[2];
    dtostrf(svjetlost, 2, 0, lightIspis);
    sendLight = false;
    pubMQTT(buff_topic, lightIspis);
    Serial.println(lightIspis);
  }

if (padaKisa) {
  sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev06", nodeId);
  sprintf(buff_msg, "ON");
  pubMQTT(buff_topic, buff_msg);
  padaKisa = false;
}

if (nePadaKisa) {

  sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev06", nodeId);
  sprintf(buff_msg, "OFF");
  pubMQTT(buff_topic, buff_msg);
  nePadaKisa = false;
}
}
//  SETUP

//===============================================================================================
void setup() {
  // configure output
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//  LOOP

//===============================================================================================
void loop() {                 // Main program loop
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  
}   // end loop



int gledaj() {
  delay(70);
  unsigned int udaljenost = sonar.ping();
  int cm = (udaljenost / 2) / 29.1;
  return cm;
}




