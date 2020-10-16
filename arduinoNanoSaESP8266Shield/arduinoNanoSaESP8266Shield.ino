#include<stdlib.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define net "FunkyMonkey"
#define PASS "7755747C77F0"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=AVELG5KLKFHOT2FJ&field1=";
SoftwareSerial monitor(0, 1); // RX, TX

void setup()
{
  monitor.begin(9600);
  Serial.begin(9600);
  sensors.begin();
  sendDebug("AT");
  delay(5000);

}

void loop() {
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd = "AT+CWJAP=\"";
  cmd += net;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  sendDebug(cmd);
  delay(5000);
  if (Serial.find("OK")) {
    Serial.println("RECEIVED: OK");
    monitor.println("RECEIVED: OK");
    return true;
  } else {
    Serial.println("RECEIVED: Error");
    monitor.println("RECEIVED: Error");
    return false;
  }
}

void updateTemp(String tenmpF) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if (Serial.find("Error")) {
    monitor.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if (Serial.find(">")) {
    monitor.print(">");
    monitor.print(cmd);
    Serial.print(cmd);
  } else {
    sendDebug("AT+CIPCLOSE");
  }
  if (Serial.find("OK")) {
    monitor.println("RECEIVED: OK");
  } else {
    monitor.println("RECEIVED: Error");
  }
}
void sendDebug(String cmd) {
  monitor.print("SEND: ");
  monitor.println(cmd);
  Serial.println(cmd);
}


