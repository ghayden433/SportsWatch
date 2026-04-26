#include <stdio.h>
#include "hardware/gpio.h"
#include "lib/ssd1306.h"
#include "lib/BeitanBN180.h"
#include "pico/stdio_usb.h"

// Include FatFS headers
#include "ff.h"
#include "sd_card.h"

#include "config.h"


int main()
{
    hw_init();
    // todo: fix config.cpp

    /*
    //SD Card demo write
    FATFS fs;
    FIL fil;
    FRESULT fr;

    // Mount
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("Mount failed: %d\n", fr);
        return 1;
    }

    // Open or create a file
    fr = f_open(&fil, "0:/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("Open failed: %d\n", fr);
        return 1;
    }

    //uint64_t start = time_us_64();
    // Write
    f_printf(&fil, "Let's hope this works\n");
    //uint64_t end = time_us_64();

    // Close and unmount
    f_close(&fil);
    f_unmount("0:");

    //printf("Done! Time: %d ms\n", (end - start));
    */

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
