#define DIGI_PIN_SOMETHING 13
uint8_t commandIn;

byte inputByte_0;

byte inputByte_1;

byte inputByte_2;

byte inputByte_3;

byte inputByte_4;

void setup()
{
    //create a serial connection at 57500 baud
    Serial.begin(9600);
}

void loop()
{
    //if we have some incomming serial data then..
    if (Serial.available() > 0)
    {
        //read 1 byte from the data sent by the pc
        commandIn = Serial.read();
        //test if the pc sent an 'a' or 'b'
        switch (commandIn)
        {
            case 'a':
            {
                //we got an 'a' from the pc so turn on the digital pin
                digitalWrite(DIGI_PIN_SOMETHING,HIGH);
                break;
            }
            case 'b':
            {
                //we got an 'b' from the pc so turn off the digital pin
                digitalWrite(DIGI_PIN_SOMETHING,LOW);
                break;
            }
        }
    }
}
