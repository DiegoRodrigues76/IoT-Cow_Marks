// --------------------------------------------------
// Libraries

#include <RoboCore_SMW_SX1262M0.h>
#include <SoftwareSerial.h>
#define INTERVALO 5*60*106
long timeStamp;

// --------------------------------------------------
// Variables

SoftwareSerial ss(10,11);
SMW_SX1262M0 lorawan(ss);

// --------------------------------------------------
// --------------------------------------------------

void setup() {
  // start the UART for the computer
  Serial.begin(9600);
  Serial.println(F("--- SMW_SX1262M0 Bridge ---"));
  
  // start the UART for the LoRaWAN module
  ss.begin(9600);
  timeStamp = millis();
}

// --------------------------------------------------
// --------------------------------------------------

void loop() {
  // SMW_SX1262M0 to computer
  if(ss.available()){
    Serial.write(ss.read());
  }

  // computer to SMW_SX1262M0
  if(Serial.available()){
    ss.write(Serial.read());
  }
  if ((millis() - timeStamp) > 3* (long)INTERVALO){
    ss.println("AT+SEND=10:TESTE");
    timeStamp = millis();
  }  
  
}

// --------------------------------------------------
// --------------------------------------------------
