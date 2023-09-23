#ifndef SPEXSAT_BOARD_MEGA
#define SPEXSAT_BOARD_MEGA 0
#endif
#ifndef SPEXSAT_BOARD_UNO
#define SPEXSAT_BOARD_UNO 1
#endif

#ifndef SPEXSAT_BOARD
#define SPEXSAT_BOARD SPEXSAT_BOARD_MEGA
#endif

#ifndef SPEXSAT_BAUD
#define SPEXSAT_BAUD 9600
#endif

#ifndef SPEXGPS_BAUD
#define SPEXGPS_BAUD 4800
#endif

//#include <SPI.h>
//#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#if SPEXSAT_BOARD == SPEXSAT_BOARD_MEGA
#define RXGPS 15
#define TXGPS 14
#endif

//SoftwareSerial serialGPS = SoftwareSerial(RXGPS, TXGPS);
String stringGPS = "";
TinyGPSPlus gps;
void setup() {
  //  pinMode(RXGPS, INPUT);
  //  pinMode(TXGPS, OUTPUT);
  
    Serial.begin(SPEXSAT_BAUD);
    Serial.println("Started");

    // GPS Setup
    #if SPEXSAT_BOARD == SPEXSAT_BOARD_MEGA
    Serial2.begin(SPEXGPS_BAUD);
    #endif
    //digitalWrite(TXGPS, HIGH);
}

void loop() {
  if(Serial2.available()){
    while (Serial2.available()) {
        Serial.write((Serial2.read()));
    }
    Serial.println();
  }
}
    /*
    while(Serial2.available()){ // check for gps data
    if(gps.encode(Serial2.read()))// encode gps data
    { 
    
    //Latitude
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(),6);
    
    //Longitude
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(),6); 

    //Altitude
    Serial.print("Altitude: "); 
    Serial.println(gps.altitude.feet());

    //Speed
    Serial.print("Speed: "); 
    Serial.println(gps.speed.mph());

    // Number of satellites connected
    Serial.print("Number of Sats connected: "); 
    Serial.println(gps.satellites.value());

    delay(2000);
    
   }
  }
}
//*/
