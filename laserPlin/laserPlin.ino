//
// Unutarnji Node sa temperaturom, senzorom za laser i pokret i prekidačem na dodir.
//

// biblioteka za WiFi
#include <ESP8266WiFi.h>
// biblioteka za MQTT protokol
#include <PubSubClient.h>

// wifi postavke
#define wifi_ssid "PametnaKuca"
#define wifi_password "forzafiume1987"

// definiranje mqtt servera koji je pokrenut na Raspberry Pi (openhab)
#define mqtt_server "192.168.5.100"

// redni broj uređaja (da znamo sa kojeg uređaja dolaze poruke)
#define nodeId 82

// integrirana LED
#define ACT1 2

// senzor temperature
#define PLIN A0

#define LASERPIN 13

// brzina serijskog porta
#define SERIAL_BAUD 115200

//  početne postavke
long  TXinterval = 60;
long  TIMinterval = 30;           // timer interval in seconds
int plinState = LOW;
int valPlin;
int laser;

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
bool  imaPlina, nemaPlina;      // boolean varijable čije stanje određuje sadržaj poruke
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
    laser = strPayload.toInt(); // convert to int
    readAction = false;               // default is 'SET' value
    if (strPayload == "READ") readAction = true;  // in this case 'READ' value
    if (length == 0) {
      error = 2; // no payload sent
    }
    switch (laser) {
      case 1:
        {
          digitalWrite(LASERPIN, HIGH);
        }
        break;
      case 2:
        {
          digitalWrite (LASERPIN, LOW);
        }
        break;
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
    pubMQTT(buff_topic, buff_msg);
  }

  
  if (imaPlina) {
    sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev06", nodeId);
    sprintf(buff_msg, "ON");
    pubMQTT(buff_topic, buff_msg);
    imaPlina = false;
  }

  if (nemaPlina) {

    sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev06", nodeId);
    sprintf(buff_msg, "OFF");
    pubMQTT(buff_topic, buff_msg);
    nemaPlina = false;
  }

}

//  SETUP

//===============================================================================================
void setup() {                  // set up serial, output and wifi connection
  pinMode(ACT1, INPUT);
  pinMode(PLIN, INPUT);
  pinMode (LASERPIN, OUTPUT);
  digitalWrite (LASERPIN, LOW);

  ACT1State = 0;
  digitalWrite(ACT1, ACT1State);
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



  valPlin = analogRead(PLIN);
  Serial.print("PLIN sensor: ");
  Serial.println(valPlin);

  if (valPlin >= 72) {
    imaPlina = true;
  }
  else {
    nemaPlina = true;
  }

  sendMsg();


  delay(1000);



}   // end loop



