void setup() {

    pinMode(MEM_SIO0, OUTPUT);
    pinMode(MEM_SIO1, OUTPUT);
    pinMode(MEM_SIO2, OUTPUT);
    pinMode(MEM_SIO3, OUTPUT);
    pinMode(MEM_SCLK, OUTPUT);
    pinMode(MEM_CS, OUTPUT);

    bytesRead = new byte[256];

    digitalWrite(MEM_CS, HIGH);
    digitalWrite(MEM_IN, LOW);
    digitalWrite(MEM_SCLK, LOW);
    
    digitalWrite(MEM_CS, LOW);
    //enabling 4 byte addressing
    writeByte(MEM_EN4B);    
    digitalWrite(MEM_CS, HIGH);

    Serial.begin(9600);
}

char readstring[265];

void loop() {

    if(Serial.available() > 0){
        readstring = Serial.readString();
        //readstring.trim(); //removes \r or \n from end of string // not bothering with this for now, can't trim char[] as string
        Serial.print("Read string: ");
        Serial.println(readstring);
    }
}



