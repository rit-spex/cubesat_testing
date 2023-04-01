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

void tMSCLK(){
    digitalWrite(MEM_SCLK, HIGH);
    digitalWrite(MEM_SCLK, LOW);
}

void writeByte(byte cmd){
    for(int i = 7; i>=0; i--){
        digitalWrite(MEM_SIO0, cmd&1<<i);
        tMSCLK();
    }
}

void 4ioWriteByte(byte cmd){
    digitalWrite(MEM_SIO0,cmd&1<<4);
    digitalWrite(MEM_SIO1,cmd&1<<5);
    digitalWrite(MEM_SIO2,cmd&1<<6);
    digitalWrite(MEM_SIO3,cmd&1<<7);
    tMSCLK();
    digitalWrite(MEM_SIO0,cmd&1<<0);
    digitalWrite(MEM_SIO1,cmd&1<<1);
    digitalWrite(MEM_SIO2,cmd&1<<2);
    digitalWrite(MEM_SIO3,cmd&1<<3);
    tMSCLK();
}

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
    
    digitalWrite(MEM_CS, LOW);
    //enabling 4 byte addressing
    writeCMD(MEM_EN4B);    
    digitalWrite(MEM_CS, HIGH);

    Serial.begin(9600);
}

private byte read_byte(){
    byte outB = 0;
    tMSCLK();
    outB |= digitalRead(MEM_SIO0)<<4;
    outB |= digitalRead(MEM_SIO1)<<5;
    outB |= digitalRead(MEM_SIO2)<<6;
    outB |= digitalRead(MEM_SIO3)<<7;
    tMSCLK();
    outB |= digitalRead(MEM_SIO0)<<0;
    outB |= digitalRead(MEM_SIO1)<<1;
    outB |= digitalRead(MEM_SIO2)<<2;
    outB |= digitalRead(MEM_SIO3)<<3;
    return outB;
}

int read_mem(byte[4] address, byte nBytes){
    long memVal=0;

    digitalWrite(MEM_CS, LOW);

    //writes command bits
    writeByte(MEM_READ);
    for(int i = 3; i >= 0; i--){
       4ioWriteByte(address[i]);
    }
    4ioWriteByte(); //TODO: write whatever performance enhancement indicator is
    
    for(int i = (/*TODO: dummy cycles*/-1); i>=0; i--){
        tMSCLK();
    }

    for(int i = nBytes-1; i>= 0; i--){
        memVal.append(read_byte()); //TODO: figure out what datatype memVal should be (this is also the function return type), probably vector?
    }

    digitalWrite(MEM_CS, HIGH);

    return memVal;
}

void write_mem(byte[4] address, byte* bytes, byte nBytes){
    digitalWrite(MEM_CS, LOW);

    writeByte(MEM_WRITE);
    for(int i = 3; i >=0; i--){
        4ioWriteByte(address[i]);
    }
    
    for(int i = nBytes; i>=0; i--){
        4ioWriteByte(bytes[i]);
    }

    digitalWrite(MEM_CS, HIGH);
}

void loop() {

    Serial.println("Val: "+read_mem() + " Time: " + millis());

}



