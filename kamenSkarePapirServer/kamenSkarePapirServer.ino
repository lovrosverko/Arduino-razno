#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>
/* Tony Canning 2018/5/6
   Derivative from Dr. Charif Mahmoudi's work here:
   https://www.hackster.io/charifmahmoudi/arduino-mkr1000-getting-started-08bb4a
   Where he notes:
   This example is modified from the original file
   https://github.com/arduino-libraries/WiFi101/blob/master/examples/SimpleWebServerWiFi/SimpleWebServerWiFi.ino
*/
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "MW40V_2384";      //  your WiFi network SSID (name)
char pass[] = "04598995";   // your network password
int keyIndex = 0;               // your network key Index number (needed only for WEP)
int ledPin = 7;
int izgubioPin = 6;//The built in LED pin on the MKR1000
int vibepin = 5;          //A pin to which an LED will be connected for
int brightness = 0;       // how bright the LED is
int fadeAmount = 17;      // how many points to fade the LED by
bool val = true;

int rezultat;
int rezultat2;
bool pobjeda = false;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  for (int i = 10; i > 0; i--) {
    Serial.print(".");
    delay(200);
  }

  Serial.print("Start Serial ");
  pinMode(ledPin, OUTPUT);
  pinMode (izgubioPin, OUTPUT); // set the LED pin mode
  pinMode(vibepin, OUTPUT);      // set the LED pin mode

  // Check for the presence of the shield
  Serial.print("WiFi101 shield: ");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("NOT PRESENT");
    return; // don't continue
  }
  Serial.println("DETECTED");
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    digitalWrite(ledPin, LOW);
    digitalWrite (izgubioPin, LOW);
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);
    digitalWrite(ledPin, HIGH);
    digitalWrite(izgubioPin, HIGH);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    for (int i = 10; i > 0; i--) {
      Serial.print(".");
      delay(500);
      digitalWrite (izgubioPin, LOW);
      delay(500);
      digitalWrite (izgubioPin, HIGH);

    }
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
  digitalWrite(ledPin, LOW);
  digitalWrite (izgubioPin, LOW);

}
void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor

        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<a href=\"/kamen\">Kamen</a>, <a href=\"/skare\">skare</a> i <a href=\"/papir\">papir</a>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see what the client request was:

        if (currentLine.endsWith("GET /kamen")) {
          rezultat2 = 0;
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
          if (usporedba(rezultat, rezultat2)) {
            digitalWrite(ledPin, HIGH);
          }
          else digitalWrite(izgubioPin, HIGH);

        }
        if (currentLine.endsWith("GET /skare")) {
          rezultat2 = 1;
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
          if (usporedba(rezultat, rezultat2)) {
            digitalWrite(ledPin, HIGH);
          }
          else digitalWrite(izgubioPin, HIGH);
        }
        if (currentLine.endsWith("GET /papir")) {
          rezultat2 = 2;
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
          if (usporedba(rezultat, rezultat2)) {
            digitalWrite(ledPin, HIGH);
          }
          else digitalWrite(izgubioPin, HIGH);
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


int akcija() {
  int akcija = random(3);
  return akcija;
}

bool usporedba(int rezultat, int rezultat2) {
  if (rezultat < rezultat2) {
    return false;
    Serial.print("Uspoređujem");
    Serial.print("Rezultat 1: "); Serial.println(rezultat);
    Serial.print("Rezultat 2: "); Serial.println(rezultat2);
  }
  else return true;
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
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
