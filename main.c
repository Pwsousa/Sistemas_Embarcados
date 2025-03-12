#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd_1306.h"
#include "ssd_1306_font.h"
int main()
{
    stdio_init_all();

    ssd_1306_init();

    ssd_1306_fill(black);

    while (true) {
    }
}
