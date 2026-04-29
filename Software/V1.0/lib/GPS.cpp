#include "GPS.h"

// declare static memebers
static GPS* instance = nullptr;

GPS::GPS() : numSentences(8) {
    // constructor code here
}

GPS& GPS::getInstance() {
    if (instance == nullptr) {
        instance = new GPS();
    }
    return *instance;
}

void GPS::readUART(char* buffer, const int bufferSize){
    while (uart_is_readable(uart1)){
        char c;
        int i = 0;
        while ((c = uart_getc(uart1)) != '\n' && i < bufferSize - 1){
            buffer[i++] = c;
        }
        buffer[i] = '\0';
    }
}


void parseNMEA(const char* nmeaSentence, GPSFix& fix){
    char fixType[6];
    
    //copy the first 5 characters of the sentence to fixType
    for (int i =0; i < 5; ++i){
        fixType[i] = nmeaSentence[i];
    }

    /*
    switch (fixType):
        case "$GPGGA":
            break;
        case "$GPRMC":
            break;
        default:
            break;
    */
}


void GPS::getFix(GPSFix& fix){
    // to hold the raw GPS data from UART
    char buffer[100];
    for (int i = 0; i < numSentences; ++i) {
        readUART(buffer, 100);
        parseNMEA(buffer, fix);
    }

}

//sample nmea data
/*
$GPRMC,235951.101,V,,,,,0.00,0.00,050180,,,N,V*32
$GPVTG,0.00,T,,M,0.00,N,0.00,K,N*32
$GPGGA,235951.101,,,,,0,0,,,M,,M,,*41
$GPGSA,A,1,,,,,,,,,,,,,,,,1*03
$GPGSA,A,1,,,,,,,,,,,,,,,,2*00
$GPGSV,1,1,00,0*65
$GLGSV,1,1,00,1*78
$GPGLL,,,,,235951.101,V,N*73
*/