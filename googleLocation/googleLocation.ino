#include <ESP8266WiFi.h>
#include <WifiLocation.h>

const char* googleApiKey = "AIzaSyDmrXvlLhDqWOl_U-UOi0OggyYVTAouvUg";
const char* ssid = "It hurts when IP";
const char* passwd = "Alibaba0011";

WifiLocation location(googleApiKey);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    Serial.println("");
    Serial.print("Spojen na: ");
    Serial.println(ssid);
    Serial.print("Status = ");
    Serial.println(WiFi.status());
    delay(500);
  }
  location_t loc = location.getGeoFromWiFi();

  Serial.println("Location request data");
  Serial.println(location.getSurroundingWiFiJson());
  Serial.println("Latitude: " + String(loc.lat, 7));
  Serial.println("Longitude: " + String(loc.lon, 7));
  Serial.println("Accuracy: " + String(loc.accuracy));


}

void loop() {

}
