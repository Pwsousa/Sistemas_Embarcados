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

/**
 * @param byte 8 bits a ser enviado
 * @return Nao retorna nada
 */
void ssd_1306_write_command(uint8_t byte){

    uint8_t _buffer_[2];
    _buffer_[0] = 0x00;     // Endereco do registrador
    _buffer_[1] = byte;     // Dado a ser enviado

    i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, _buffer_, sizeof(_buffer_), false);
}

/**
 * @param buff 
 * @param _len
 * @return
 * Retorna Ok ou ERROR
 */
ssd_1306_erro ssd_1306_fill_buffer(uint8_t *buff, uint8_t _len){
    
    if(_len <= SSD1306_BUFFER_SIZE){
        memcpy(buffer, buff, _len);
        return OK;
    }
    return ERROR;
}

/**
 * @author Lucas Guilherme
 * @param buff
 * @param buff_size
 * @return
 * Sem retorno
 */
void ssd_1306_write_data(uint8_t *buff, size_t buff_size){

    uint8_t _buffer_temp_[buff_size + 1];
    _buffer_temp_[0] = 0x40;

    memcpy(&_buffer_temp_[1], buff, buff_size);
    i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, _buffer_temp_, sizeof(_buffer_temp_), false);
   
}

/**
 *@param void sem parametro
 */
void ssd_1306_init(void){

    ssd_1306_reset();

    // Wait for the screen to boot
    sleep_ms(100);

    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Init OLED
    // display off
    ssd_1306_set_display_on_off(0);

    ssd_1306_write_command(0x20); //Set Memory Addressing Mode
    ssd_1306_write_command(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                // 10b,Page Addressing Mode (RESET); 11b,Invalid

    ssd_1306_write_command(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    ssd_1306_write_command(0xC0); // Mirror vertically
#else
    ssd_1306_write_command(0xC8); //Set COM Output Scan Direction
#endif

    ssd_1306_write_command(0x00); //---set low column address
    ssd_1306_write_command(0x10); //---set high column address

    ssd_1306_write_command(0x40); //--set start line address - CHECK

    ssd_1306_set_contrast(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    ssd_1306_write_command(0xA0); // Mirror horizontally
#else
    ssd_1306_write_command(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef SSD1306_INVERSE_COLOR
    ssd_1306_write_command(0xA7); //--set inverse color
#else
    ssd_1306_write_command(0xA6); //--set normal color
#endif

// Set multiplex ratio.
#if (SSD1306_HEIGHT == 128)
    // Found in the Luma Python lib for SH1106.
    ssd_1306_write_command(0xFF);
#else
    ssd_1306_write_command(0xA8); //--set multiplex ratio(1 to 64) - CHECK
#endif

#if (SSD1306_HEIGHT == 32)
    ssd_1306_write_command(0x1F); //
#elif (SSD1306_HEIGHT == 64)
    ssd_1306_write_command(0x3F); //
#elif (SSD1306_HEIGHT == 128)
    ssd_1306_write_command(0x3F); // Seems to work for 128px high displays too.
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd_1306_write_command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    ssd_1306_write_command(0xD3); //-set display offset - CHECK
    ssd_1306_write_command(0x00); //-not offset

    ssd_1306_write_command(0xD5); //--set display clock divide ratio/oscillator frequency
    ssd_1306_write_command(0xF0); //--set divide ratio

    ssd_1306_write_command(0xD9); //--set pre-charge period
    ssd_1306_write_command(0x22); //

    ssd_1306_write_command(0xDA); //--set com pins hardware configuration - CHECK

#if (SSD1306_HEIGHT == 32)
    ssd_1306_write_command(0x02);
#elif (SSD1306_HEIGHT == 64)
    ssd_1306_write_command(0x12);
#elif (SSD1306_HEIGHT == 128)
    ssd_1306_write_command(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd_1306_write_command(0xDB); //--set vcomh
    ssd_1306_write_command(0x20); //0x20,0.77xVcc

    ssd_1306_write_command(0x8D); //--set DC-DC enable
    ssd_1306_write_command(0x14); //
    ssd_1306_set_display_on_off(1); //--turn on SSD1306 panel

    // Clear screen
    ssd_1306_fill(black);
    
    // Flush buffer to screen
    ssd_1306_up_date_screen();
    
    // Set default values for screen object
    ssd_1306.corde_x = 0;
    ssd_1306.corde_y = 0;
    ssd_1306.iniciali = 1;
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
 * @param void
 * @return uint8_t
 */
uint8_t ssd_1306_get_display_on(void){
    return ssd_1306.display_on;
}

/**
 * @param on 
 * @return void
 * @
 */
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
 * @param uint8_t x
 * @param uint8_t y
 * @param char bitmap
 * @param uint8_t w
 * @param uint8_t h
 * @param ssd_1306_color color
 * @return void
 * @paragraph Essa funcao e para desenhar um bitmap
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
 * @param uint8_t value
 * @return void
 * const
 */
void ssd_1306_set_contrast(uint8_t value){

    ssd_1306_write_command(0x81);  // Trocar esse 0x81 por uma definição
    ssd_1306_write_command(value);
}

/**
 *@param ssd_1306_verti *verte
 *@param uint16_t size
 *@param ssd_1306_color color
 *@return void
 *@note
 */
void ssd_1306_draw_polyline(ssd_1306_verti *verte, uint16_t size, ssd_1306_color color){

    if(verte == NULL)
        return;

    for( uint16_t i = 1; i < size; i++)
        ssd_1306_draw_line(verte[i - 1].x, verte[i - 1].y, verte[i].x, verte[i].y, color);

    return;
}

/**
 * @param uint8_t x_1
 * @param uint8_t y_1
 * @param uint8_t x_0
 * @param uint8_t y_0
 * @param ssd_1306_color color
 * @return void
 * @note Draw line by Bresenhem's algorithm 
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
 * @author Lucas Guilherme
 * @param uint8_t x_0
 * @param uint8_t x_1
 * @param uint8_t y_0
 * @param uint8_t y_1
 * @param ssd_1306_color color
 * @return void
 */
void ssd_1306_draw_rectangle(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, ssd_1306_color color){

    ssd_1306_draw_line(x_0, y_0, x_1, y_0, color);
    ssd_1306_draw_line(x_1, y_0, x_1, y_1, color);
    ssd_1306_draw_line(x_1, y_1, x_0, y_1, color);
    ssd_1306_draw_line(x_0, y_1, x_0, y_0, color);
}

/**
 * @param uint8_t x_0
 * @param uint8_t x_1
 * @param uint8_t y_0
 * @param uint8_t y_1
 * @return OK ou ERROR
 */
ssd_1306_erro ssd_1306_inverte_retangulo(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1){

}

/**
 * @author Lucas Guilherme
 * @param uint8_t x_0
 * @param uint8_t x_1
 * @param uint8_t y_0
 * @param uint8_t y_1
 * @param ssd_1306_color color
 * @return void
 * @paragraph
 */
void ssd_1306_fill_rectangle(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, ssd_1306_color color){
    
    // Verificar o maior entre as entradas
    uint8_t start_x, start_y, end_x, end_y;

    start_x = ((x_0 <= x_1) ? x_0:x_1);
    start_y = ((y_0 <= y_1) ? y_0:y_1);
    end_x = ((x_0 <= x_1) ? x_1:x_0);
    end_y = ((y_0 <= y_1) ? y_1:y_0);

    // Inverter o retangulo
    for(int y = start_y; (y <= end_y)&&(y < SSD1306_HEIGHT); y++){
        for(int x = start_x; (x <= end_x)&&(x < SSD1306_WIDTH); x++)
            ssd_1306_draw_pixel(x, y, color);
    }
    return; // retirar depois 
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

/**
 * @param uint8_t x
 * @param uint8_t y
 * @param uint8_t radius
 * @param uint16_t start_angle
 * @param uint16_t sweep
 * @param ssd_1306_color color
 * @return void
 * @note Draw arc with radius line
 */
void ssd1306_draw_arc_with_radius_line(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, ssd_1306_color color){
    return;
}

/**
 * @author Lucas Guilherme
 * @param uint8_t cord_x
 * @param uint8_t cord_y
 * @param  uint8_t par_r
 * @param ssd_1306_color color
 * @return void
 * @paragraph 
 * @note Draw circle by Bresenhem's algorithm
 */
void ssd_1306_draw_circle(uint8_t cord_x, uint8_t cord_y, uint8_t par_r, ssd_1306_color color){
    if(cord_y >= SSD1306_HEIGHT || cord_x >= SSD1306_WIDTH)
        return;

    uint32_t x, y, erro_1, erro_2;

    x = -par_r;
    y = 0;
    erro_1 = 2 - 2*par_r;

    do{
        ssd_1306_draw_pixel(cord_x - x, cord_y + y, color);
        ssd_1306_draw_pixel(cord_x + x, cord_y + y, color);
        ssd_1306_draw_pixel(cord_x + x, cord_y - y, color);
        ssd_1306_draw_pixel(cord_x - x, cord_y - y, color);

        erro_2 = erro_1;

        if(erro_2 <= y){
            y++;
            erro_1 = erro_1 + (y*2 + 1);
            if(-x == y && erro_2 <= x)
                erro_2 = 0;
        }
        if( erro_2 > x){
            x++;
            erro_1 = erro_1 + (x*2 + 1);
        }
    }while( x <= 0);

    return;
}

/**
 * @param uint8_t cord_x 
 * @param uint8_t cord_y
 * @param uint8_t par_r 
 * @param ssd_1306_color color
 * @return void
 * @paragraph
 * @note
 */
void ssd_1306_fill_circle(uint8_t cord_x, uint8_t cord_y, uint8_t par_r, ssd_1306_color color){
    
    if(cord_x >= SSD1306_WIDTH || cord_y >= SSD1306_HEIGHT)
        return;

    uint32_t x, y, erro_1, erro_2;

    erro_1 = 2 - 2*par_r;

    do{
        for(uint8_t a = (cord_y+y); a >= (cord_y - y); a--)
            for(uint8_t b = (cord_x - x); b >= (cord_x + x); b--)
                ssd_1306_draw_pixel(a, b, color);

        erro_2 = erro_1;

        if(erro_2 <= y){
            y++;
            erro_1 = erro_1 + (y*2 + 1);
            if(-x == y && erro_2 <= x)
                erro_2 = 0;
        }

        if(erro_2 > x){
            x++;
            erro_1 = erro_1 + (x*2 + 1);
        }

    }while(x<= 0);

    return;
}

/**
 * 
 */

