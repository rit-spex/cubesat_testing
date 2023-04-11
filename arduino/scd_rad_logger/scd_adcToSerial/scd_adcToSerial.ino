#ifndef SPEXSAT_BAUD
    #define SPEXSAT_BAUD 9600
#endif

#include <SPI.h>

#define SPEXSAT_BOARD_MEGA

#ifdef SPEXSAT_BOARD_UNO
    #define ADC_DOUT 11 //This connects to the DOUT pin on the ADC
    #define ADC_DIN 12
    #define ADC_SCLK 13
    #define ADC_CS 10
#endif

#ifdef SPEXSAT_BOARD_MEGA
    #define ADC_DOUT 23 //This connects to the DOUT pin on the ADC
    #define ADC_DIN 38
    #define ADC_SCLK 39
    #define ADC_CS 22
#endif

#define ADC_BITC 12
#define ADC_CHANNEL_BYTE 0//0 = channel 1, 1 = channel 2
#define ADC_CHB_LOC 3

void setup() {

    pinMode(ADC_DIN, OUTPUT);
    pinMode(ADC_DOUT, INPUT);
    pinMode(ADC_SCLK, OUTPUT);
    pinMode(ADC_CS, OUTPUT);

    digitalWrite(ADC_CS, HIGH);
    digitalWrite(ADC_DIN, LOW);
    digitalWrite(ADC_SCLK, LOW);

    Serial.begin(SPEXSAT_BAUD);
}

int read_adc(){
    long adcVal=0;
    byte cmdBits=B00000000 | ADC_CHANNEL_BYTE << ADC_CHB_LOC;

    digitalWrite(ADC_CS, LOW);

    //writes command bits
    for(int i = 7; i>=4; i--){
        digitalWrite(ADC_DIN, cmdBits & 1<<i);
        digitalWrite(ADC_SCLK, HIGH);
        digitalWrite(ADC_SCLK, LOW);
    }

    int j = ADC_BITC-1;
    for(int i = 3; i>= 0; i--, j--){
        digitalWrite(ADC_DIN, cmdBits & 1<<i);
        adcVal |= digitalRead(ADC_DOUT)<<j;
        digitalWrite(ADC_SCLK, HIGH);
        digitalWrite(ADC_SCLK, LOW);
    }


    for(; j>=0; j--){
        adcVal |= digitalRead(ADC_DOUT)<<j;
        digitalWrite(ADC_SCLK, HIGH);
        digitalWrite(ADC_SCLK, LOW);
    }
    digitalWrite(ADC_CS, HIGH);

    return adcVal;
}

void loop() {

    Serial.println(String("Val: "+read_adc()) + " Time: " + millis());

}
