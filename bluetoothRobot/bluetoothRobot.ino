#include <AFMotor.h>
#include <SoftwareSerial.h>


#define HC06 Serial2

#define desni_brzina 10
#define desni_natrag 9
#define desni_naprijed 8
#define lijevi_natrag 7
#define lijevi_naprijed 6
#define lijevi_brzina 5

int maxBrzina = 255;
int brzina = 150;

char motorSet;
int speakerPin = 11;

//////////////////////////////////////////////////////////////////////////////////////////////

// KOD ZA KRETANJE
void desniNaprijed(int speedO) {
  analogWrite (desni_brzina, speedO);
  digitalWrite (desni_natrag, LOW);
  digitalWrite (desni_naprijed, HIGH);
}

void desniNatrag(int speedO) {
  analogWrite (desni_brzina, speedO);
  digitalWrite (desni_natrag, HIGH);
  digitalWrite (desni_naprijed, LOW);
}

void lijeviNaprijed(int speedO) {
  analogWrite (lijevi_brzina, speedO);
  digitalWrite (lijevi_natrag, LOW);
  digitalWrite (lijevi_naprijed, HIGH);
}

void lijeviNatrag(int speedO) {
  analogWrite (lijevi_brzina, speedO);
  digitalWrite (lijevi_natrag, LOW);
  digitalWrite (lijevi_naprijed, HIGH);
}


void naprijed() {
  desniNaprijed(brzina);
  lijeviNaprijed(brzina);
}

void lijevo() {
  analogWrite (desni_brzina, brzina);
  analogWrite (lijevi_brzina, brzina);
  digitalWrite (desni_natrag, HIGH);
  digitalWrite (desni_naprijed, LOW);
  digitalWrite (lijevi_natrag, LOW);
  digitalWrite (lijevi_naprijed, HIGH);
}
void naprijedLijevo() {
  analogWrite (desni_brzina, brzina / 2);
  analogWrite (lijevi_brzina, brzina);
  digitalWrite (desni_natrag, LOW);
  digitalWrite (desni_naprijed, HIGH);
  digitalWrite (lijevi_natrag, LOW);
  digitalWrite (lijevi_naprijed, HIGH);
}

void desno() {
  analogWrite (desni_brzina, brzina);
  analogWrite (lijevi_brzina, brzina);
  digitalWrite (desni_natrag, LOW);
  digitalWrite (desni_naprijed, HIGH);
  digitalWrite (lijevi_natrag, HIGH);
  digitalWrite (lijevi_naprijed, LOW);
}

void naprijedDesno() {

}

void natragDesno() {

}

void stani() {
  analogWrite (desni_brzina, 0);
  analogWrite (lijevi_brzina, 0);
  digitalWrite (desni_natrag, LOW);
  digitalWrite (desni_naprijed, LOW);
  digitalWrite (lijevi_natrag, LOW);
  digitalWrite (lijevi_naprijed, LOW);
}

void natrag() {
  analogWrite (desni_brzina, brzina);
  analogWrite (lijevi_brzina, brzina);
  digitalWrite (desni_natrag, HIGH);
  digitalWrite (desni_naprijed, LOW);
  digitalWrite (lijevi_natrag, LOW);
  digitalWrite (lijevi_naprijed, HIGH);
}
//////////////////////////////////////////// SETUP ////////////////////////////
void setup() {

  Serial.begin(9600);
  HC06.begin(9600);

  pinMode (desni_brzina, OUTPUT);
  pinMode (lijevi_brzina, OUTPUT);
  pinMode (desni_natrag, OUTPUT);
  pinMode (desni_naprijed, OUTPUT);
  pinMode (lijevi_natrag, OUTPUT);
  pinMode (lijevi_naprijed, OUTPUT);



}

//////////////////////////////////////////// LOOP ////////////////////////////
void loop() {

  if (HC06.available())
  { char plaviZub = HC06.read(); Serial.println(plaviZub);

    switch (plaviZub) {
      case '0':
        brzina = 0;
        break;
      case '1':
        brzina = 80;
        break;
      case '2':
        brzina = 100;
        break;
      case '3':
        brzina = 120;
        break;
      case '4':
        brzina = 140;
        break;
      case '5':
        brzina = 160;
        break;
      case '6':
        brzina = 180;
        break;
      case '7':
        brzina = 200;
        break;
      case '8':
        brzina = 220;
        break;
      case '9':
        brzina = 240;
        break;
      case 'q':
        brzina = 255;
        break;
      case 'V': //krug1
        
        break;
      case 'R':  //desno
        desno();
        break;
      case 'L':  //lijevo
        lijevo();
        break;
      case 'F':  //naprijed
        naprijed();
        break;
      case 'B':  //natrag
        natrag();
        break;
      case 'X':  //krug2
        
        break;
      case 'S':  //stop
        stani();
        break;
      case 'x':  //stop
        stani();
        break;
      case 'v':  //stop
        stani();
        break;
      case 'D':  //stop
        stani();
        break;
      case 'G':  //stop
        naprijedLijevo();
        break;
      case 'I':  //stop
        naprijedDesno();
        break;
      case 'H':  //stop
       // natragLijevo();
        break;
      case 'J':  //stop
       // natragDesno();
        break;
    }
  }

} // END LOOP
