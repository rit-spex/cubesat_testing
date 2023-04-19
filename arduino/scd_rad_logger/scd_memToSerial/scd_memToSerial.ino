#ifndef SPEXSAT_BAUD
    #define SPEXSAT_BAUD 9600
#endif

//#include <SPI.h>

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
#define MEM_CE 0x60 //chip erase
#define MEM_READ 0xEB //4READ on datasheet, next 4 bytes are memory address, 6th is dummy
#define MEM_WRITE 0x38 //4PP on datasheet, next 4 bytes are memory addredd, followed by 1-256 data cycles
#define MEM_RDSR 0x05 //read status register
#define MEM_PERFEN 0x00 //performance enhance mode is for repeated reads, not something we need here
#define MEM_ENQIO 0x35

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
    digitalWrite(MEM_SIO1, LOW);
    digitalWrite(MEM_SIO2, LOW);
    digitalWrite(MEM_SIO3, LOW);
    digitalWrite(MEM_SCLK, LOW);
    delay(100);
    digitalWrite(MEM_CS, LOW);
    //enabling 4 byte addressing
    writeByte(MEM_EN4B);    
    digitalWrite(MEM_CS, HIGH);

    digitalWrite(MEM_CS, LOW);
    writeByte(0x98);
    digitalWrite(MEM_CS, HIGH);

    digitalWrite(MEM_CS, LOW);
    writeByte(MEM_ENQIO);
    digitalWrite(MEM_CS, HIGH);

    Serial.begin(9600);

}

byte basic_read(){
    byte out = 0;
    pinMode(MEM_SIO0, INPUT);
    for(int i = 7; i >= 0; i--){
        out |= digitalRead(MEM_SIO1)<<i;
    }
    pinMode(MEM_SIO0, OUTPUT);
    return out;
}

byte read_byte(){
    //return basic_read();
    pinMode(MEM_SIO0, INPUT);
    pinMode(MEM_SIO1, INPUT);
    pinMode(MEM_SIO2, INPUT);
    pinMode(MEM_SIO3, INPUT);
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
    pinMode(MEM_SIO0, OUTPUT);
    pinMode(MEM_SIO1, OUTPUT);
    pinMode(MEM_SIO2, OUTPUT);
    pinMode(MEM_SIO3, OUTPUT);
    return outB;
}

byte* read_mem(byte address[4], int nBytes){
    byte bytesRead[256];
    digitalWrite(MEM_CS, LOW);
    //writes command bits
    quadIOWriteByte(MEM_READ);
    for(int i = 3; i >= 0; i--){
       quadIOWriteByte(address[i]);
    }
    quadIOWriteByte(MEM_PERFEN);
    
    for(int i = 0; i<MEM_DUMMYC-1; i++){
        tMSCLK();
    }
    Serial.println("Reading bytes");
    for(int i = nBytes-1; i>= 0; i--){
      
        bytesRead[i] = (read_byte());
        Serial.print(bytesRead[i]);
        Serial.print(" ");

    }
    Serial.println();
    digitalWrite(MEM_CS, HIGH);

    return bytesRead;
}

void write_mem(byte address[4], byte* bytes, int nBytes){
    Serial.print("Writing: ");
    Serial.print("Length = ");
    Serial.println(nBytes);
    digitalWrite(MEM_CS, LOW);
    delay(1);
    quadIOWriteByte(MEM_WREN);
    delay(1);
    digitalWrite(MEM_CS, HIGH);
    delay(1);
    digitalWrite(MEM_CS, LOW);
    quadIOWriteByte(MEM_WRITE);
    for(int i = 3; i >=0; i--){
        quadIOWriteByte(address[i]);
    }
    Serial.println("Writing bytes");
    for(int i = nBytes; i>=0; i--){
        Serial.println(bytes[i]);
        quadIOWriteByte(bytes[i]);
    }
    
    quadIOWriteByte(MEM_WRDI);
    digitalWrite(MEM_CS, HIGH);
}

int size = 0;
String readstring;

byte* intToBytes(int a){
  byte arr[4] = {0};
  arr[0] = (a & 0xff);
  arr[1] = ((a >> 8)& 0xff);
  arr[2] = ((a>>16)&0xff);
  arr[3] = ((a>>24)&0xff);
  return arr;
}

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
                Serial.print("address int: "); Serial.print(address.toInt()); Serial.print(" length int: "); Serial.println(length.toInt());
                byte* retBytes = (read_mem(intToBytes(address.toInt()), length.toInt()));
                
                switch(returnType){
                    /*case 'i':
                        {
                        Serial.println("return type i");
                        byte tempInt[4];
                        int j = 0;
                        for(int i = 0; i <= length.toInt(); i++, j++){
                            //tempInt.a[j] = retBytes[i];
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
                    }*/
                    case 'c':
                        {
                        for(int i = 0; i < length.toInt(); i++){
                            Serial.print(char(retBytes[i]));
                        }
                        break;
                    }
                    case 'b':
                        {
                        for(int i = 0; i < length.toInt(); i++){
                            Serial.println((retBytes[i]));
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
                write_mem(intToBytes(address.toInt()), buffer, length.toInt());
                size += length.toInt();
                break;
            }
            case 's':
                {
                Serial.print("Size = ");
                Serial.println(size);
                byte b = 0;
                digitalWrite(MEM_CS, LOW);
                quadIOWriteByte(MEM_RDSR);
                digitalWrite(MEM_CS, HIGH);
                b = read_byte();
                Serial.println(b, BIN);
                /*do{
                    digitalWrite(MEM_CS, LOW);
                    quadIOWriteByte(MEM_WREN);
                    digitalWrite(MEM_CS, HIGH);
                    digitalWrite(MEM_CS, LOW);
                    quadIOWriteByte(MEM_RDSR);
                    digitalWrite(MEM_CS, HIGH);
                    b = read_byte();
                    Serial.print(b, BIN);
                    Serial.print("| b && 0b10: ");
                    Serial.println(b && 0b10);
                  }while((b & 0b10) != 2);*/
                quadIOWriteByte(MEM_RDSR);
                b = 0;
                b = read_byte();
                digitalWrite(MEM_CS, HIGH);
                Serial.println();
                Serial.println(b, BIN);
                break;
            }
            case 'c':
                {
                if(readstring[1] == 'y'){
                  digitalWrite(MEM_CS, LOW);
                  byte b = 0;
                  do{
                    writeByte(MEM_WREN);
                    writeByte(MEM_RDSR);
                    b = read_byte();
                  }while(!(b && 0b1));
                  writeByte(MEM_CE);
                  Serial.println("Chip erased");
                }
                break;
            }
          case 'd':
                {
                if(readstring[1] == 'y'){
                  digitalWrite(MEM_CS, LOW);
                  quadIOWriteByte(0x66);
                  digitalWrite(MEM_CS, HIGH);
                  //wait(tSHSL);
                  digitalWrite(MEM_CS, LOW);
                  quadIOWriteByte(0x99);
                  digitalWrite(MEM_CS, HIGH);
                  digitalWrite(MEM_CS, LOW);
                  quadIOWriteByte(0x66);
                  digitalWrite(MEM_CS, HIGH);
                  Serial.println("Chip reset");
                }
                break;
            }
        }
        Serial.println();
    }
}



