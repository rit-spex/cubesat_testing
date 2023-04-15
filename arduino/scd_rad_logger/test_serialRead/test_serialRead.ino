#ifndef SPEXSAT_BAUD
    #define SPEXSAT_BAUD 9600
#endif

#include <SPI.h>

#define SPEXSAT_BOARD_MEGA

/*
PIN SETUP FOR ELECTRICAL PEOPLE
1: CS   8: VCC
2: SIO1 7: SIO3
3: SIO2 6: SCLK
4: GND  5: SIO0

*/

#ifdef SPEXSAT_BOARD_UNO
    #define MEM_CS 9
    #define MEM_SCLK 8
    #define MEM_SIO0 7
    #define MEM_SIO1 6
    #define MEM_SIO2 5
    #define MEM_SIO3 4
#endif

#ifdef SPEXSAT_BOARD_MEGA
    #define MEM_SCLK 32
    #define MEM_CS 28
    #define MEM_SIO0 33
    #define MEM_SIO1 29
    #define MEM_SIO2 30
    #define MEM_SIO3 31
#endif

#define MEM_EN4B 0xB7
#define MEM_SUS 0xB0 //suspends current program/erase
#define MEM_RES 0x30 //resumes suspended program/erase
#define MEM_DP 0xB9 //deep power down
#define MEM_RDP 0xAB //release deep power down
#define MEM_WREN 0x06 //write enable
#define MEM_WRDI 0x04 //write disable
#define MEM_SE 0x20 //sector erase
#define MEM_READ 0xEB //4READ on datasheet, next 4 bytes are memory address, 6th is dummy
#define MEM_WRITE 0x38 //4PP on datasheet, next 4 bytes are memory addredd, followed by 1-256 data cycles
#define MEM_PERFEN 0x00 //performance enhance mode is for repeated reads, not something we need here
#define MEM_DUMMYC 8


void setup() {

    pinMode(MEM_SIO0, OUTPUT);
    pinMode(MEM_SIO1, OUTPUT);
    pinMode(MEM_SIO2, OUTPUT);
    pinMode(MEM_SIO3, OUTPUT);
    pinMode(MEM_SCLK, OUTPUT);
    pinMode(MEM_CS, OUTPUT);

   //bytesRead = new byte[256];

   /* digitalWrite(MEM_CS, HIGH);
    digitalWrite(MEM_IN, LOW);
    digitalWrite(MEM_SCLK, LOW);
    
    digitalWrite(MEM_CS, LOW);
    //enabling 4 byte addressing
    //writeByte(MEM_EN4B);    
    digitalWrite(MEM_CS, HIGH);
*/
    Serial.begin(9600);
}

String readstring;

void loop() {

    if(Serial.available() > 0){
        readstring = Serial.readString();
        //readstring.trim(); //removes \r or \n from end of string // not bothering with this for now, can't trim char[] as string
        Serial.print("Read string: ");
        Serial.println(readstring);
        Serial.print("Char 0");
        Serial.println(readstring[0]);
    }
}



