#include "hardware/uart.h"

struct GPSFix {
    double latitude;
    double longitude;
    double altitude;
    int numSatellites;
    int fixQuality;
};

/*
*   Singleton class to manage GPS operations using UART.
*   provides methods to read various GPS data and check connection strength.
*/
class GPS{
public:
    GPS& getInstance();
    int connStrength();
    void getFix(GPSFix& fix);

private:
    GPS();
    GPS(GPS&);
    void readUART(char* buffer, const int bufferSize);
    void parseNMEA(const char* nmeaSentence, GPSFix& fix);

    const int numSentences;
};