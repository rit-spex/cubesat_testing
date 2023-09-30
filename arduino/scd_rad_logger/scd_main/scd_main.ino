//#define SPEXSAT_DEV_MODE

#ifndef SPEXSAT_BOARD_MEGA
    #define SPEXSAT_BOARD_MEGA 0
#endif
#ifndef SPEXSAT_BOARD_UNO
    #define SPEXSAT_BOARD_UNO 1
#endif

#ifndef SPEXSAT_BAUD
    #define SPEXSAT_BAUD 9600
#endif

#ifndef SPEXGPS_BAUD
    #define SPEXGPS_BAUD 9600
#endif

#include <SPI.h>
#include <SD.h>
#include <TinyGPSPlus.h>
#ifndef SPEXSAT_BOARD
    #define SPEXSAT_BOARD SPEXSAT_BOARD_MEGA
#endif

#if SPEXSAT_BOARD == SPEXSAT_BOARD_UNO
    #define ADC_DOUT 11 //This connects to the DOUT pin on the ADC
    #define ADC_DIN 12
    #define ADC_SCLK 13
    #define ADC_CS 10
#elif SPEXSAT_BOARD == SPEXSAT_BOARD_MEGA
    #define SD_CS_PIN 53    
    #define ADC_CS -1
#endif

#define ADC_BITC 12
#define ADC_CHANNEL_BYTE 0//0 = channel 1, 1 = channel 2
#define ADC_CHB_LOC 3

TinyGPSPlus gps;
unsigned long t_plus = 0;
RawDegrees gps_lat;
RawDegrees gps_lon;
uint32_t gps_t = 0;
double gps_alt = 0;
double gps_spd = 0;
uint32_t gps_sats = 0;
uint16_t adc_val = 0;
File dataCSV;

void setup(){
    // SD Setup
    while(!SD.begin(SD_CS_PIN));
    // GPS Setup
    Serial2.begin(SPEXGPS_BAUD);    
}

void read_adc(){
  adc_val = 0;

  byte cmdBits=B00000000 | ADC_CHANNEL_BYTE << ADC_CHB_LOC;

	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //TODO: double check SPI settings
  digitalWrite(ADC_CS, LOW);
	byte adcVal_A = 0;
	byte adcVal_B = 0;
	adcVal_A = SPI.transfer(cmdBits);
	adcVal_B = SPI.transfer(0x00);

	adc_val = (adcVal_A << 8) | adcVal_B; 	

  digitalWrite(ADC_CS, HIGH);
	SPI.endTransaction();
  #ifdef SPEXSAT_DEBUG
	  Serial.print(adcVal_A);
	  Serial.print(", ");
	  Serial.print(adcVal_B);
    Serial.print(" | ");
  #endif
}

void read_gps(){
    if(Serial2.available()){ //Check if GPS data exists
        while(Serial2.available()){
            if(gps.encode(Serial2.read())){
                gps_lat = gps.location.rawLat();
                gps_lon = gps.location.rawLng();
                gps_t = gps.time.value();
                gps_alt = gps.altitude.meters();
                gps_spd = gps.speed.mps();
                gps_sats = gps.satellites.value();
            }
        }
    }
}

void save_all_vals(){
    dataCSV = SD.open("data.csv", FILE_WRITE);
    dataCSV.print(t_plus);
    dataCSV.print(',');
    dataCSV.print(adc_val);
    dataCSV.print(',');
    if(gps_lon.negative){
        dataCSV.print('-');
    }
    else{
        dataCSV.print('+');
    }
    dataCSV.print(gps_lon.deg);
    dataCSV.print('|');
    dataCSV.print(gps_lon.billionths);
    dataCSV.print(',');
    if(gps_lat.negative){
        dataCSV.print('-');
    }
    else{
        dataCSV.print('+');
    }
    dataCSV.print(gps_lat.deg);
    dataCSV.print('|');
    dataCSV.print(gps_lat.billionths);
    dataCSV.print(',');
    dataCSV.print(gps_t);
    dataCSV.print(',');
    dataCSV.print(gps_alt);
    dataCSV.print(',');
    dataCSV.print(gps_spd);
    dataCSV.print(',');
    dataCSV.print(gps_sats);
    //dataCSV.print(',&');
    //dataCSV.print(sum_all_vals());
    dataCSV.println();
    dataCSV.close();
}

uint32_t sum_all_vals(){
  uint32_t sum = gps_sats + gps_t + t_plus + adc_val;
  double dsum = gps_alt + gps_spd;
  sum += (uint32_t)dsum;
  if(gps_lat.negative){
    sum -= (gps_lat.deg + gps_lat.billionths);
  }
  else{
    sum += (gps_lat.deg + gps_lat.billionths);
  }
  if(gps_lon.negative){
    sum -= (gps_lon.deg + gps_lon.billionths);
  }
  else{
    sum += (gps_lon.deg + gps_lon.billionths);
  }
  return sum;
}

void loop() {
    t_plus = millis();
    read_adc();
    read_gps();
    save_all_vals();
    while(millis()-100 < t_plus);
}
