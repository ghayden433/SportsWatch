#include "hardware/i2c.h"
#include "font8x8.h" 

#define FRAMEBUFFER_SIZE 1025 // 128x64 pixels / 8 bits per byte + 1 control byte
#define FONT_OFFSET 32 // ASCII offset for font array

class ssd1306 {
    private:
        i2c_inst_t *port;
        uint8_t sda;
        uint8_t scl;
        uint8_t addr;
        uint8_t framebuffer[FRAMEBUFFER_SIZE]; // 128x64 pixels / 8 bits per byte

    public:
        ssd1306(i2c_inst_t *port, uint8_t sda, uint8_t scl, uint8_t addr) {
            this->port = port;
            this->sda = sda;
            this->scl = scl;
            this->addr = addr;

            // Initialize I2C for display, clock speed 400kHz
            i2c_init(port, 400 * 1000);
            gpio_set_function(sda, GPIO_FUNC_I2C);
            gpio_set_function(scl, GPIO_FUNC_I2C);

            // pull-ups are required on I2C lines
            gpio_pull_up(sda);
            gpio_pull_up(scl);

            this->framebuffer[0] = 0x40;   // Control byte for data

            // Initialize display setings
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

            // Set column and page address before sending framebuffer
            uint8_t set_col_page[] = {
                0x00,             // Control byte for commands
                0x21, 0x00, 0x7F, // Set column address: 0 to 127
                0x22, 0x00, 0x07  // Set page address: 0 to 7
            };

            // Send initialization commands to the display
            i2c_write_blocking(port, addr, disp_init_command, sizeof(disp_init_command), false);
            i2c_write_blocking(port, addr, set_col_page, sizeof(set_col_page), false);
            clear(); // Clear display on init
        }

        void clear() {
            for(int i = 1; i < FRAMEBUFFER_SIZE; i++){
                this->framebuffer[i] = 0; 
            }
            i2c_write_blocking(this->port, this->addr, framebuffer, sizeof(framebuffer), false);
        }


        // prints text on the OLED display from the top right
        void print_text(char* text){
            clear  (); // clear display before printing new text
            int base = 1; // start after control byte
            while (*text){
                for (int i = 0; i < 8; i++){
                    framebuffer[base + i] = FONT[*text - FONT_OFFSET][i];
                }
                base += 8;
                text++;
            }
            
            i2c_write_blocking(this->port, this->addr, framebuffer, sizeof(framebuffer), false);
        }
};