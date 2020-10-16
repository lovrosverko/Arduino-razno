#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define IR5 A4

void setup() {
  // put your setup code here, to run once:
  pinMode (IR1, INPUT);
  pinMode (IR2, INPUT);
  pinMode (IR3, INPUT);
  pinMode (IR4, INPUT);
  pinMode (IR5, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.print("IR1: ");
  Serial.println(analogRead(IR1));
  Serial.print("IR2: ");
  Serial.println(analogRead(IR2));
  Serial.print("IR3: ");
  Serial.println(analogRead(IR3));
  Serial.print("IR4: ");
  Serial.println(analogRead(IR4));
  Serial.print("IR5: ");
  Serial.println(analogRead(IR5));
  Serial.println("--------------------------------");

delay(500);

}
