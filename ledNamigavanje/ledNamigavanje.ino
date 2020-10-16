int led1 = 6;
int led2 = 7;
void setup() {
  // put your setup code here, to run once:
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  digitalWrite (led1, LOW);
  digitalWrite (led2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (led1, HIGH);
  delay(500);
  digitalWrite (led2, HIGH);
  delay(500);
  digitalWrite (led1, LOW);
  delay(500);
  digitalWrite(led2, LOW);
  delay(500);

  for (int i = 0; i < 5; i++)
  {
    digitalWrite (led1, HIGH);
    digitalWrite (led2, LOW);
    delay(500);
    digitalWrite (led1, LOW);
    digitalWrite (led2, HIGH);
    delay(500);
  }
  digitalWrite (led1, LOW);
  digitalWrite (led2, LOW);
  delay(500);
  digitalWrite (led2, HIGH);
  delay(500);
  digitalWrite (led1, HIGH);
  delay(500);
  digitalWrite (led2, LOW);
  delay(500);
  digitalWrite(led1, LOW);
  delay(500);
  for (int i = 0; i < 5; i++)
  {
    digitalWrite (led1, HIGH);
    digitalWrite (led2, LOW);
    delay(500);
    digitalWrite (led1, LOW);
    digitalWrite (led2, HIGH);
    delay(500);
  }
  digitalWrite (led1, LOW);
  digitalWrite (led2, LOW);
  delay(500);
}
