/************************* includes *************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "math.h"

#include "ssd_1306.h"

#if defined(SSD1306_USE_I2C)

#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15 

void ssd_1306_reset(void){
    
    /*do nothing*/
}

/**
 * @param byte 8 bits a ser enviado
 * @return Nao retorna nada
 * @note Envia um byte para o registrador 
 */
void ssd_1306_write_command(uint8_t byte){

    uint8_t _buffer_[2];
    _buffer_[0] = 0x00;     // Endereco do registrador
    _buffer_[1] = byte;     // Dado a ser enviado

    i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, _buffer_, sizeof(_buffer_), false);
}

/**
 * @author Lucas Guilherme
 * @param buff
 * @param buff_size
 * @return void 
 * @note Envia os dados 
 */
void ssd_1306_write_data(uint8_t *buff, size_t buff_size){

    uint8_t _buffer_temp_[buff_size + 1];
    _buffer_temp_[0] = 0x40;

    memcpy(&_buffer_temp_[1], buff, buff_size);
    i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, _buffer_temp_, sizeof(_buffer_temp_), false);
   
}

#endif

/************************* variaveis *************************/
static uint8_t buffer_ssd_1306[SSD1306_BUFFER_SIZE];
static ssd_1306_t ssd_1306;
/************************* Funcoes *************************/

/**
 * @param buff 
 * @param _len
 * @return Retorna Ok ou ERROR 
 * @note
 */
ssd_1306_erro ssd_1306_fill_buffer(uint8_t *buff, uint32_t _len){
    
    if(_len <= SSD1306_BUFFER_SIZE){
        memcpy(buffer_ssd_1306, buff, _len);
        return OK;
    }
    return ERROR;
}

/**
 * @param void void
 * @return void
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

    ssd_1306_write_command(SET_MEMORY_ADDR_MODE); //Set Memory Addressing Mode
    ssd_1306_write_command(SET_LOWER_COLUMN);     // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                                  // 10b,Page Addressing Mode (RESET); 11b,Invalid

    ssd_1306_write_command(SET_PAGE_START); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    ssd_1306_write_command(0xC0); // Mirror vertically
#else
    ssd_1306_write_command(SET_COM_OUT_SCAN_DIRE); //Set COM Output Scan Direction
#endif

    ssd_1306_write_command(SET_LOWER_COLUMN); //---set low column address
    ssd_1306_write_command(SET_HIGHER_COLUMN); //---set high column address

    ssd_1306_write_command(SET_DISPLAY_LINE); //--set start line address - CHECK

    ssd_1306_set_contrast(MAX_CONTRAST);

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
    ssd_1306_set_display_on_off(SSD1306_ON); //--turn on SSD1306 panel

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
 * @param ssd_1306_color color
 * @note Preenche o buffer com a cor especificada
 */
void ssd_1306_fill(ssd_1306_color color){

    memset(buffer_ssd_1306, (color == black) ? 0x00: 0xFF, sizeof(buffer_ssd_1306));
}

/**
 * @author Lucas Guilherme
 * @param void
 * @return uint8_t
 * @note
 */
uint8_t ssd_1306_get_display_on(void){
    return ssd_1306.display_on;
}

/**
 * @param void
 * @return void
 * @note Write data to each page of RAM. Number of pages depends on the screen height:
 * 32px == 4 ppages
 * 64px == 8 pages
 * 128px == 16 page
 */
void ssd_1306_up_date_screen(void){

    for(uint8_t i = 0; i<SSD1306_HEIGHT/8; i++){
        ssd_1306_write_command(0xB0 + i);
        ssd_1306_write_command(0x00 + SSD1306_X_OFFSET_LOWER);
        ssd_1306_write_command(0x10 + SSD1306_X_OFFSET_UPPER);
        ssd_1306_write_data(&buffer_ssd_1306[SSD1306_WIDTH*i], SSD1306_WIDTH);
    }
}

/**
 * @param on 
 * @return void
 * @
 */
void ssd_1306_set_display_on_off(const uint8_t on){
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
void ssd_1306_draw_bitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, ssd_1306_color color){
    
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
void ssd_1306_draw_polyline(const ssd_1306_verti *verte, uint16_t size, ssd_1306_color color){

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
    
    int32_t delt_x = abs(x_1 - x_0);
    int32_t delt_y = abs(y_1 - y_0);

    int32_t sign_x;
    int32_t sign_y;
    int32_t erro_1;
    int32_t erro_2;

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

    return;
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
   
    // Verifica limites de tela
    if ((x_1 >= SSD1306_WIDTH) || (y_1 >= SSD1306_HEIGHT) || (x_0 > x_1) || (y_0 > y_1)) {
      return ERROR;
    }
  
    // Inicializa a variável de índice
    uint32_t i;
  
    // Verifica se o retângulo não está em uma única linha de 8 pixels
    if ((y_0 / 8) != (y_1 / 8)) {
      for (uint32_t x = x_0; x <= x_1; x++) {
        // Inverte bits na linha inicial parcial
        i = x + (y_0 / 8) * SSD1306_WIDTH;
        buffer_ssd_1306[i] ^= 0xFF << (y_0 % 8);
        i += SSD1306_WIDTH;
  
        // Inverte bits nas linhas intermediárias completas
        for (; i < x + (y_1 / 8) * SSD1306_WIDTH; i += SSD1306_WIDTH)
            buffer_ssd_1306[i] ^= 0xFF;
        
        // Inverte bits na linha final parcial
        buffer_ssd_1306[i] ^= 0xFF >> (7 - (y_1 % 8));
      }

    } else {
      // Se o retângulo está em uma única linha de 8 pixels
      const uint8_t mask = (0xFF << (y_0 % 8)) & (0xFF >> (7 - (y_1 % 8)));
      for (i = x_0 + (y_0 / 8) * SSD1306_WIDTH; i <= (uint32_t)x_1 + (y_1 / 8) * SSD1306_WIDTH; i++) {
        buffer_ssd_1306[i] ^= mask;
      }
    }
    return OK;
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
 * @param uint8_t x 
 * @param uint8_t y
 * @return void
 * @note Position the cursor 
 */
void ssd_1306_set_cursor(uint8_t x, uint8_t y){
    ssd_1306.corde_x = x;
    ssd_1306.corde_y = y;
}

/**
 * @param char *str
 * @param ssd_1306_font font
 * @param ssd_1306_color color
 * @return char 
 * @note
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
 * @param char _char_
 * @param ssd_1306_font font
 * @param ssd_1306_color color
 * @return cahr 
 * @note 
 */
char ssd_1306_write_char(char _char_, ssd_1306_font font, ssd_1306_color color){
   
    if(_char_ < 32 || _char_ > 126)
        return 0;

    if(SSD1306_WIDTH < (ssd_1306.corde_x + font.width) || SSD1306_HEIGHT < (ssd_1306.corde_y + font.height))
        return 0;
    
    int i, _dado_, j;
    for(i = 0; i < font.height; i++){
        _dado_ = font.data[(_char_ - 32)*font.height + i];
        for(j = 0; j < font.width; j++){
            if((_dado_ << j) & 0x8000)
                ssd_1306_draw_pixel(ssd_1306.corde_x + j, (ssd_1306.corde_y + i), (ssd_1306_color)color);
            else 
                ssd_1306_draw_pixel(ssd_1306.corde_x + j, (ssd_1306.corde_y + i), (ssd_1306_color)!color);
        }
    }

    ssd_1306.corde_x += font.char_width ? font.char_width[_char_ - 32] : font.width;

    return _char_;
}

/**
 * @param uint8_t x
 * @param uint8_t y
 * @param ssd_1306_color color
 * @note Desenha um pixel na tela em coordenadas especificadas
 */
void ssd_1306_draw_pixel(uint8_t x, uint8_t y, ssd_1306_color color){

    if(x >= SSD1306_WIDTH || y > SSD1306_HEIGHT)
        return;
    
    // se color == white
    if(color != black)
        buffer_ssd_1306[x + (y/8)*SSD1306_WIDTH] |= 1<<(y%8);
    else
        buffer_ssd_1306[x+(y/8)*SSD1306_WIDTH] &= ~(1<<(y%8));
}

static float ssd_1306_deg_to_rad( float degree){

    float _degree = (degree*(3.14f/180.0f));

    return _degree;
}

static uint16_t ssd_1306_normalizacao(uint16_t degree){

    uint16_t loc_angle = degree % 360;
    return ((loc_angle == 0 && degree != 0) ? 360 : loc_angle);
}

/**
 * @
 */
void ssd_1306_draw_arc(uint8_t x_center, uint8_t y_center, uint8_t radius, uint16_t start_angle, uint16_t end_angle, ssd_1306_color color) {
    // Converte os ângulos para radianos
    float rad_start = start_angle * 3.14159 / 180.0;
    float rad_end = end_angle * 3.14159 / 180.0;

    // Usa o Algoritmo de Bresenham adaptado para um arco
    int x = radius, y = 0;
    int decision = 1 - radius;

    while (x >= y) {
        // Calcula os pontos do arco dentro do intervalo de ângulo
        float angle1 = atan2((float)y, (float)x);
        float angle2 = atan2((float)x, (float)y);
        
        if (angle1 >= rad_start && angle1 <= rad_end) {
            ssd_1306_draw_pixel(x_center + x, y_center + y, color);
            ssd_1306_draw_pixel(x_center - x, y_center + y, color);
        }

        if (angle2 >= rad_start && angle2 <= rad_end) {
            ssd_1306_draw_pixel(x_center + y, y_center + x, color);
            ssd_1306_draw_pixel(x_center - y, y_center + x, color);
        }

        if (decision <= 0) {
            y++;
            decision += 2 * y + 1;
        } else {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}

/**
 * @author Lucas Guilherme
 * @param uint8_t center_x
 * @param uint8_t center_y
 * @param  uint8_t radius
 * @param ssd_1306_color color
 * @return void
 * @paragraph 
 * @note Draw circle by Bresenhem's algorithm
 */

void ssd_1306_draw_circle(uint8_t center_x, uint8_t center_y, uint8_t radius, ssd_1306_color color) {
    int x = 0;
    int y = radius;
    int decision = 3 - (2 * radius);

    while (x <= y) {
        // Desenhar os 8 octantes do círculo
        ssd_1306_draw_pixel(center_x + x, center_y + y, color);
        ssd_1306_draw_pixel(center_x + y, center_y + x, color);
        ssd_1306_draw_pixel(center_x - x, center_y + y, color);
        ssd_1306_draw_pixel(center_x - y, center_y + x, color);
        ssd_1306_draw_pixel(center_x - x, center_y - y, color);
        ssd_1306_draw_pixel(center_x - y, center_y - x, color);
        ssd_1306_draw_pixel(center_x + x, center_y - y, color);
        ssd_1306_draw_pixel(center_x + y, center_y - x, color);

        if (decision <= 0) {
            decision += (4 * x) + 6;
        } else {
            decision += (4 * (x - y)) + 10;
            y--;
        }
        x++;
    }
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

