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

void quadIOWriteByte(byte cmd){
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

byte bytesRead[256];

void setup() {

    pinMode(MEM_SIO0, OUTPUT);
    pinMode(MEM_SIO1, OUTPUT);
    pinMode(MEM_SIO2, OUTPUT);
    pinMode(MEM_SIO3, OUTPUT);
    pinMode(MEM_SCLK, OUTPUT);
    pinMode(MEM_CS, OUTPUT);

    digitalWrite(MEM_CS, HIGH);
    digitalWrite(MEM_SIO0, LOW);
    digitalWrite(MEM_SCLK, LOW);
    
    digitalWrite(MEM_CS, LOW);
    //enabling 4 byte addressing
    writeByte(MEM_EN4B);    
    digitalWrite(MEM_CS, HIGH);

    Serial.begin(9600);
}

byte read_byte(){
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

byte* read_mem(byte address[4], int nBytes){
    byte bytesRead[256];
    digitalWrite(MEM_CS, LOW);

    //writes command bits
    writeByte(MEM_READ);
    for(int i = 3; i >= 0; i--){
       quadIOWriteByte(address[i]);
    }
    quadIOWriteByte(MEM_PERFEN);
    
    for(int i = (MEM_DUMMYC); i>=0; i--){
        tMSCLK();
    }

    for(int i = nBytes-1; i>= 0; i--){
        bytesRead[i] = (read_byte());
    }

    digitalWrite(MEM_CS, HIGH);

    return bytesRead;
}

void write_mem(byte address[4], byte* bytes, int nBytes){
    digitalWrite(MEM_CS, LOW);
    writeByte(MEM_WREN);
    writeByte(MEM_WRITE);
    for(int i = 3; i >=0; i--){
        quadIOWriteByte(address[i]);
    }
    
    for(int i = nBytes; i>=0; i--){
        quadIOWriteByte(bytes[i]);
    }
    
    writeByte(MEM_WRDI);
    digitalWrite(MEM_CS, HIGH);
}

int size = 0;
String readstring;

union intArr{
  int i;
  byte a[4];
};


void loop() {

    if(Serial.available() > 0){
        readstring = Serial.readString();
        //readstring.trim(); //removes \r or \n from end of string // not bothering with this for now, can't trim char[] as string
        Serial.print("Read string: ");
        Serial.println(readstring);
        int l = 1;
        String address;
        String length;
        switch(readstring[0]){
            case 'r':
                {
                Serial.println("Case r");
                for(int i = 0; i < 4;i++, l++){
                    address += readstring[l];
                }
                Serial.println("Address: "+address);
                for(int i = 0; i < 4; i++, l++){

                  length += readstring[l];
                }
                Serial.println("Length: " + length);
                char returnType = readstring[l];
                intArr T;
                T.i = address.toInt();
                byte* retBytes = (read_mem(T.a, length.toInt()));
                
                switch(returnType){
                    case 'i':
                        {
                        Serial.println("return type i");
                        intArr tempInt;
                        int j = 0;
                        for(int i = 0; i <= length.toInt(); i++, j++){
                            tempInt.a[j] = retBytes[i];
                            //Serial.println(tempInt.a[j]);
                            if(j==3){
                            Serial.print(i/3);
                            Serial.print(" integer: ");
                            Serial.println(tempInt.i);
                            j=0;
                            
                            }
                        }
                        if(j > 0){
                            for(int i = j+1; i<4; i++){
                                tempInt.a[i] = 0;
                            }
                            Serial.print(" integer: ");
                            Serial.println(tempInt.i);
                        }
                        break;
                    }
                    case 'c':
                        {
                        for(int i = 0; i < length.toInt(); i++){
                            Serial.print(char(retBytes[i]));
                        }
                        break;
                    }
                }
                break;
            }
            case 'w':
                {
                address = "";
                byte buffer[256];
                Serial.println("Writing");
                for(int i = 0; i < 4; i++, l++){
                    address += readstring[l];
                }
                
                for(int i = 0; i < 4; i++, l++){
                    length += readstring[l];
                }
                
                
                for(int i = 0; i < length.toInt(); i++, l++){
                    buffer[i] = readstring[l];
                }
                Serial.print("Writing to memory: ");
                intArr T2;
                T2.i = address.toInt();
                write_mem(T2.a, buffer, length.toInt());
                size += length.toInt();
                break;
            }
            case 's':
                {
                Serial.print("Size = ");
                Serial.print(size);
                break;
            }
        }
        Serial.println();
    }
}



