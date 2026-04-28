#include "GPS.h"

void GPS::read(char* buffer, int bufferSize){
    while (uart_is_readable(uart1)){
        char c;
        int i = 0;
        while ((c = uart_getc(uart1)) != '\n' && i < bufferSize - 1){
            buffer[i++] = c;
        }
        buffer[i] = '\0';
    }
}
