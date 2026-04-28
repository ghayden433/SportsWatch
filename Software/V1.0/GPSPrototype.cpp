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

    /*
    SD& fileSystem = SD::getInstance();
    fileSystem.mount();
    fileSystem.openFile("myFile.txt");
    fileSystem.write("I really hope this works\n");
    fileSystem.closeFile();
    fileSystem.unmount();
    */


    
    while(true){
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
