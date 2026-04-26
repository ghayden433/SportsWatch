#include "config.h"

volatile NavState navigation = IDLE;

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

void hw_init() {
    navigation = IDLE; // initialize navigation state
    stdio_init_all();

    // Initialize UART for GPS
    uart_init(uart1, 9600);
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(uart1, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(uart1, UART_RX_PIN));
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
}
