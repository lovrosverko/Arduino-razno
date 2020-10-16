#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 6, 2, 9, 4, 5);
int val;
int temp = 0;


void setup() 
{
 lcd.begin(16,2);
Serial.begin(9600);
}

void loop() 
{
 val = analogRead(temp);
 float mv = ( val/1024.0)*500; 
 lcd.print("Temp. = ");
 lcd.print(mv);
 lcd.print("C");
 delay(10000);
 lcd.clear();
}
