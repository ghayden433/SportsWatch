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

// I2C defines for display
#define DISP_PORT i2c0
#define DISP_SDA 16
#define DISP_SCL 17
#define DISP_ADDR 0x3C

// UART defines for GPS
#define BAUD_RATE 9600
#define UART_ID uart1
#define UART_TX_PIN 6
#define UART_RX_PIN 7

int main()
{
    stdio_init_all();

    // initialize display object
    ssd1306 display(DISP_PORT, DISP_SDA, DISP_SCL, DISP_ADDR);
    
    //initialize GPS object
    beitianBN180 gps(UART_ID, BAUD_RATE, UART_TX_PIN, UART_RX_PIN);

    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, true);   

    while (true){
        char c = gps.read();
        if (c){
            display.print_text(&c);
        }
        sleep_ms(500);
        display.clear();
        gpio_put(LED_PIN, 1);
        sleep_ms(500);
    }
    return 0;
}
