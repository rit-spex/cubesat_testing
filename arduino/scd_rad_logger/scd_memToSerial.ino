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
    #define MEM_SCLK 26
    #define MEM_CS 27
    #define MEM_SIO0 28
    #define MEM_SIO1 29
    #define MEM_SIO2 30
    #define MEM_SIO3 31
#endif

#define MEM_EQIO 0x35
#define MEM_EN4B 0xB7
#define MEM_SUS 0xB0 //suspends current program/erase
#define MEM_RES 0x30 //resumes suspended program/erase
#define MEM_DP 0xB9 //deep power down
#define MEM_RDP 0xAB //release deep power down
#define MEM_WREN 0x06 //write enable
#define MEM_WRDI 0x04 //write disable
#define MEM_SE 0x20 //sector erase
#define MEM_READ 0xEC //4READ4B on datasheet, next 4 bytes are memory address, 6th is dummy
#define MEM_WRITE 0x3E //4PP4B on datasheet, next 4 bytes are memory addredd, followed by 1-256 data cycles

void setup() {

    pinMode(MEM_SIO0, OUTPUT);
    pinMode(MEM_SIO1, OUTPUT);
    pinMode(MEM_SIO2, OUTPUT);
    pinMode(MEM_SIO3, OUTPUT);
    pinMode(MEM_SCLK, OUTPUT);
    pinMode(MEM_CS, OUTPUT);

    digitalWrite(MEM_CS, HIGH);
    digitalWrite(MEM_IN, LOW);
    digitalWrite(MEM_SCLK, LOW);

    Serial.begin(9600);
}

int read_mem(){
    long memVal=0;
    byte cmdBits=b00000000 | MEM_CHANNEL_BYTE << MEM_CHB_LOC;

    digitalWrite(MEM_CS, LOW);

    //writes command bits
    for(int i = 7; i>=4; i--){
        digitalWrite(MEM_DIN, cmdBits & 1<<i);
        digitalWrite(MEM_SCLK, HIGH);
        digitalWrite(MEM_SCLK, LOW);
    }
    int j = MEM_BITC-1
    for(int i = 3; i>= 0; i--, j--){
        digitalWrite(MEM_DIN, cmdBits & 1<<i);
        memVal += digitalRead(MEM_DOUT)<<j;
        digitalWrite(MEM_SCLK, HIGH);
        digitalWrite(MEM_SCLK, LOW);
    }


    for(; j>=0; j--){
        memVal += digitalRead(MEM_DOUT)<<j;
        digitalWrite(MEM_SCLK, HIGH);
        digitalWrite(MEM_SCLK, LOW);
    }
    digitalWrite(MEM_CS, HIGH);

    return memVal;
}

void loop() {

    Serial.println("Val: "+read_mem() + " Time: " + millis());

}



