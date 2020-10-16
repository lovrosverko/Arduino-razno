#define ledica 5

void setup() {
  // put your setup code here, to run once:
pinMode (ledica, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(ledica, HIGH);
delay(500);
digitalWrite(ledica, LOW);
delay(500);

}
