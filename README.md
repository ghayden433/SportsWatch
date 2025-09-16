This GPS watch project began from my interest in building a custom fitness device that records accurate run and cycle data and allows users to create and implement their own features.

# Hardware🖥️🔧
### Prototype
- Based on RP2040, prototyped on pi pico
- features ssd1306 OLED Display over I2C
- Beitian BN-180 for GPS over UART
- SD for storage of SPI
- Button interrupts for navigation
- going prototype route at the moment due to cost of manufacturing PCBs

### V1.0
- STM32L476 Microprocessor
- Quectel L96 for GPS
- BMA423 for step tracking
- Rotary Encoder and push buttons for navigation
- USB Micro B supports power and data for charging and programming as a device
- SD storage
- I2C 128x64 OLED display

### V1.1
#### Updates
- STM32F103
- BMA456 as pedometer
- Smaller footprint rotary encoder
- Bottom side no longer requires assembly

# Software 💾
### Prototype
- written in C++ useing object oriented methodology
- bare metal software
- features I2C display software
- this is a rewrite of the python version that I lost on the prototype hardware, and it currently displays the letter B on the display at startup

