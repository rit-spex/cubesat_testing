#ifndef SPEXSAT_BOARD_MEGA
    #define SPEXSAT_BOARD_MEGA 0
#endif
#ifndef SPEXSAT_BOARD_UNO
    #define SPEXSAT_BOARD_UNO 1
#endif

#ifndef SPEXSAT_BAUD
    #define SPEXSAT_BAUD 9600
#endif

#include <SPI.h>

#ifndef SPEXSAT_BOARD
    #define SPEXSAT_BOARD_MEGA
#endif

#if SPEXSAT_BOARD == SPEXSAT_BOARD_UNO
    #define ADC_DOUT 11 //This connects to the DOUT pin on the ADC
    #define ADC_DIN 12
    #define ADC_SCLK 13
    #define ADC_CS 10
#elif SPEXSAT_BOARD == SPEXSAT_BOARD_MEGA
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
    long curADC=0;
    byte cmdBits=B00000000 | ADC_CHANNEL_BYTE << ADC_CHB_LOC;

    digitalWrite(ADC_CS, LOW);

    //writes command bits
    for(int i = 7; i>=4; i--){
        digitalWrite(ADC_DIN, cmdBits & 1<<i);
        digitalWrite(ADC_SCLK, HIGH);
        digitalWrite(ADC_SCLK, LOW);
        //delay(1);
    }

    int j = ADC_BITC-1;
    for(int i = 3; i>= 0; i--, j--){
        digitalWrite(ADC_DIN, cmdBits & 1<<i);
        curADC = digitalRead(ADC_DOUT)<<j;
        Serial.print(curADC >> j);
        adcVal |= curADC;
        digitalWrite(ADC_SCLK, HIGH);
        digitalWrite(ADC_SCLK, LOW);
        
    }


    for(; j>=0; j--){
        curADC = digitalRead(ADC_DOUT)<<j;
        Serial.print(curADC >> j);
        adcVal |= curADC;
        digitalWrite(ADC_SCLK, HIGH);
        digitalWrite(ADC_SCLK, LOW);
        delay(1);
    }
    digitalWrite(ADC_CS, HIGH);
    Serial.print(" | ");
    return adcVal;
}
//This is for testing the function, comment it out when including somewhere else
#define SPEXSAT_TESTING_ADC
#ifdef SPEXSAT_TESTING_ADC
float sum = 0;
void loop() {
  Serial.println(read_adc());
    /*sum = 0;
    Serial.print(" Start Time: ");
    Serial.print(millis());
    for(int i = 0; i < 1000; i++){
      sum += read_adc();
      delay(1);
    }
    Serial.print(" End Time: ");
    Serial.print(millis());
    Serial.print(" Val: ");
    Serial.println(sum/1000);*/
}
#endif
