#include <stdio.h>
#include "hardware/gpio.h"
#include "lib/ssd1306.h"
#include "lib/BeitanBN180.h"
#include "pico/stdio_usb.h"
#include "lib/SD.h"


#include "config.h"


int main()
{
    hw_init();

    SD& fileSystem = SD::getInstance();
    fileSystem.mount();
    fileSystem.openFile("myFile.txt");
    fileSystem.write("I really hope this works\n");
    fileSystem.closeFile();
    fileSystem.unmount();


    uart_puts(uart1, "$PMTK353,1,1,1,1,1*2A\r\n"); 
    uart_puts(uart1, "$PMTK314,-1*04\r\n"); 
    uart_puts(uart1, "$PMTK000*32\r\n");

    while(true){
        if (uart_is_readable(uart1)){
            char c;
            while ((c = uart_getc(uart1)) != '\n'){
                printf("%c", c);
            }
            printf("\n");
        }
                switch (navigation) {
            case IDLE:
                // do nothing
                break;
            case SELECT:
                printf("SELECT button pressed\n");
                navigation = IDLE; // reset state
                break;
            case BACK:
                printf("BACK button pressed\n");
                navigation = IDLE; // reset state
                break;
        }
    }

    return 0;
}
