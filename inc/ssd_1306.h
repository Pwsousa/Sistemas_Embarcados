#ifndef SSD1306_H
#define SSD1306_H

/******************** includes  ********************/
#include <stddef.h>
#include <stdint.h>
#include <_ansi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "math.h"

/******************** Definições  ********************/
#define I2C_SDA_PIN             14
#define I2C_SCL_PIN             15
#define SSD1306_I2C_CLK         400
#define SSD1306_HEIGHT          64
#define SSD1306_WIDTH           128
#define SSD1306_BUFFER_SIZE     SSD1306_WIDTH * SSD1306_HEIGHT / 8
#define SSD1306_I2C_PORT        i2c1
#define SSD1306_I2C_ADDR        0x3C

/******************** Estruturas  ********************/
typedef struct{
    uint16_t corde_x;
    uint16_t corde_y;
    uint8_t iniciali;
    uint8_t display_on;
}ssd_1306_t;

typedef struct{
    uint8_t x;
    uint8_t y;
}ssd_1306_vertice;

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *const data;
    const uint16_t *const char_width;
}ssd_1306_font;

/******************** Enumerações  ********************/

typedef enum{
    black = 0x00, // Nenhum pixel
    white = 0x01 // Pixel foi setado
}ssd_1306_color;

typedef enum{
    OK = 0x00,
    ERROR = 0x01
}ssd_1306_erro;
/******************** Cabeçalhos das funções ********************/

void ssd_1306_init(void);
void ssd_1306_up_date_screen(void);
void ssd_1306_fill();

void ssd_1306_write_command(uint8_t);
void ssd_1306_write_data(uint8_t*, size_t);
void ssd_1306_reset(void);
void ssd_1306_set_display_on_off(bool);
#endif // SSD1306_H