// --------------------------------------------------
// Libraries

#include <RoboCore_SMW_SX1262M0.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#define INTERVALO 5*60*106
#define GPS_RX 4
#define GPS_TX 3
#define GPS_Serial_Baud 9600

long timeStamp;
TinyGPS gps;

// --------------------------------------------------
// Variables

SoftwareSerial ss(10,11);
SMW_SX1262M0 lorawan(ss);
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// --------------------------------------------------
// --------------------------------------------------

void setup() {
  // start the UART for the computer
  Serial.begin(9600);
  Serial.println(F("--- SMW_SX1262M0 Bridge ---"));
  
  // start the UART for the LoRaWAN module
  ss.begin(9600);
  timeStamp = millis();
  
  //GPS
  Serial.begin(GPS_Serial_Baud);
  gpsSerial.begin(GPS_Serial_Baud);
}

// --------------------------------------------------
// --------------------------------------------------

void loop() 
{
  // GPS
  bool newData = false;
  unsigned long chars;
  
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gpsSerial.available())
    {
      char c = gpsSerial.read();
      // Serial.write(c); //apague o comentario para mostrar os dados crus
      if (gps.encode(c)) // Atribui true para newData caso novos dados sejam recebidos
        newData = true;
    }
 
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
  
  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.println();
    Serial.println();
  }
}

// --------------------------------------------------
// --------------------------------------------------
