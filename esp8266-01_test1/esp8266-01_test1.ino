#include <SoftwareSerial.h>

SoftwareSerial wifi(1,0); // RX, TX

void setup()
{
Serial.begin(115200); // FTDI
wifi.begin(115200); // GPS; 9600 first time, 38400 second time
}

void loop()
{

wifi.write('AT+CWJAP="FunkyMonkey","7755747C77F0"');
Serial.println("AT");

if (wifi.available())
{
  Serial.println("BLA");
Serial.println(wifi.read());
}
delay(1000);
}
