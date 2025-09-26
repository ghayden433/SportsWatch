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
#define UART_TX_PIN 4
#define UART_RX_PIN 5

int main()
{
    //stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 0);
    sleep_ms(500);


    // initialize display object
    ssd1306 display(DISP_PORT, DISP_SDA, DISP_SCL, DISP_ADDR);
    
    //initialize GPS object
    beitianBN180 gps(UART_ID, BAUD_RATE, UART_TX_PIN, UART_RX_PIN);

    gpio_put(25, 1);

    // initialize buffer for GPS data
    uint8_t gps_data_len = 129; 
    char gps_data[gps_data_len];
    for (int i = 0; i < gps_data_len; i++){
        gps_data[i] = 'B';
    }
    gps_data[gps_data_len - 1] = '\0'; // null terminator

    while (true){
        sleep_ms(1000);
        display.print_text(gps_data);

        //clear buffer
        for (int i = 0; i < gps_data_len; i++){
            gps_data[i] = ' ';
        }
        gps.read_nmea_sentence(gps_data, gps_data_len);
        gpio_put(25, 1);
    }
    return 0;
}
