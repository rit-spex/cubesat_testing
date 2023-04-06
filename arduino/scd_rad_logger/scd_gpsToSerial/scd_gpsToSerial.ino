#ifndef SPEXSAT_BAUD
#define SPEXSAT_BAUD 9600
#endif

#ifndef SPEXGPS_BAUD
#define SPEXGPS_BAUD 4800
#endif

#include <SPI.h>
#include <SoftwareSerial.h>

#define RXGPS 0
#define TXGPS 1

SoftwareSerial serialGPS = SoftwareSerial(RXGPS, TXGPS);
String stringGPS = "";

void setup() {
    pinMode(RXGPS, INPUT);
    pinMode(TXGPS, OUTPUT);
  
    Serial.begin(SPEXSAT_BAUD);
    Serial.println("Started");

    // GPS Setup
    serialGPS.begin(SPEXGPS_BAUD);
    digitalWrite(TXGPS, HIGH);

    // Cut first gibberish (idk why we need this but it was in the template)
    while (serialGPS.available()) 
        if (serialGPS.read() == '\r')
            break;
}

void loop() {
    if (serialGPS.available()) {
        Serial.println(serialGPS.read());
    }
}
