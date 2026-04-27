#pragma once

#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"

// I2C defines for display
#define DISP_PORT i2c0
#define DISP_SDA 16
#define DISP_SCL 17
#define DISP_ADDR 0x3C

// UART defines for GPS
// needs fixed to match GPS wiring
#define BAUD_RATE 9600
#define UART_ID uart1
#define UART_TX_PIN 20
#define UART_RX_PIN 21
 
// navigation pin definitions
#define SELECT_BUTTON_PIN 2
#define BACK_BUTTON_PIN 3

// initialize navigation state
typedef enum NavState {
    IDLE,
    SELECT,
    BACK
} NavState;

extern volatile NavState navigation; 


void gpio_interrupt(uint gpio, uint32_t events);
void hw_init();


