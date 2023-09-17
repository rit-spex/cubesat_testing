#include <SPI.h>
#include <SD.h>

#ifndef SPEXSAT_BAUD
#define SPEXSAT_BAUD 9600
#endif

#ifndef SPEXSAT_BOARD_MEGA
#define SPEXSAT_BOARD_MEGA 0
#endif

#ifndef SPEXSAT_BOARD_UNO
#define SPEXSAT_BOARD_UNO 1
#endif

#ifndef SPEXSAT_BOARD
#define SPEXSAT_BOARD SPEXSAT_BOARD_MEGA
#endif

#if SPEXSAT_BOARD == SPEXSAT_BOARD_MEGA
#define SD_CS_PIN 53
#else
#define SD_CS_PIN
#endif

File dataCSV;
void setup_SD(){
    while(!SD.begin(SD_CS_PIN));
}

void write_SD_line(void* data_arr, unsigned int array_len) {
    dataCSV = SD.open("data.csv", FILE_WRITE);
    for(int i = 0; i < array_len; i++){
        dataCSV.print(*(data_arr+i));
        if(i != array_len -1){
            dataCSV.print(',');
    }
    dataCSV.println();
    dataCSV.close();
}

void setup(){
    setup_SD();
}

void loop(void){
    for(int i = 0; i < 10; i++){
        char* l = "Data line: ";
        char* p = " test.";
        int j = i;
        void* arr[3] = {l, &j, p};
        write_SD_line(arr, 3);
    }
    while(1);
}

