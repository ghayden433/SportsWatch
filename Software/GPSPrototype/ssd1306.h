#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>


class ssd1306 {
    public:
        i2c_inst_t *port;
        uint8_t sda;
        uint8_t scl;
        uint8_t addr;

        ssd1306(i2c_inst_t *port, uint8_t sda, uint8_t scl, uint8_t addr) {
            this->port = port;
            this->sda = sda;
            this->scl = scl;
            this->addr = addr;

            // Initialize I2C for display
            i2c_init(port, 400 * 1000);
            gpio_set_function(sda, GPIO_FUNC_I2C);
            gpio_set_function(scl, GPIO_FUNC_I2C);

            // pull-ups are required on I2C lines
            gpio_pull_up(sda);
            gpio_pull_up(scl);

            // control bytes
            uint8_t disp_init_command[] = {
                0x00,       // Control byte for commands
                0xAE,       // Display OFF
                0xD5, 0x80, // Set display clock divide ratio/oscillator frequency
                0xA8, 0x3F, // Set multiplex ratio (1 to 64)
                0xD3, 0x00, // Set display offset
                0x40,       // Set start line address
                0x8D, 0x14, // Enable charge pump
                0x20, 0x00, // Memory addressing mode: horizontal
                0xA1,       // Set segment re-map 0 to 127
                0xC8,       // Set COM output scan direction: remapped mode
                0xDA, 0x12, // Set COM pins hardware configuration
                0x81, 0xCF, // Set contrast control
                0xD9, 0xF1, // Set pre-charge period
                0xDB, 0x40, // Set VCOMH deselect level
                0xA4,       // Entire display ON (resume RAM content display)
                0xA6,       // Set normal display (not inverted)
                0xAF        // Display ON
            };

            // Send initialization commands to the display
            i2c_write_blocking(port, addr, disp_init_command, sizeof(disp_init_command), false);
            // Small delay to ensure the display is ready
            sleep_ms(100);

            // Initialize framebuffer
            uint8_t data_cb = 0x40;
            uint8_t framebuffer[1024];
            memset(framebuffer, 0x00, sizeof(framebuffer)); // Fill framebuffer with black pixels

            // Set column and page address before sending framebuffer
            uint8_t set_col_page[] = {
                0x00,       // Control byte for commands
                0x21, 0x00, 0x7F, // Set column address: 0 to 127
                0x22, 0x00, 0x07  // Set page address: 0 to 7
            };
            i2c_write_blocking(port, addr, set_col_page, sizeof(set_col_page), false);
        }


        
        int printB() {
            // initialize framebuffer
            uint8_t data_cb = 0x40;
            uint8_t framebuffer[1024];
            memset(framebuffer, 0x00, sizeof(framebuffer)); // Fill framebuffer with black pixels

            // Set column and page address before sending framebuffer
            uint8_t set_col_page[] = {
                0x00,       // Control byte for commands
                0x21, 0x00, 0x7F, // Set column address: 0 to 127
                0x22, 0x00, 0x07  // Set page address: 0 to 7
            };
            i2c_write_blocking(port, addr, set_col_page, sizeof(set_col_page), false);

            // Prepare data buffer: 0x40 followed by framebuffer
            uint8_t oled_data[1025];
            oled_data[0] = 0x40;
            memcpy(&oled_data[1], framebuffer, 1024);
            oled_data[1] = 0x00;
            oled_data[2] = 0x7F;
            oled_data[3] = 0x7F;
            oled_data[4] = 0x49;
            oled_data[5] = 0x49;
            oled_data[6] = 0x7F;
            oled_data[7] = 0x36;
            oled_data[8] = 0x00;

            // Send framebuffer data to the display in one transaction
            i2c_write_blocking(this->port, this->addr, oled_data, sizeof(oled_data), false);

            return 0;
        }
};