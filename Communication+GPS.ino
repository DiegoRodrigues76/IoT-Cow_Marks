// --------------------------------------------------
// Libraries

#include <RoboCore_SMW_SX1262M0.h>
#include <SoftwareSerial.h>
#define INTERVALO 5*60*106
long timeStamp;

// --------------------------------------------------
// Variables

SoftwareSerial ss(10,11);
SoftwareSerial SerialGPS(3, 4);
TinyGPS GPS;
SMW_SX1262M0 lorawan(ss);

float lat, lon, vel;
unsigned long data, hora;
unsigned short sat;

// --------------------------------------------------
// --------------------------------------------------

void setup() {
  // start the UART for the computer
  Serial.begin(9600);
  Serial.println(F("--- SMW_SX1262M0 Bridge ---"));
  
  // start the UART for the LoRaWAN module
  ss.begin(9600);
  timeStamp = millis();
  
  // GPS
  SerialGPS.begin(9600);
  Serial.begin(9600);
 
  Serial.println("Buscando satelites...");
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
  
    while (SerialGPS.available()) {
    if (GPS.encode(SerialGPS.read())) {
 
      //Hora e data
      GPS.get_datetime(&data, &hora);
      
      Serial.print("--");
      Serial.print((hora / 1000000) - 3);
      Serial.print(":");
      Serial.print((hora % 1000000) / 10000);
      Serial.print(":");
      Serial.print((hora % 10000) / 100);
      Serial.print(" -- ");
 
      Serial.print(data / 10000);
      Serial.print("/");
      Serial.print((data % 10000) / 100);
      Serial.print("/");
      Serial.print(data % 100);
      Serial.println("--");
      
      //latitude e longitude
      GPS.f_get_position(&lat, &lon);
 
      Serial.print("Latitude: ");
      Serial.println(lat, 6);
      Serial.print("Longitude: ");
      Serial.println(lon, 6);
 
      //velocidade
      vel = GPS.f_speed_kmph();
 
      Serial.print("Velocidade: ");
      Serial.println(vel);

      Serial.print("http://maps.google.com/maps?q=");
      Serial.print(lat, 6);
      Serial.print(",");
      Serial.print(lon, 6);

      Serial.println("");
     
 
      //Satelites
      sat = GPS.satellites();
      
 
      if (sat != TinyGPS::GPS_INVALID_SATELLITES) {
        Serial.print("Satelites: ");
        Serial.println(sat);
      }

    Serial.println("");
    
}

// --------------------------------------------------
// --------------------------------------------------
