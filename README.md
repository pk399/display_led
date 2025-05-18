# LED Display

This is a code for a DIY project of a RGB LED Matrix

## Folder structure

- `src` - code for the Raspberry Pi Pico microcontroller, `CMakeLists.txt` builds it
- `ESP32_Server` - code for the ESP32 for sending inputs through UART

## Hardware setup

We used the following components in the assembly:

1. 1x Raspberry Pi Pico
2. 30x RGB LEDs (5*6 matrix)
3. 18x resistors (1 per color per row of LEDs)
4. 3x shift registers
5. 1x ESP32 dev board

Connections:

1. Raspberry Pi supplies 3.3V power and ground lines
2. Shift registers are chained together, supplying the matrix, RPi connected to them with 3 pins
3. ESP32's ground is connected with RPi, also TX, RX
