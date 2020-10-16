#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Otherwise these settings will be used for WiFi connection
#define AP_SSID     "FunkyMonkey"
#define AP_PASSWORD "7755747C77F0"

#define mqtt_server "192.168.5.100"
#define nodeId 01

//  sensor setting
#define LED01 D4
#define echoPin D6 // Echo Pin
#define trigPin D7 // Trigger Pin

//  STARTUP DEFAULTS
long  TXinterval = 60;
long  TIMinterval = 30;           // timer interval in seconds

double lat;
double lng;
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
bool  led01;
bool  ultrasenzor, geoLokacija, send99;         // message triggers
char  buff_topic[30];             // mqtt topic
char  buff_msg[32];             // mqtt message

char buff_udaljenost[20];
char *daljina;
char buff_lng[20];
char buff_lat[20];
char *lngtude;
char *lattude;

int udaljenost, udaljenostStara, trajanje;
int maxUdaljenost = 300;
void mqttSubs(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient MQTTclient(mqtt_server, 1883, mqttSubs, espClient); // instantiate MQTT client

/* Serial Baud Rate */
#define SERIAL_BAUD       115200
/* Delay paramter for connection. */
#define WIFI_DELAY        500
/* Max SSID octets. */
#define MAX_SSID_LEN      32
/* Wait this much until device gets IP. */
#define MAX_CONNECT_TIME  30000

/* SSID that to be stored to connect. */
char ssid[MAX_SSID_LEN] = "FunkyMonkey";

char host[] = "www.googleapis.com";
String google_apikey = "AIzaSyCY04HHHzRbAwnDYN3zNJcPMfmGVEfvo8U";

void pubMQTT(String topic, String topic_val) { // publish MQTT message to broker
  Serial.print("topic " + topic + " value:");
  Serial.println(String(topic_val).c_str());
  MQTTclient.publish(topic.c_str(), String(topic_val).c_str(), true);
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
  if (strlen(topic) == 25) {            // correct topic length ?
    DID = (topic[25] - '0') * 10 + topic[26] - '0'; // extract device ID from MQTT topic
    payload[length] = '\0';           // terminate string with '0'
    String strPayload = String((char*)payload); // convert to string
    if (strPayload == "ON") {
      digitalWrite (LED01, LOW);
    }
    if (strPayload == "OFF") {
      digitalWrite (LED01, HIGH);
    }

    readAction = false;               // default is 'SET' value
    if (strPayload == "READ") readAction = true;  // in this case 'READ' value

    if (length == 0) {
      error = 2; // no payload sent
    }
  }
}

void reconnect() {                // reconnect to mqtt broker
  sprintf(buff_topic, "kuca/slanje/wemos%02d/#", nodeId);
  while (!MQTTclient.connected()) {
    Serial.print("Connect to MQTT broker...");
    if (MQTTclient.connect("ESP_GW")) {
      MQTTclient.subscribe(buff_topic);
      Serial.println("connected");
    } else {
      Serial.println("Failed, try again in 5 seconds");
      delay(5000);
    }
  }
}

void OpaliUltra() {
  /*
    The following trigPin/echoPin cycle is used to determine the
    distance of the nearest object by bouncing soundwaves off of it.
    The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    Give a short LOW pulse beforehand to ensure a clean HIGH pulse.
  */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  trajanje = pulseIn(echoPin, HIGH);
  // Calculate the distance (in cm) based on the speed of sound.
  udaljenost = trajanje / 58.2;
  Serial.println(udaljenost);
}

void sendMsg() {
  int i;
  if (wakeUp) {
    wakeUp = false;
    sprintf(buff_topic, "kuca/primanje/wemos%02d/wakeup", nodeId);
    sprintf(buff_msg, "NODE %d WAKEUP", nodeId);
    send99 = false;
    pubMQTT(buff_topic, buff_msg);
  }
  if (ultrasenzor) {
    ultrasenzor = false;
    OpaliUltra();
    while (udaljenost > maxUdaljenost) {
      OpaliUltra();
      Serial.println("Ultrasonic error!");
    }
    daljina = dtostrf(udaljenost, 5, 2, buff_udaljenost);
    delay(20);
    sprintf(buff_topic, "kuca/primanje/kanta%02d/razina", nodeId);
    sprintf(buff_msg, " ", daljina);
    send99 = false;
    pubMQTT(buff_topic, daljina);
    if (udaljenost <= 15) {
      sprintf(buff_topic, "kuca/primanje/kante/alarm");
      sprintf(buff_msg, "Kanta %02d je puna!", nodeId);
      send99 = false;
      pubMQTT(buff_topic, buff_msg);
    }
  }
  if (geoLokacija) {
    geoLokacija = false;

    lngtude = dtostrf(lng, 3, 6, buff_lng);
    lattude = dtostrf(lat, 3, 6, buff_lat);
    delay(20);
    sprintf(buff_topic, "kuca/primanje/kanta%02d/longitud", nodeId);
    sprintf(buff_msg, " ", lngtude);
    send99 = false;
    pubMQTT(buff_topic, lngtude);

    sprintf(buff_topic, "kuca/primanje/kanta%02d/latitude", nodeId);
    sprintf(buff_msg, " ", lattude);
    pubMQTT(buff_topic, lattude);

  }

}
/* Scan available networks and request Google to determine your location. */
void scanAndLocate() {
  int errorCode;

  memset(ssid, 0, MAX_SSID_LEN);
  int n = WiFi.scanNetworks();
  Serial.println("Scan done!");
  if (n == 0) {
    Serial.println("No networks found!");
  } else {

    DynamicJsonBuffer  jsonBuffer;
    byte mac[6];
    JsonObject& root = jsonBuffer.createObject();
    root["considerIp"] = "false";
    JsonArray& data = root.createNestedArray("wifiAccessPoints");
    for (int i = 0; i < n; i++) {
      JsonObject& wifiAP = jsonBuffer.createObject();
      wifiAP["macAddress"] = WiFi.BSSIDstr(i);
      wifiAP["signalStrength"] = WiFi.RSSI(i);
      wifiAP["signalToNoiseRatio"] = 0;
      data.add(wifiAP);
    }
    root.prettyPrintTo(Serial);
    Serial.println();

    String postdata;
    root.printTo(postdata);

    String resultdata = httpsPost("/geolocation/v1/geolocate?key=" + google_apikey, "application/json", postdata, errorCode);

    Serial.println(errorCode);

    JsonObject& res_root = jsonBuffer.parseObject(resultdata);

    if (!root.success()) {
      Serial.println("JSON parsing failed!");
      return;
    }

    lat = res_root["location"]["lat"];
    lng = res_root["location"]["lng"];
    double acc = res_root["accuracy"];

    geoLokacija = true;

    Serial.print(lat, 5);
    Serial.print(",");
    Serial.println(lng, 5);
  }
}

#define POST_DEBUG 1

String httpsPost(String url, String contentType, String data, int &errorCode) {
  WiFiClientSecure client;

  if (client.connect(host, 443)) {
    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: " + (String)host);
    client.println("User-Agent: ESP8266/1.0");
    client.println("Connection: close");
    client.print("Content-Type: ");
    client.println(contentType);
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);

    String response = "";
    while (client.connected()) {
      response = response + client.readString();
    }
    response = response + client.readString();
#ifdef POST_DEBUG
    Serial.println("==========");
    Serial.println(response);
    Serial.println("==========");
#endif
    errorCode = response.substring(response.indexOf(' ') + 1).toInt();
    if (errorCode == 0) {
      errorCode = 444;
      return "No Response";
    }

    int bodypos =  response.indexOf("\r\n\r\n") + 4;
    return response.substring(bodypos);
  }
  else {
    errorCode = 600;
    return "Network Unreachable";
  }
}

/* Scan available networks and sort them in order to their signal strength. */
void scanAndSort() {
  memset(ssid, 0, MAX_SSID_LEN);
  int n = WiFi.scanNetworks();
  Serial.println("Scan done!");
  if (n == 0) {
    Serial.println("No networks found!");
  } else {
    Serial.print(n);
    Serial.println(" networks found.");
    int indices[n];
    for (int i = 0; i < n; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }

    for (int i = 0; i < n; ++i) {
      Serial.print(WiFi.SSID(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.RSSI(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.encryptionType(indices[i]));
      Serial.println();
      if (WiFi.encryptionType(indices[i]) == ENC_TYPE_NONE) {
        Serial.println("Found non-encrypted network. Store it and exit to connect.");
        memset(ssid, 0, MAX_SSID_LEN);
        strncpy(ssid, WiFi.SSID(indices[i]).c_str(), MAX_SSID_LEN);
        break;
      }
    }
  }
}

void setup() {
  pinMode(LED01, OUTPUT);
  pinMode (trigPin, OUTPUT);
  digitalWrite (trigPin, LOW);
  pinMode (echoPin, INPUT);
  Serial.begin(SERIAL_BAUD);
  Serial.println("Started.");
  delay(10);
  Serial.println();             // connect to WIFI
  Serial.print("Connecting to ");
  Serial.println(AP_SSID);
  WiFi.begin(AP_SSID, AP_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  scanAndSort();
  delay(WIFI_DELAY);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    scanAndLocate();
  }
  else {
    Serial.println("Cannot established connection on given time.");
  }


  if (!MQTTclient.connected()) {
    reconnect();
  }
  MQTTclient.loop();
  if (!ultrasenzor) {
    ultrasenzor = true;
  }
  sendMsg();
  delay (2000);
}

