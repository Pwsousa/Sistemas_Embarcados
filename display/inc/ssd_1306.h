#ifndef SSD1306_H
#define SSD1306_H

/******************** includes  *********************/
#include <stddef.h>
#include <stdint.h>
#include <_ansi.h>

#include "ssd_1306_conf.h"

/******************** Definições  ********************/
#define SSD1306_I2C_CLK 400

#ifdef SSD1306_X_OFFSET
#define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#endif


#ifndef SSD1306_I2C_PORT
#define SSD1306_I2C_PORT        i2c1
#endif

#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR        0x3C
#endif

#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif

#ifndef SSD1306_BUFFER_SIZE
#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8
#endif

/******************************** comandos define ****************************************/
#define MAX_CONTRAST            0xFF
#define SSD1306_ON              1
#define SSD1306_OFF             0

/******************************** comandos fundamentais **********************************/

#define SET_CONTRAST_CONTROL    0x81
#define DISPLAY_ON              0xA4
#define DISPLAY_OFF             0XA4

/******************************** comandos de memoria **********************************/
#define SET_LOWER_COLUMN        0x00
#define SET_HIGHER_COLUMN       0x10
#define SET_MEMORY_ADDR_MODE    0X20
#define SET_COLUMN_ADDR_MODE    0x21
#define SET_PAGE_ADDR           0x22
#define SET_PAGE_START          0xB0

/******************************** Hardware Configuration ********************************/
#define SET_DISPLAY_LINE        0x40
#define SET_SEGMENT_RE_MAP      0x0A
#define SET_COM_OUT_SCAN_DIRE   0xC8

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

typedef struct {
    uint8_t x;
    uint8_t y;
} ssd_1306_verti;

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

/***************************** Funções de inversão **************/
void ssd_1306_fill(ssd_1306_color color);
void ssd_1306_fill_circle(uint8_t cord_x, uint8_t cord_y, uint8_t par_r, ssd_1306_color color);
void ssd_1306_fill_rectangle(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, ssd_1306_color color);
ssd_1306_erro ssd_1306_inverte_retangulo(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1);

/**************************** Funções de desenhos ****************/ 
void ssd_1306_draw_pixel(uint8_t x, uint8_t y, ssd_1306_color color);
void ssd_1306_draw_line(uint8_t x_1, uint8_t y_1, uint8_t x_0, uint8_t y_0, ssd_1306_color color);
void ssd_1306_draw_arc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, ssd_1306_color color);
void ssd_1306_draw_circle(uint8_t cord_x, uint8_t cord_y, uint8_t par_r, ssd_1306_color color);
void ssd_1306_draw_polyline(const ssd_1306_verti *verte, uint16_t size, ssd_1306_color color);
void ssd_1306_draw_rectangle(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, ssd_1306_color color);
void ssd_1306_draw_bitmap(uint8_t x, uint8_t y, const  unsigned char *bitmap, uint8_t w, uint8_t h, ssd_1306_color color);

/******************************** Funções de escrita ********************************************************************/ 
char ssd_1306_write_string(char *str, ssd_1306_font font, ssd_1306_color color);
char ssd_1306_write_char(char _char_, ssd_1306_font font, ssd_1306_color color);

/****************************** Funções de set ***********************************/
void ssd_1306_set_cursor(uint8_t x, uint8_t y);
void ssd_1306_set_contrast(const uint8_t value);
void ssd_1306_set_display_on_off(const uint8_t on);

/**************************** Funções get ****************************************/
uint8_t ssd_1306_get_display_on(void);

/*********************** Funções de baixo nível********************/
void ssd_1306_reset(void);
void ssd_1306_write_command(uint8_t byte);
void ssd_1306_write_data(uint8_t *buff, size_t buff_size);
ssd_1306_erro ssd_1306_fill_buffer(uint8_t *buff, uint32_t _len);

#endif // SSD1306_H