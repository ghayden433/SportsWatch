#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include <string.h>
#include "ssd1306.h"

// I2C defines for display
#define DISP_PORT i2c0
#define DISP_SDA 16
#define DISP_SCL 17
#define DISP_ADDR 0x3C

int main()
{
    stdio_init_all();

    ssd1306 display(DISP_PORT, DISP_SDA, DISP_SCL, DISP_ADDR);
    display.printB();

    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, true);  
    gpio_put(LED_PIN, 1); // Turn on the LED to indicate the oled thing is over
}
