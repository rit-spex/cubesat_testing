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
    #define SPEXSAT_BOARD SPEXSAT_BOARD_MEGA
#endif

#if SPEXSAT_BOARD == SPEXSAT_BOARD_UNO
    #define DOUT 11 //This connects to the DOUT pin on the ADC
    #define DIN 12
    #define SCLK 13
    #define ADC_CS 10
#elif SPEXSAT_BOARD == SPEXSAT_BOARD_MEGA
    #define SDI 50 //This connects to the DOUT pin on the ADC
    #define SDO 51
    #define SCLK 52
    #define ADC_CS 53
#endif

#define ADC_BITC 12
#define ADC_CHANNEL_BYTE 0//0 = channel 1, 1 = channel 2
#define ADC_CHB_LOC 3

void setup_adc() {

    Serial.begin(SPEXSAT_BAUD);
}

int read_adc(){
    uint16_t adcVal=0;
    long curADC=0;
    byte cmdBits=B00000000 | ADC_CHANNEL_BYTE << ADC_CHB_LOC;

	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //TODO: double check SPI settings
    digitalWrite(ADC_CS, LOW);
	byte adcVal_A = 0;
	byte adcVal_B = 0;
	adcVal_A = SPI.transfer(cmdBits);
	adcVal_B = SPI.transfer(0x00);

	adcVal = (adcVal_A << 8) | adcVal_B; 	

    digitalWrite(ADC_CS, HIGH);
	SPI.endTransaction();
    #ifdef SPEXSAT_DEBUG
	Serial.print(adcVal_A);
	Serial.print(", ");
	Serial.print(adcVal_B);
    Serial.print(" | ");
    #endif
    return adcVal;
}
//This is for testing the function, comment it out when including somewhere else
#define SPEXSAT_TESTING_ADC
#ifdef SPEXSAT_TESTING_ADC
void setup(){
  setup_adc();
}
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
