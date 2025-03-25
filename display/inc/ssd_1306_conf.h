#ifndef SSD1306_CONFIG_H
#define SSD1306_CONFIG_H

/*********************** includes *******************************/

/*********************** Definições *******************************/

// Choose a bus
#define SSD1306_USE_I2C

#define SSD1306_I2C_PORT        i2c1
#define SSD1306_I2C_ADDR        0x3C //(0x3C << 1)

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26
#define SSD1306_INCLUDE_FONT_16x24
#define SSD1306_INCLUDE_FONT_16x15

// The width of the screen can be set using this
// define. The default value is 128.
#define SSD1306_WIDTH   128

// If your screen horizontal axis does not start
// in column 0 you can use this define to
// adjust the horizontal offset
// #define SSD1306_X_OFFSET

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
#define SSD1306_HEIGHT   64

#endif //SSD1306_CONFIG_H