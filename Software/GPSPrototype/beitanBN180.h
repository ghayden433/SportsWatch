#include "hardware/uart.h"
#include "pico/stdlib.h"

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

        //read len chars from UART
        void read(char* buf, int len){
            buf[len-1] = '\0'; // initialize null terminator 

            for (int i = 0; i < len-1; i++){
                if(uart_is_readable(this->uart_id)){
                     buf[i] = uart_getc(this->uart_id);
                }
                else{
                    buf[i] = '!'; // if no data, fill with !
                }
            }  
        }

        void read_nmea_sentence(char* buf, int len) {
            buf[len - 1] = 0; // initialize null terminator
            char c = 0;
            int i = 0;
            while(c != '\r') {
                while (!uart_is_readable(this->uart_id)) { tight_loop_contents(); }
                c = uart_getc(this->uart_id);
                buf[i++] = c;
                if (i >= len - 1) break; // prevent overflow
            }
        }
};