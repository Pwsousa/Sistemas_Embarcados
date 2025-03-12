/************************* includes *************************/
#include "ssd_1306.h"

/************************* variaveis *************************/
static uint8_t buffer[SSD1306_BUFFER_SIZE];
static ssd_1306_t ssd_1306;
/************************* Funcoes *************************/
/**
 * @
 */
void ssd_1306_reset(void){
    /*do nothing*/
}

void ssd_1306_write_command(uint8_t byte){
    uint8_t _buffer_[2];
    _buffer_[0] = 0x00;
    _buffer_[1] = byte;
    i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, _buffer_, sizeof(_buffer_), false);
}

void ssd_1306_write_data(uint8_t *buff, size_t buff_size){
    uint8_t _buffer_temp_[buff_size + 1];
    _buffer_temp_[0] = 0x40;

    memcpy(&_buffer_temp_[1], buff, buff_size);
    i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, _buffer_temp_, sizeof(_buffer_temp_), false);
   
}

/**
 * @
 */
void ssd_1306_init(void){
    ssd_1306_reset();

    sleep_ms(50);

    // Inicializar o i2c
    i2c_init(i2c1, SSD1306_I2C_CLK*1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    /*Comandos para o OLED*/
    ssd_1306_set_display_on_off(false); // display off
    ssd_1306_write_command(0x20);       // memory addressing mode
    ssd_1306_write_command(0x00);       // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                        // 10b,Page Addressing Mode (RESET); 11b,Invalid
    
    ssd_1306_write_command(0xB0);

}

/**
 * @author Lucas Guilherme
 * @param ssd_1306_color
 * @details Preenche o buffer com a cor especificada
 */
void ssd_1306_fill(ssd_1306_color color){
    memset(buffer, (color == black) ? 0x00: 0xFF, sizeof(buffer));
}

/**
 * @author Lucas Guilherme
 */
 uint8_t ssd_1306_get_display_on(void){
    return ssd_1306.display_on;
}

void ssd_1306_set_display_on_off(bool on){
    if(on){
        ssd_1306.display_on = 1;
        ssd_1306_write_command(0xAF); //display on
    }else{
        ssd_1306.display_on = 0;
        ssd_1306_write_command(0xAE); // display off
    }
}

/**
 * @
 */
void ssd_1306_draw_bitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, ssd_1306_color color){
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
        return;
    
    uint16_t byte_widt = (w + 7)/8;
    uint8_t byte = 0;

    for(int i = 0; i<h; i++, y++){
        for(int j = 0; j < w; j++){
            if(j & 7)
                byte <<= 1;
            else 
                byte = (*(const unsigned char*)(&bitmap[i*byte_widt + j/8]));
            if(byte & 0x80)
                ssd_1306_draw_pixel(x + j, y, color);
        }
    }
    return;
}

/**
 * 
 */
void ssd_1306_set_contrast(const uint8_t value){
    ssd_1306_write_command(0x81); 
    ssd_1306_write_command(value);
}

/**
 * 
 */
void ssd_1306_draw_polyline(){

}

/**
 * 
 */
void ssd_1306_draw_line(uint8_t x_1, uint8_t y_1, uint8_t x_0, uint8_t y_0, ssd_1306_color color){
    int delt_x = abs(x_1 - x_0);
    int delt_y = abs(y_1 - y_0);

    int sign_x;
    int sign_y;
    int erro_1;
    int erro_2;

    if(x_0 < x_1)
        sign_x = 1;
    else 
        sign_x = -1;

    if(y_0 < y_1)
        sign_y = 1;
    else 
        sign_y = -1;

    erro_1 = delt_x - delt_y;

    ssd_1306_draw_pixel(x_1, y_1, color);

    while((x_0 != x_1) || (y_0 != y_1)){
        ssd_1306_draw_pixel(x_0, y_0, color);
        erro_2 = erro_1*2;
        if(erro_2 > -delt_y){
            erro_1 -= delt_y;
            x_0 += sign_x;
        }
        if(erro_2 < delt_x){
            erro_1 += delt_x;
            y_0 += sign_y;
        }
    }

    return 0;
}
/**
 * @
 */
void ssd_1306_draw_rectangle(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, ssd_1306_color color){
    ssd_1306_draw_line(x_0, y_0, x_1, y_0, color);
    ssd_1306_draw_line(x_1, y_0, x_1, y_1, color);
    ssd_1306_draw_line(x_1, y_1, x_0, y_1, color);
    ssd_1306_draw_line(x_0, y_1, x_0, y_0, color);
}
/**
 * @
 */
void ssd_1306_set_cursor(uint8_t x, uint8_t y){
    ssd_1306.corde_x = x;
    ssd_1306.corde_y = y;
}

/**
 * @
 */
char ssd_1306_write_string(char *str, ssd_1306_font font, ssd_1306_color color){
    while(*str){
        if(ssd_1306_write_char(*str, font, color) != *str)
            return *str;
        str++;
    }

    return *str;
}

/**
 * @
 */
char ssd_1306_write_char(char _char_, ssd_1306_font font, ssd_1306_color color){
    if(_char_ < 32 || _char_ > 126)
        return 0;
    if(SSD1306_WIDTH < (ssd_1306.corde_x + font.width) || SSD1306_HEIGHT < (ssd_1306.corde_y + font.height))
        return 0;
    
    for(int i = 0; i < font.height; i++){
        int _dado_ = font.data[(_char_ - 32)*font.height + i];
        for(int j = 0; j < font.width; j++){
            if((_dado_ << j) & 0x8)
                ssd_1306_draw_pixel(ssd_1306.corde_x + j, (ssd_1306.corde_y + i), (ssd_1306_color)color);
            else 
                ssd_1306_draw_pixel(ssd_1306.corde_x + j, (ssd_1306.corde_y + i), (ssd_1306_color)!color);
        }
    }

    ssd_1306.corde_x += font.char_width ? font.char_width[_char_ - 32] : font.width;

    return _char_;
}

/**
 * @details Desenha um pixel na tela em coordenadas especificadas
 */
void ssd_1306_draw_pixel(uint8_t x, uint8_t y, ssd_1306_color color){
    if(x >= SSD1306_WIDTH || y > SSD1306_HEIGHT)
        return;
    
    // se color == white
    if(color != black)
        buffer[x + (y/8)*SSD1306_WIDTH] |= 1<<(y%8);
    else
        buffer[x+(y/8)*SSD1306_WIDTH] &= ~(1<<(y%8));
}