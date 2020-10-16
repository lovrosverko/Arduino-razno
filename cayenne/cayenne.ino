#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneWiFi.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "cpgseed4q3";
// Your network name and password.
char ssid[] = "FunkyMonkey";
char password[] = "7755747C77F0";

void setup()
{
  Serial.begin(9600);
  Cayenne.begin(token, ssid, password);
}

void loop()
{
  Cayenne.run();
}
