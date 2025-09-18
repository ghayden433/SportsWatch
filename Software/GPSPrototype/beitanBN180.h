#include "hardware/uart.h"
#include "pico/stdlib.h"

#define BAUD_RATE 9600
#define UART_ID uart1
#define UART_TX_PIN 6
#define UART_RX_PIN 7

class beitianBN180 {
    private:
        uart_inst_t *uart_id;
        uint16_t baud;
        uint8_t tx;
        uint8_t rx;

    public:
        beitianBN180(uart_inst_t *uart, uint16_t baud, uint8_t tx, uint8_t rx){
            this->uart_id = uart;
            this->baud = baud;
            this->tx = tx;
            this->rx = rx;

            // Initialize UART
            uart_init(this->uart_id, this->baud);
            gpio_set_function(this->tx, GPIO_FUNC_UART);
            gpio_set_function(this->rx, GPIO_FUNC_UART);

            uart_set_format(this->uart_id, 8, 1, UART_PARITY_NONE);
            uart_set_fifo_enabled(this->uart_id, true);
        }

        char read(){
            if(uart_is_readable(this->uart_id)){
                char c = uart_getc(this->uart_id);
                return c;
            }
            return 0;
        }
};