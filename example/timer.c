#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define PIN_LED_A 13

int led_value = 0;

bool repeating_timer_callback(struct repeating_timer *t) {
    led_value = 1 * led_value;
    gpio_put(PIN_LED_A, led_value);
    return true;
};


void setup();

int main(){
    struct repeating_timer timer;
    add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);

    while(true){
        tight_loop_contents();
    }
    
}

void setup(){
    stdio_init_all();
    gpio_init(PIN_LED_A);
    gpio_set_dir(PIN_LED_A, GPIO_OUT);
}