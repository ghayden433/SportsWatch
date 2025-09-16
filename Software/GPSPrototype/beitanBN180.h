#include "hardware/uart.h"
#include "pico/stdlib.h"

#define BAUD_RATE 9600
#define UART_ID uart1
#define UART_TX_PIN 6
#define UART_RX_PIN 7

class beitianBN180 {
    private:
        uart_inst_t *uart;
        uint8_t baud;
        uint8_t tx;
        uint8_t rx;

    public:
        beitianBN180(uart_inst_t *uart, uint8_t baud, uint8_t tx, uint8_t rx){
            this->uart = uart;
            this->baud = baud;
            this->tx = tx;
            this->rx = rx;

            // Initialize UART
            uart_init(this->uart, this->baud);
            gpio_set_function(this->tx, GPIO_FUNC_UART);
            gpio_set_function(this->rx, GPIO_FUNC_UART);
        }

        void read(){
            
        }
}