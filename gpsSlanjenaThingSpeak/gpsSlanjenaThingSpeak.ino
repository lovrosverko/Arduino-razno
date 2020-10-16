#include <TinyGPS.h>
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>

TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

char ssid[] = "Pozitron";          //  Change this to your network SSID (name).
char pass[] = "forzafiume1987!";           // Change this to your network password.
char mqttUserName[] = "C4awL3r";  // Use any name.
char mqttPass[] = "WGZTUTT8T7O5CACK";      // Change to your MQTT API Key from Account > MyProfile.
char writeAPIKey[] = "64D5VUNXX8VFBYTY";    // Change to your channel write API key.
long channelID = 1009890;                    // Change to your channel ID.
int fieldNumber = 4;                        // Field to use is publishing to only one field.

static const char alphanum[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";  // For random generation of client ID.

WiFiClient client;                           // Initialize the Wi-Fi client library.

/*
  Create a partially initialized PubSubClient instance, and define the ThingSpeak MQTT broker.
*/
PubSubClient mqttClient(client); // Initialize the PubSubClient library.
const char* server = "mqtt.thingspeak.com";
/*
  Define other global variables to track the last connection time and to define the time interval to publish the data.
*/
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 15L * 1000L; // Post data every 20 seconds.

void setup()
{
  lastConnectionTime = millis();
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial1.begin(9600);
  delay(5000);

  int status = WL_IDLE_STATUS;  // Set temporary Wi-Fi status.

  // Attempt to connect to Wi-Fi network.
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 Wi-Fi network.
    delay(5000);
  }

  Serial.println("Connected to wifi");
  mqttClient.setServer(server, 1883);   // Set the MQTT broker details.
}

void loop() // run over and over
{
  bool newdata = false;

  // Every 5 seconds we print an update
  // Reconnect if MQTT client is not connected.
  if (!mqttClient.connected())
  {
    reconnect();
  }

  mqttClient.loop();   // Call the loop continuously to establish connection to the server.

  // If interval time has passed since the last connection, publish data to ThingSpeak.
  if (millis() - lastConnectionTime > postingInterval)
  {
    if (Serial1.available())


    {
      char c = Serial1.read();

      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c))
      {
        newdata = true;
      }
    }

    if (newdata)
    {
      Serial.println("Acquired Data");
      Serial.println("-------------");
      gpsdump(gps);
      Serial.println("-------------");
      Serial.println();


    }

    Serial.println(millis());
  }
}
void gpsdump(TinyGPS & gps)
{
  lastConnectionTime = millis();
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

  gps.get_position(&lat, &lon, &age);
  /*
    Serial.print("Lat/Long(10^-5 deg): "); Serial.print(lat); Serial.print(", "); Serial.print(lon);
    Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");
  */

  // On Arduino, GPS characters may be lost during lengthy Serial.print()
  // On Teensy, Serial prints to USB, which has large output buffering and
  //   runs very fast, so it's not necessary to worry about missing 4800
  //   baud GPS characters.

  gps.f_get_position(&flat, &flon, &age);
  /*
    Serial.print("Lat/Long(float): "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);
    Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");

    Serial.println("--------------------");
    Serial.println("--------------------");
    Serial.print("Debug: "); Serial.print(flat, 5); Serial.print(", "); Serial.print(flon, 5);
    Serial.println("--------------------");
    Serial.println("--------------------");
  */
  gps.get_datetime(&date, &time, &age);
  /*
    Serial.print("Date(ddmmyy): "); Serial.print(date); Serial.print(" Time(hhmmsscc): ");
    Serial.print(time);
    Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");
  */
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  /*
    Serial.print("Date: "); Serial.print(static_cast<int>(month)); Serial.print("/");
    Serial.print(static_cast<int>(day)); Serial.print("/"); Serial.print(year);
    Serial.print("  Time: "); Serial.print(static_cast<int>(hour + 8));  Serial.print(":"); //Serial.print("UTC +08:00 Malaysia");
    Serial.print(static_cast<int>(minute)); Serial.print(":"); Serial.print(static_cast<int>(second));
    Serial.print("."); Serial.print(static_cast<int>(hundredths)); Serial.print(" UTC +08:00 Malaysia");
    Serial.print("  Fix age: ");  Serial.print(age); Serial.println("ms.");

    Serial.print("Alt(cm): "); Serial.print(gps.altitude()); Serial.print(" Course(10^-2 deg): ");
    Serial.print(gps.course()); Serial.print(" Speed(10^-2 knots): "); Serial.println(gps.speed());
    Serial.print("Alt(float): "); printFloat(gps.f_altitude()); Serial.print(" Course(float): ");
    printFloat(gps.f_course()); Serial.println();
    Serial.print("Speed(knots): "); printFloat(gps.f_speed_knots()); Serial.print(" (mph): ");
    printFloat(gps.f_speed_mph());
    Serial.print(" (mps): "); printFloat(gps.f_speed_mps()); Serial.print(" (kmph): ");
    printFloat(gps.f_speed_kmph()); Serial.println();
  */
  gps.stats(&chars, &sentences, &failed);
  /*
    Serial.print("Stats: characters: "); Serial.print(chars); Serial.print(" sentences: ");
    Serial.print(sentences); Serial.print(" failed checksum: "); Serial.println(failed);
  */
  String data = String("field1=") + String(flat, 5) + "&field2=" + String(flon, 5);
  int length = data.length();
  const char *msgBuffer;
  msgBuffer = data.c_str();
  Serial.println(msgBuffer);

  // Create a topic string and publish data to ThingSpeak channel feed.
  String topicString = "channels/" + String( channelID ) + "/publish/" + String(writeAPIKey);
  length = topicString.length();
  const char *topicBuffer;
  topicBuffer = topicString.c_str();
  if (!mqttClient.publish( topicBuffer, msgBuffer )) {
    Serial.println("--------------------nije poslano------------------");
  }
  else {
    Serial.println("--------------------poslano------------------");
    Serial.print("GPS lat: "); Serial.print(flat, 5); Serial.print(" GPS long: "); Serial.println(flon, 5);
    Serial.println("---------------------------------------------");

  }



}


void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0)
  {
    Serial.print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print(".");

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}

void reconnect()
{
  char clientID[9];

  // Loop until reconnected.
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID
    for (int i = 0; i < 8; i++) {
      clientID[i] = alphanum[random(51)];
    }
    clientID[8] = '\0';

    // Connect to the MQTT broker.
    if (mqttClient.connect(clientID, mqttUserName, mqttPass))
    {
      Serial.println("connected");
    } else
    {
      Serial.print("failed, rc=");
      // Print reason the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
