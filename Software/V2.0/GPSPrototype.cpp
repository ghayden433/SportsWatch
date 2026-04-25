#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "lib/ssd1306.h"
#include "lib/BeitanBN180.h"
#include "pico/stdio_usb.h"

// Include FatFS headers
#include "ff.h"
#include "sd_card.h"

// I2C defines for display
#define DISP_PORT i2c0
#define DISP_SDA 16
#define DISP_SCL 17
#define DISP_ADDR 0x3C

// UART defines for GPS
// needs fixed to match GPS wiring
#define BAUD_RATE 9600
#define UART_ID uart1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
 
// navigation pin definitions
#define SELECT_BUTTON_PIN 2
#define BACK_BUTTON_PIN 3


// initialize navigation state
typedef enum NavState {
    IDLE,
    SELECT,
    BACK
} NavState;

volatile NavState navigation = IDLE; 

void read_response(uint32_t timeout_ms) {
    char buf[256];
    int idx = 0;
    uint32_t start = to_ms_since_boot(get_absolute_time());

    while (to_ms_since_boot(get_absolute_time()) - start < timeout_ms) {
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if (c == '\n') {
                buf[idx] = '\0';
                printf("%s\n", buf);
                idx = 0;
            } else if (c != '\r') {
                if (idx < 256 - 1)
                    buf[idx++] = c;
            }
        }
    }
}

// Select interrupt service routine
void gpio_interrupt(uint gpio, uint32_t events) {
    switch (gpio) {
        case SELECT_BUTTON_PIN:
            navigation = SELECT;
            break;
        case BACK_BUTTON_PIN:
            navigation = BACK;
            break;
    }
    //sleep_ms(100); // debounce delay
}


int main()
{
    stdio_init_all();

    // Initialize UART for GPS
    uart_init(uart1, 9600);
    gpio_set_function(20, UART_FUNCSEL_NUM(uart1, 20));
    gpio_set_function(21, UART_FUNCSEL_NUM(uart1, 21));
    gpio_init(28);
    gpio_set_dir(28, GPIO_IN);

    // select button interrupt initialization 
    gpio_init(SELECT_BUTTON_PIN);
    gpio_set_dir(SELECT_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(SELECT_BUTTON_PIN);
    gpio_set_irq_enabled(SELECT_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);

    // back button interrupt initialization
    gpio_init(BACK_BUTTON_PIN);
    gpio_set_dir(BACK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BACK_BUTTON_PIN);
    gpio_set_irq_enabled(BACK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);

    // give callback function for irq and enable interrupts
    gpio_set_irq_callback(&gpio_interrupt);
    irq_set_enabled(IO_IRQ_BANK0, true);

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    sleep_ms(500);

/*
    int selpress = 0;
    while(true){
        sleep_ms(50); //attempting to debounce, but doesn't actually work
        switch (navigation) {
            case IDLE:
                // do nothing
                break;
            case SELECT:
                ++selpress;
                printf("SELECT button pressed: %d\n", selpress);
                navigation = IDLE; // reset state
                break;
            case BACK:
                printf("BACK button pressed\n");
                navigation = IDLE; // reset state
                break;
        }
    }
*/



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

    // Open/create file
    fr = f_open(&fil, "0:/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("Open failed: %d\n", fr);
        return 1;
    }

    // Write
    f_printf(&fil, "Hello from Pico!\n");

    // Close and unmount
    f_close(&fil);
    f_unmount("0:");

    printf("Done!\n");


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
