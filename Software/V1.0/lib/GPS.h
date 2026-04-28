#include "hardware/uart.h"

class GPS{
public:
    GPS& getInstance();
    int connStrength();
    void read(char* buffer, int bufferSize);


private:
    GPS();
    GPS(GPS&);
};