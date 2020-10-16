#include <AFMotor.h>
#include <Servo.h> 
#include <NewPing.h>

#define TRIG_PIN 19 // definira pin A5 na Motor Drive Shield-u (zalemljen kabel od ultrazvučnog senzora)
#define ECHO_PIN 17 // definira pin A3 na Motor Drive Shield-u (zalemljen kabel od ultrazvučnog senzora)
#define MAX_DISTANCE 200 // najveća korisna udaljenost - 200cm (više neće registrirati)
#define MAX_SPEED 180 // postavka brzine DC motora na 180/256 (cca 70%)zbog manje potrošnje baterije
#define MAX_SPEED_OFFSET 5 // dozvoljena razlika brzine u DC motorima (20+20)
#define COLL_DIST 10 // udaljenost na kojoj ide u rikverc - 10cm
#define TURN_DIST COLL_DIST+10 //udaljenost na kojoj skreće - 20 cm

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); //definiranje pinova koji se koriste i najveće korisne udaljenosti
AF_DCMotor motor1(4, MOTOR12_1KHZ); // definiranje motora na M1 izlazu Motor Drive Shield-a, 1kHz PWM frekvencije
AF_DCMotor motor2(2, MOTOR12_1KHZ); // definiranje motora na M1 izlazu Motor Drive Shield-a, 1kHz PWM frekvencije
Servo servo;  // definiranje servo motora za okretanje ultrazvučnog senzora


int udaljenostLijevo, udaljenostDesno;
int udaljenost = 0;
String postavkaMotora = "";
int brzinaMotora = 0;

//-------------------------------------------- SETUP ----------------------------------------------------------------------------
void setup() {
  servo.attach(9); // SERVO_2 na Motor Drive Shield-u
 
  servo.write(90); // postavljanje serva na sredinu ("oči" naprijed)
 
  delay(1000); // pauza 1 sekunda
 }
//------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------- LOOP ------------------------------------------------------------------------------
void loop() {
  servo.write(90);  
  delay(90);
  udaljenost = pingaj();   
  if (udaljenost < COLL_DIST) {
    provjeraUdaljenosti();}
  idiNaprijed();  
  delay(500);
 }

 void provjeraUdaljenosti() {
  poZor();
  servo.write(25);  // provjera desno
    delay(500);
    udaljenostDesno = pingaj();
    delay(500);
    servo.write(155);  // provjera lijevo
    delay(1000);
    udaljenostLijevo = pingaj(); 
    delay(500);
    servo.write(90);
    delay(100);
    usporediUdaljenosti();
  }

  
void usporediUdaljenosti()   // bira gdje će skrenuti
{
  if (udaljenostLijevo>udaljenostDesno) // hoću li lijevo?
  {

    skreniLijevo();
  }
  else if (udaljenostDesno>udaljenostLijevo) // hoću li desno kod kamene stijene?
  {

    skreniDesno();
    
  }
   else // if (udaljenostDesno=udaljenostLijevo)
  {
    chelomNazad();
  }
}

//----------------------------------------------------- FUNKCIJE ----------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------

int pingaj() { // vraća udaljenost u cm
  delay(70);   
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void poZor() {          // isključi DC motore
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
 }  
//-------------------------------------------------------------------------------------------------------------------------------------
void idiNaprijed() {
    

      
    postavkaMotora = "NAPRIJED";
    motor1.run(FORWARD);      // ide naprijed
    motor2.run(FORWARD);      
  for (brzinaMotora = 0; brzinaMotora < MAX_SPEED; brzinaMotora +=2) // ubrzava postepeno
  {
    motor1.setSpeed(brzinaMotora);
    motor2.setSpeed(brzinaMotora+MAX_SPEED_OFFSET);
    delay(5);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void rikverc() {
    postavkaMotora = "RIKVERC";
    motor1.run(BACKWARD);      // ide u rikverc
    motor2.run(BACKWARD);     
  for (brzinaMotora = 0; brzinaMotora < MAX_SPEED; brzinaMotora +=2)
  {
    motor1.setSpeed(brzinaMotora);
    motor2.setSpeed(brzinaMotora+MAX_SPEED_OFFSET);
    delay(5);
  }
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void skreniDesno() {
  postavkaMotora = "DESNO";
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  delay(200); // definira koliko dugo skreće
  postavkaMotora = "NAPRIJED";
  motor1.run(FORWARD);      // kreće naprijed nakon skretanja
  motor2.run(FORWARD);      
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void skreniLijevo() {
  postavkaMotora = "LIJEVO";
  motor1.run(BACKWARD);     
  motor2.run(FORWARD);      
  delay(200);
  postavkaMotora = "NAPRIJED";
  motor1.run(FORWARD);      
  motor2.run(FORWARD);      
  }  
//-------------------------------------------------------------------------------------------------------------------------------------
void chelomNazad() {
  postavkaMotora = "DESNO";
  motor1.run(FORWARD);      
  motor2.run(BACKWARD);     
  delay(400); 
  postavkaMotora = "NAPRIJED";
  motor1.run(FORWARD);
  motor2.run(FORWARD);      
}  
