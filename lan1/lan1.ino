#include "SPI.h"
#include "Phpoc.h"
 
PhpocEmail email;
int senzor = A0
void setup() {
  Serial.begin(9600);
 
  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET | PF_LOG_APP);
  pinMode(senzor, INPUT);
}
 
int previous_status = 1;
 
 
void loop() {
  int current_status = digitalRead(A0);
  int returnValue;
  
  if (previous_status == 1 && current_status == 0) {    // if door is opened...
    email.setOutgoingServer("smtp.gmail.com", 587);
    email.setOutgoingLogin("lovro.sverko@sser.hr", "bigbadwolf4321");
 
    email.setFrom("lovro.sverko@sser.hr", "Lovro SSER");
    email.setTo("lovro.sverko@gmail.com", "Lovro Pvt");
    
    email.setSubject("Nema svjetla [#905]");  // Mail Subject
    
    // Mail Contents
    email.beginMessage();
    email.println("#905");
    email.println("");
    email.println("Nema svjetla.");
    email.endMessage();                
 
    if (email.send() > 0)  // Send Email
      Serial.println("Your Mail has been sent successfully");
    else
      Serial.println("Your Mail is not sent");
 
    
  }   else if (previous_status == 0 && current_status == 1) { // if door is closed...
 
        // Write codes in the same way
  
  }
 
  previous_status = current_status;
  delay(1500);
 
}
