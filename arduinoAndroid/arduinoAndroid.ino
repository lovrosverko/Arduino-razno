#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_GFX.h>





const char* ssid = "FunkyMonkey";
const char* password = "7755747C77F0";

ESP8266WebServer server(80);

const int output1 = 14;
const int output2 = 12;
const int output3 = 13;
const int output4 = 15;

boolean device1 = false;
boolean device2 = false;
boolean device3 = false;
boolean device4 = false;

void handleRoot() {
  //digitalWrite(led, 1);
  //server.send(200, "text/plain", "hello from esp8266!");
  //digitalWrite(led, 0);

  String cmd;     
      cmd += "<!DOCTYPE HTML>\r\n";
      cmd += "<html>\r\n";
      //cmd += "<header><title>ESP8266 Webserver</title><h1>\"ESP8266 Web Server Control\"</h1></header>";
      cmd += "<head>";
      cmd += "<meta http-equiv='refresh' content='5'/>";
      cmd += "</head>";
      
      if(device1){
        cmd +=("<br/>Device1  : ON");
      }
      else{
        cmd +=("<br/>Device1  : OFF");
      }
      
      if(device2){
        cmd +=("<br/>Device2  : ON");
      }
      else{
        cmd +=("<br/>Device2  : OFF");
      }
           
       if(device3){
        cmd +=("<br/>Device3  : ON");
      }
      else{
        cmd +=("<br/>Device3  : OFF");
      }
      
      if(device4){
        cmd +=("<br/>Device4  : ON");
      }
      else{
        cmd +=("<br/>Device4  : OFF");
      }
           
      cmd += "<html>\r\n";
      server.send(200, "text/html", cmd);
}

void handleNotFound(){
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}

void setup(void){
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);

  digitalWrite(output1, LOW); 
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output4, LOW);
   
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");


 

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

   
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/status1=1", [](){
    server.send(200, "text/plain", "device1 = ON");
    digitalWrite(output1, HIGH); 
    device1 = true;
  });

  server.on("/status1=0", [](){
    server.send(200, "text/plain", "device1 = OFF");
    digitalWrite(output1, LOW); 
    device1 = false;
  });

  server.on("/status2=1", [](){
    server.send(200, "text/plain", "device2 = ON");
    digitalWrite(output2, HIGH); 
    device2 = true;
  });

  server.on("/status2=0", [](){
    server.send(200, "text/plain", "device2 = OFF");
    digitalWrite(output2, LOW); 
    device2 = false;
  });

  server.on("/status3=1", [](){
    server.send(200, "text/plain", "device3 = ON");
    digitalWrite(output3, HIGH); 
    device3 = true;
  });

  server.on("/status3=0", [](){
    server.send(200, "text/plain", "device3 = OFF");
    digitalWrite(output3, LOW); 
    device3 = false;
  });

  server.on("/status4=1", [](){
    server.send(200, "text/plain", "device4 = ON");
    digitalWrite(output4, HIGH); 
    device4 = true;
  });

  server.on("/status4=0", [](){
    server.send(200, "text/plain", "device4 = OFF");
    digitalWrite(output4, LOW); 
    device4 = false;
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
