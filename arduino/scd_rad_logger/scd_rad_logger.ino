#ifndef SPEXSAT_BAUD
#define SPEXSAT_BAUS 9600
#endif
#include <SPI.h>

#define DOUT 11 //This connects to the DIN pin on the ADC
#define DIN 12
#define SCLK 13
#define CS 10

#define ADC_CHANNEL_BYTE //TBD

byte clr;

void setup() {
Serial.begin(9600);

pinMode(DIN, INPUT);
pinMode(DOUT, OUTPUT);
pinMode(SCLK, OUTPUT);
pinMode(CS, OUTPUT);

digitalWrite(CS, HIGH);

}

void loop() {
/*
SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));
digitalWrite(CS, LOW);
int dataHigh = SPI.transfer(ADC_CHANNEL_BYTE);
int dataLow = SPI.transfer(0);
digitalWrite(CS, HIGH);
SPI.endTransaction();
*/


Serial.println((dataHigh << 8) + dataLow);

}



