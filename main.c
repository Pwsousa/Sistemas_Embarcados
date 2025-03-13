#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd_1306.h"
#include "ssd_1306_font.h"

void testa_tudo(void);

int main(){

    stdio_init_all();

    ssd_1306_init();
    ssd_1306_fill(black);
    ssd_1306_draw_rectangle(0, 0, 127, 63, white);
    ssd_1306_set_cursor(20, 30);
    ssd_1306_write_string("Deus e bom", Font_7x10, white);
    ssd_1306_up_date_screen();
    sleep_ms(500);

    ssd_1306_fill(black);
    ssd_1306_up_date_screen();

    while (true) {
        testa_tudo();

        ssd_1306_fill(white);
        ssd_1306_up_date_screen();
        sleep_ms(500);
        ssd_1306_fill(black);
        ssd_1306_up_date_screen();
    }
}


ssd_1306_verti vert_[] = {
    {10, 10},
    {20, 20},
    {30, 30},
    {40, 40}
};


void testa_tudo(void){
    /************************** Desenhando retangulo *************************/
    ssd_1306_fill(black);
    ssd_1306_draw_rectangle(0, 0, 10, 10, white);
    ssd_1306_up_date_screen();
    sleep_ms(2000);

    ssd_1306_fill(black);
    ssd_1306_fill_rectangle(0, 0, 10, 10, black);
    ssd_1306_up_date_screen();
    sleep_ms(2000);
    /************************** Desenhando polylinha *************************/
    ssd_1306_fill(black);
    ssd_1306_draw_polyline(vert_, 10, white);
    ssd_1306_up_date_screen();
    sleep_ms(2000);

    /************************** Desenhando circulo *************************/
    ssd_1306_fill(black);
    ssd_1306_draw_circle(20, 1, 20, white);
    ssd_1306_up_date_screen();
    ssd_1306_fill_circle(20, 10, 10, white);
    ssd_1306_up_date_screen();
    sleep_ms(2000);
    /************************** Desenhando arco com linha ***************/
    ssd_1306_fill(black);
    ssd_1306_draw_arc_with_radius_line(10, 10, 20, 90, 1, white);
    ssd_1306_up_date_screen();
    sleep_ms(2000);
    /************************** Desenhando arco *************************/
    ssd_1306_fill(black);
    ssd_1306_draw_arc(10, 10, 20, 45, 1, white);
    ssd_1306_up_date_screen();
    sleep_ms(2000);
    /************************** Desenhando linha *************************/
    ssd_1306_fill(black);
    ssd_1306_draw_line(0, 0, 163, 0, white);
    ssd_1306_set_cursor(20, 20);
    ssd_1306_up_date_screen();
    sleep_ms(1000);
    ssd_1306_draw_line(0, 0, 0, 63, white);
    ssd_1306_set_cursor(20, 20);
    ssd_1306_up_date_screen();
    sleep_ms(1000);
    // Testando o desenho pixel a pixel
    // testando linhas 
    for(int i = 0; i<123; i++){
        ssd_1306_draw_pixel(i, 10, white);
        ssd_1306_set_cursor(10, 10);
        ssd_1306_up_date_screen();
        sleep_ms(50);
    }
    // Testatndo as colunas 
    for(int i = 0; i<63; i++){
        ssd_1306_draw_pixel(10, i, white);
        ssd_1306_set_cursor(10, 10);
        ssd_1306_up_date_screen();
        sleep_ms(50);
    }

    /************************** Desligando o OLED *************************/
    ssd_1306_set_display_on_off(0);
    ssd_1306_up_date_screen();

}