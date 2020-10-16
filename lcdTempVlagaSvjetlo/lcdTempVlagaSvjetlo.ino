#include <DHT.h>
#include <LiquidCrystal.h>
#define svjetloPin A5
#define DHTPIN A1
#define DHTTYPE DHT11
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);

int motorPin = 3;
int lcd_key     = 0;
int adc_key_in  = 0;
int brojac;
int stanje;
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;
}
void menuNaprijed()
{
  brojac = brojac + 1;
  stanje = brojac % 3;
  switch (stanje) {
    case 0:
      {
        lcd.clear();
        int temp = dht.readTemperature();
        lcd.setCursor(0, 0);
        lcd.print("TEMPERATURA     ");
        lcd.setCursor(0, 1);
        lcd.print(temp);
        lcd.setCursor(3, 1);
        lcd.print("'C ");
        break;
      }
    case 1:
      {
        lcd.clear();
        int vlaga = dht.readHumidity();
        lcd.setCursor(0, 0);
        lcd.print("VLAGA ZRAKA    ");
        lcd.setCursor(0, 1);
        lcd.print(vlaga);
        lcd.setCursor(3, 1);
        lcd.print("% ");
        break;
      }
    case 2:
      {
        lcd.clear();
        int svjetlo = analogRead(svjetloPin);
        svjetlo = map(svjetlo, 0, 1023, 100, 0);
        lcd.setCursor(0, 0);
        lcd.print("RASVJETA   ");
        lcd.setCursor(0, 1);
        lcd.print(svjetlo);
        lcd.setCursor(3, 1);
        lcd.print("% ");
        break;
      }
    case 3:
      {
        // ovdje ide senzor vlage u zemlji
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("KLIKNI DALJE    ");
        lcd.setCursor(0, 1);
        lcd.print("  <<<<    >>>>  ");
        break;
      }

  }
}

void menuNatrag()
{
  if (brojac == 0) {
    brojac = 3;
  }
  brojac = brojac - 1;
  stanje = brojac % 3;

  switch (stanje) {
    case 0:
      {
        lcd.clear();
        int temp = dht.readTemperature();
        lcd.setCursor(0, 0);
        lcd.print("TEMPERATURA     ");
        lcd.setCursor(0, 1);
        lcd.print(temp);
        lcd.setCursor(3, 1);
        lcd.print("'C ");
        break;
      }
    case 1:
      {
        lcd.clear();
        int vlaga = dht.readHumidity();
        lcd.setCursor(0, 0);
        lcd.print("VLAGA ZRAKA    ");
        lcd.setCursor(0, 1);
        lcd.print(vlaga);
        lcd.setCursor(3, 1);
        lcd.print("% ");
        break;
      }
    case 2:
      {
        lcd.clear();
        int svjetlo = analogRead(svjetloPin);
        svjetlo = map(svjetlo, 0, 1023, 100, 0);
        lcd.setCursor(0, 0);
        lcd.print("RASVJETA   ");
        lcd.setCursor(0, 1);
        lcd.print(svjetlo);
        lcd.setCursor(3, 1);
        lcd.print("% ");
        break;
      }
    case 3:
      {
        // ovdje ide senzor vlage u zemlji
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("KLIKNI DALJE    ");
        lcd.setCursor(0, 1);
        lcd.print("  <<<<    >>>>  ");
        break;
      }
  }

}


void setup()
{
  brojac = 0;
  pinMode (motorPin, OUTPUT);
  digitalWrite (motorPin, LOW);
  dht.begin();
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  int temp = dht.readTemperature();
  int svjetlo = analogRead(svjetloPin);
  int vlaga = dht.readHumidity();

  lcd.setCursor(0, 1);           
  lcd_key = read_LCD_buttons();  
  switch (lcd_key)              
  {
    case btnRIGHT:
      {
        menuNaprijed();
        break;
      }
    case btnLEFT:
      {
        menuNatrag();
        break;
      }
    case btnUP:
      {
        lcd.setCursor(0, 0);
        lcd.print("ODABRALI STE:   ");
        lcd.setCursor(0, 1);
        lcd.print(" Automatski rad ");
        break;
      }
    case btnDOWN:
      {
        lcd.setCursor(0, 0);
        lcd.print("ODABRALI STE:   ");
        lcd.setCursor(0, 1);
        lcd.print(" Rucne postavke ");
        break;
      }
    case btnSELECT:
      {
       
        lcd.setCursor(0, 0);
        lcd.print("PALI PILU!      ");
        digitalWrite (motorPin, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("na 5 sekundi    ");
        delay (5000);
        lcd.setCursor(0, 0);
        lcd.print("GASI PILU!      ");
        lcd.setCursor(0, 1);
        lcd.print("              ");
        digitalWrite (motorPin, LOW);
        break;
      }
    case btnNONE:
      {
        break;
      }
  }

/////////////////////////////////////////////////////////////////////////////////////////////////// DEBUG //
  Serial.print ("Brojac = "); Serial.print(brojac); Serial.print("     ");
  Serial.print ("Stanje = "); Serial.println(stanje);

}
