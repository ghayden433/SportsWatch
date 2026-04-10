#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "ssd1306.h"
#include "BeitanBN180.h"
#include "pico/stdio_usb.h"

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

    /*
    //uart_puts(uart1, "$PMTK104*37\r\n"); // GPS cold start command
    uart_puts(uart1, "$PMTK314,-1*04\r\n"); // Set NMEA sentence output to all
    uart_puts(uart1, "$PMTK605*3\r\n"); // Set update rate to 1Hz

    printf("\n--- Firmware Version (PMTK605) ---\n");
    uart_puts(uart1, "$PMTK605*31");
    read_response(2000);

    // Query NMEA output config
    printf("\n--- NMEA Output Config (PMTK414) ---\n");
    uart_puts(uart1, "$PMTK414*33");
    read_response(2000);

    while(true){
        if (uart_is_readable(uart1)){
            char c;
            while ((c = uart_getc(uart1)) != '\n'){
                printf("%c", c);
            }
            uart_puts(uart1, "$PMTK605*3\r\n");
            printf("\n");
        }
    }
        */

    return 0;
}
